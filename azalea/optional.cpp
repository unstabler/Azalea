//
// Created by cheesekun on 19/05/19.
//

#include "optional.hpp"

#include <stdexcept>


template<typename T>
Optional<T>::Optional() :
    _value(nullptr)
{

}

template<typename T>
Optional<T>::Optional(T &value) :
    _value(nullptr)
{
    set(value);
}

template<typename T>
Optional<T>::~Optional()
{

}

template<typename T>
T& Optional<T>::get()
{
    if (!this->isPresent()) {
        throw std::runtime_error("value is not present");
    }
    return *_value;
}

template<typename T>
void Optional<T>::set(const T &value)
{
    _value.reset(new T(value));
}

template <typename T>
void Optional<T>::clear()
{
    _value.reset(nullptr);
}

template<typename T>
bool Optional<T>::isPresent() const
{
    return _value != nullptr;
}

template<typename T>
const T *Optional<T>::operator=(const T &value)
{
    set(value);
    return get();
}
