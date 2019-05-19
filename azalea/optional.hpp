//
// Created by cheesekun on 19/05/19.
//

#pragma once

#include <memory>

template <typename T>
class Optional {
    public:
        explicit Optional();
        explicit Optional(T &value);
        Optional(const Optional&) = delete;
        ~Optional();
        
        T &get();
        void set(const T &value);
        void clear();
        bool isPresent() const;
        
        const T* operator= (const T& value);
    private:
        std::unique_ptr<T> _value;
};

#include "optional.cpp"
