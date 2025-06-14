#pragma once
#include <dlfcn.h>

#include <string>
#include <memory>
#include <iostream>

template <typename T>
class dlLoader {
 public:
    class dlError : public std::exception {
     private:
        std::string message;

     public:
        explicit dlError(const std::string& errorMsg)
            : message("dlLoader: " + errorMsg) {}
        const char *what() const noexcept override {
            return message.c_str();
        }
    };

    static bool verifyLib(const std::string &path, std::string getter);
    static std::unique_ptr<T> getLib(const std::string &path,
        std::string getter);
};

template <typename T>
inline bool dlLoader<T>::verifyLib(const std::string &path,
    std::string getter) {
    void *handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << path << "(1): " << dlerror() << std::endl;
        return false;
    }
    void* symbolPtr = dlsym(handle, getter.c_str());
    if (!symbolPtr) {
        std::cerr << path << "(2): " << dlerror() << std::endl;
        dlclose(handle);
        return false;
    }
    try {
        auto test = reinterpret_cast<std::unique_ptr<T> *>
            (symbolPtr);
        (void)test;
    } catch(const std::exception& e) {
        std::cerr << path << ": Bad format for entry point." << std::endl;
        return false;
    }
    dlclose(handle);
    return true;
}

template <typename T>
inline std::unique_ptr<T> dlLoader<T>::getLib(const std::string &path,
    std::string getter) {
    if (!verifyLib(path, getter))
        throw dlError("Invalid lib");
    void *handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle)
        throw dlError("Can't open lib");
    auto createModule = reinterpret_cast<std::unique_ptr<T> (*)()>
        (dlsym(handle, getter.c_str()));
    if (!createModule) {
        dlclose(handle);
        throw dlError("Can't find symbol");
    }
    auto module = createModule();
    return module;
}
