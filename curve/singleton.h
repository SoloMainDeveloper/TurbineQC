#ifndef UTILS_SINGLETON_H
#define UTILS_SINGLETON_H // GCC keeps whining about redefining Singleton<T> without this
#pragma once

template<typename T>
class Singleton {
protected:
    Singleton()
    {
        if(exists()) {
            throw new std::runtime_error("Duplicate singleton instance");
        }

        _instance = static_cast<T*>(this);
    }

    virtual ~Singleton()
    {
        _instance = nullptr;
    }

    static T* instance()
    {
        if(!exists()) {
            throw new std::runtime_error("No singleton instance");
        }

        return _instance;
    }

    static T* instanceIfExists()
    {
        return _instance;
    }

private:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;

    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static bool exists()
    {
        return _instance != nullptr;
    }

    inline static T* _instance = nullptr; // C++17 allows inline static members
};

#endif // UTILS_SINGLETON_H
