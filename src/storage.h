#pragma once
#include <string>
#include <map>
#include <mutex>

namespace vanish {
namespace storage {

class EncryptedStore {
public:
    EncryptedStore(const std::string& master_password);
    
    bool save_key(const std::string& platform, const std::string& api_key);
    std::string load_key(const std::string& platform);

private:
    std::string _password;
    std::string _file_path;
    std::map<std::string, std::string> _keys;
    std::mutex _mutex;

    bool _load_from_disk();
    bool _save_to_disk();
};

} // namespace storage
} // namespace vanish
