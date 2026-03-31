#include "storage.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <dpapi.h>

#pragma comment(lib, "Crypt32.lib")

namespace vanish {
namespace storage {

EncryptedStore::EncryptedStore(const std::string& master_password) 
    : _password(master_password), _file_path("keys.dat") {
    _load_from_disk();
}

bool EncryptedStore::save_key(const std::string& platform, const std::string& api_key) {
    std::lock_guard<std::mutex> lock(_mutex);
    _keys[platform] = api_key;
    return _save_to_disk();
}

std::string EncryptedStore::load_key(const std::string& platform) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _keys.find(platform);
    if (it != _keys.end()) {
        return it->second;
    }
    return "";
}

bool EncryptedStore::_load_from_disk() {
    std::ifstream file(_file_path, std::ios::binary);
    if (!file.is_open()) return false;

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    if (size <= 0) return false;
    file.seekg(0, std::ios::beg);

    std::vector<BYTE> encrypted_data(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(encrypted_data.data()), size)) return false;

    DATA_BLOB data_in;
    data_in.pbData = encrypted_data.data();
    data_in.cbData = static_cast<DWORD>(encrypted_data.size());

    DATA_BLOB entropy;
    entropy.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(_password.data()));
    entropy.cbData = static_cast<DWORD>(_password.size());

    DATA_BLOB data_out;
    if (CryptUnprotectData(&data_in, nullptr, _password.empty() ? nullptr : &entropy, nullptr, nullptr, 0, &data_out)) {
        std::string json_str(reinterpret_cast<char*>(data_out.pbData), data_out.cbData);
        LocalFree(data_out.pbData);

        // hacking together a dirty json parser right here so we don't have to pull in a massive library just for two keys
        size_t pos = 0;
        while ((pos = json_str.find("\"", pos)) != std::string::npos) {
            size_t end_key = json_str.find("\"", pos + 1);
            if (end_key == std::string::npos) break;
            std::string key = json_str.substr(pos + 1, end_key - pos - 1);

            size_t start_val = json_str.find("\"", end_key + 1);
            if (start_val == std::string::npos) break;
            size_t end_val = json_str.find("\"", start_val + 1);
            if (end_val == std::string::npos) break;

            std::string val = json_str.substr(start_val + 1, end_val - start_val - 1);
            _keys[key] = val;
            pos = end_val + 1;
        }
        return true;
    }
    return false;
}

bool EncryptedStore::_save_to_disk() {
    // manually stringing together our json output.
    // sure it doesn't handle weird escapes too well but who cares? it's fine for simple api tokens
    std::string json_str = "{";
    bool first = true;
    for (const auto& pair : _keys) {
        if (!first) json_str += ",";
        json_str += "\"" + pair.first + "\":\"" + pair.second + "\"";
        first = false;
    }
    json_str += "}";

    DATA_BLOB data_in;
    data_in.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(json_str.data()));
    data_in.cbData = static_cast<DWORD>(json_str.size());

    DATA_BLOB entropy;
    entropy.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(_password.data()));
    entropy.cbData = static_cast<DWORD>(_password.size());

    DATA_BLOB data_out;
    if (CryptProtectData(&data_in, L"VanishKeys", _password.empty() ? nullptr : &entropy, nullptr, nullptr, 0, &data_out)) {
        std::ofstream file(_file_path, std::ios::binary);
        if (file.write(reinterpret_cast<char*>(data_out.pbData), data_out.cbData)) {
            LocalFree(data_out.pbData);
            return true;
        }
        LocalFree(data_out.pbData);
    }
    
    // honestly, if encryption fails down here we probably have worse OS permission problems anyway
    std::cerr << "Failed to encrypt/save keys." << std::endl;
    return false;
}

} // namespace storage
} // namespace vanish
