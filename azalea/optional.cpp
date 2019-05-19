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
    _value(value)
{

}

template<typename T>
Optional<T>::~Optional()
{

}

template<typename T>
const T &Optional<T>::get() const
{
    if (!this->isPresent()) {
        throw std::runtime_error("value is not present");
    }
    return *_value;
}

template<typename T>
void Optional<T>::set(T &value)
{
    _value = &value;
}

template<typename T>
void Optional<T>::clear()
{
    _value = nullptr;
}

template<typename T>
bool Optional<T>::isPresent() const
{
    return _value != nullptr;
}

template<typename T>
Optional<T>::operator T&() const
{
    return get();
}
