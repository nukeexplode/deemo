#pragma once

#include <mutex>

template <typename T>
class Singleton {
public:
    Singleton() = delete;
    ~Singleton() = delete;

    //
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& Instance() {
        if (val_ == nullptr) {
            val_ = new T();
        }

        return *val_;
    }

private:
    static T* val_;
};

template <typename T>
T* Singleton<T>::val_ = nullptr;

// template <typename T>
// class Singleton {
// public:
//     Singleton() = delete;
//     ~Singleton() = delete;

//     Singleton(const Singleton&) = delete;
//     Singleton& operator=(const Singleton&) = delete;

//     T& Instance() {
//         return val_;
//     }

// private:
//     static T* val_;
// };

// template <typename T>
// static T* Singleton<T>::val_ = new T();

// template <typename T>
// class Singeleton {
// pbulic:
//     Singleton() = delete;
//     ~Singleton() = delete;

//     Singleton(const Singleton&) = delete;
//     Singleton& operator=(const Singleton&) = delete;

//     T& Instance() {
//         if (val_ == nullptr) {
//             std::unique_lock lock(mutex);
//             if (val_ == nullptr) {
//                 val_ = new T();
//             }
//         }

//         return &val_;
//     }

// private:
//     static T* val_;
//     std::mutex mutex;
// };

// template <typename T>
// static T* Singeleton<T>::val_ = nullptr;