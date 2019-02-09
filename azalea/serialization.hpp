#pragma once

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

template <typename T>
inline QJsonObject toJSON(T *source)
{
    QJsonObject destination;
    toJSON(source, destination);
    
    return destination;
}

template <typename T>
void toJSON(T *source, QJsonObject &destination);

template <typename T>
void fromJSON(T *destination, QJsonObject source);

namespace serialization {
    template<typename TIterator>
    inline QJsonArray TO_JSONARRAY(TIterator first, TIterator last)
    {
        QJsonArray array;
        
        while (first != last) {
            auto value = *first;
            array.push_back(toJSON(value));
            first++;
        }
        
        return array;
    }
}

/**
 */
namespace deserialization {

    inline long LONG(const QJsonValueRef &valueRef)
    {
        // XXX
        return (long) valueRef.toDouble();
    }

    inline bool BOOL(const QJsonValueRef &valueRef)
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isBool());
        return valueRef.toBool();
    }

    inline QString STRING(const QJsonValueRef &valueRef) 
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isString());
        return valueRef.toString();
    }

    inline QJsonObject JSONOBJECT(const QJsonValueRef &valueRef)
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isObject());
        return valueRef.toObject();
    }
    
    inline QJsonArray JSONARRAY(const QJsonValueRef &valueRef)
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isArray());
        return valueRef.toArray();
    }
    
    template <typename T>
    inline T* OBJECT(const QJsonObject &value)
    {
        T* destination = new T;
        fromJSON(destination, value);
        return destination;
    }

    template <typename T>
    inline T* OBJECT(const QJsonValueRef &valueRef)
    {
        QJsonObject value = JSONOBJECT(valueRef);
        return OBJECT<T>(value);
    }
    
    template <typename T, class TOutputIterator>
    inline void TRANSFORM_ARRAY(const QJsonArray &array, TOutputIterator tBegin)
    {
        std::transform(
                array.begin(),
                array.end(),
                tBegin,
                [](const QJsonValue &jsonValue) {
                    return OBJECT<T>(jsonValue.toObject());
                }
        );
    }
    
    template <typename T>
    inline QList<T*>* ARRAY(const QJsonValueRef &valueRef)
    {
        auto *destination = new QList<T*>;
        TRANSFORM_ARRAY<T, std::back_insert_iterator<QList<T*>>>(
                JSONARRAY(valueRef),
                // FIXME: destination->begin() 쓰면 아무런 값도 들어가지 않는데 그 이유는 무엇일까요 . .)
                std::back_inserter(*destination)
        );
        return destination;
    }

}
