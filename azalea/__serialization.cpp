#include <QString>
#include <cereal/cereal.hpp>

/**
 * XXX: 이거 맞는건가
 */
template<class Archive>
std::string save_minimal(const Archive&, const QString& string)
{
    return string.toStdString();
}

/**
 * XXX: 이거 맞는건가
 */
template<class Archive>
void load_minimal(const Archive&, QString& string, const std::string &value)
{
    string = QString::fromStdString(value);
}
