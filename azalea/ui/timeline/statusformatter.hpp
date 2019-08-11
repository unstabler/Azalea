#pragma once

#include <QSharedPointer>
#include "mastodon/v1/entities/mastodon.hpp"

class StatusFormatter
{
public:
    StatusFormatter();
    
    QString toRichText(v1::Status *status);
    QSharedPointer<v1::Emoji> resolveEmoji(v1::Status *status, const QString shortcode);
};

