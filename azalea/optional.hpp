//
// Created by cheesekun on 19/05/19.
//

#pragma once

template <typename T>
class Optional {
    public:
        explicit Optional();
        explicit Optional(T &value);
        Optional(const Optional&) = delete;
        ~Optional();
        
        const T &get() const;
        void set(T& value);
        void clear();
        bool isPresent() const;
        
        explicit operator T&() const;
    private:
        T *_value;
};

#include "optional.cpp"
