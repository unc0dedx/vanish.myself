#pragma once
#include <string>

namespace vanish {
namespace storage {

class EncryptedStore {
public:
    EncryptedStore(const std::string& master_password);
    
    bool save_key(const std::string& platform, const std::string& api_key);
    std::string load_key(const std::string& platform);

private:
#ifndef _WIN32
    // TODO: AES-GCM encryption contexts 
#endif
    std::string _password;
};

} // namespace storage
} // namespace vanish
