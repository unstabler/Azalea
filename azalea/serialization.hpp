#pragma once

#include <QString>
#include <cereal/cereal.hpp>

#include "mastodon/__serialization_helper.hpp"
#include "mastodon/v1/entities/__serialization_helper.hpp"

/**
 * QString의 serialization helper
 */
template<class Archive>
std::string save_minimal(const Archive&, const QString& string)
{
    return string.toStdString();
}

/**
 * QString의 deserialization helper
 */
template<class Archive>
void load_minimal(const Archive&, QString& string, const std::string &value)
{
    string = QString::fromStdString(value);
}
