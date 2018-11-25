#pragma once

#include <QJsonObject>

template <typename T>
void fromJSON(T *destination, QJsonObject source);

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

    template <typename T>
    inline T* OBJECT(const QJsonValueRef &valueRef)
    {
        T* destination = new T;
        fromJSON(destination, JSONOBJECT(valueRef));
        return destination;
    }

}
