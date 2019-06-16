#pragma once

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

#include "optional.hpp"

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
    
    template<typename T>
    inline void SET_OPTIONAL(QJsonValueRef jsonValue, Optional<T> &value) {
        if (value.isPresent()) {
            jsonValue = QVariant(value.get()).toString();
        }
    }
}

/**
 */
namespace deserialization {

    inline long LONG(const QJsonValue &valueRef)
    {
        // XXX
        return (long) valueRef.toDouble();
    }

    inline bool BOOL(const QJsonValue &valueRef)
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isBool());
        return valueRef.toBool();
    }

    inline QString STRING(const QJsonValue &valueRef)
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isString());
        return valueRef.toString();
    }

    inline QJsonObject JSONOBJECT(const QJsonValue &valueRef)
    {
        Q_ASSERT(valueRef.isNull() || valueRef.isObject());
        return valueRef.toObject();
    }
    
    inline QJsonArray JSONARRAY(const QJsonValue &valueRef)
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

    /**
     * value reference를 <T>로 변환합니다.
     * value reference가 null이나 undefined를 가리킬 경우 nullptr를 반환합니다.
     */
    template <typename T>
    inline T* OBJECT(const QJsonValue &valueRef)
    {
        if (valueRef.isNull() || valueRef.isUndefined()) {
            return nullptr;
        }
        
        QJsonObject value = JSONOBJECT(valueRef);
        return OBJECT<T>(value);
    }
    
    template <typename T>
    inline QSharedPointer<T> OBJECT_SHAREDPTR(const QJsonObject &value) {
        return QSharedPointer<T>(OBJECT<T>(value));
    }
    
    template <typename T>
    inline QSharedPointer<T> OBJECT_SHAREDPTR(const QJsonValue &valueRef) {
        return QSharedPointer<T>(OBJECT<T>(valueRef));
    }
    
    /**
     * @deprecated
     */
    template <typename T, class TOutputIterator>
    [[deprecated]]
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
    
    /**
     * QJsonValue에 들어있는 Object[]를 QList<T*>로 변환합니다
     * @deprecated
     */
    template <typename T>
    [[deprecated]]
    inline QList<T*>* ARRAY(const QJsonValue &valueRef)
    {
        auto *destination = new QList<T*>;
        TRANSFORM_ARRAY<T, std::back_insert_iterator<QList<T*>>>(
                JSONARRAY(valueRef),
                // FIXME: destination->begin() 쓰면 아무런 값도 들어가지 않는데 그 이유는 무엇일까요 . .)
                std::back_inserter(*destination)
        );
        return destination;
    }
     /**
     * QJsonValue에 들어있는 Object[]를 QList<SharedPtr<T>>로 변환합니다
     */
    template <typename T>
    inline QList<QSharedPointer<T>>* ARRAY_SHAREDPTR(const QJsonValue &valueRef)
    {
        auto *destination = new QList<QSharedPointer<T>>;
        auto sourceArray = JSONARRAY(valueRef);
        
        std::transform(
                sourceArray.begin(),
                sourceArray.end(),
                std::back_inserter(*destination),
                [](const QJsonValue &jsonValue) {
                    return OBJECT_SHAREDPTR<T>(jsonValue.toObject());
                }
        );
        
        return destination;
    }

}
