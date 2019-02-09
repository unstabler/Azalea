#pragma once

#include <QObject>
#include <QList>

#include "mastodon/v1/entities/mastodon.hpp"

class StatusAdapter : public QObject
{
    Q_OBJECT
    // TODO: Q_PROPERTY wrap
    
    private:
        v1::Status *_status;
        QList<StatusAdapter*> _replies;
};

