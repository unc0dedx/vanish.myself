#include "storage.h"
#include <iostream>

namespace vanish {
namespace storage {

EncryptedStore::EncryptedStore(const std::string& master_password) 
    : _password(master_password) {
    // initialize our cipher context here later.
}

bool EncryptedStore::save_key(const std::string& platform, const std::string& api_key) {
    std::cout << "Saving key for " << platform << " (stub)" << std::endl;
    // Write to local JSON and encrypt.
    return true;
}

std::string EncryptedStore::load_key(const std::string& platform) {
    // Read encrypted JSON and decrypt.
    return "dummy_key_for_now";
}

} // namespace storage
} // namespace vanish
