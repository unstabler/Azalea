//
// Created by cheesekun on 19/02/10.
//

#include "StatusAdapter.hpp"

#include <QTimeZone>

StatusAdapter::StatusAdapter(QObject *parent, QSharedPointer<v1::Status> status) :
    QObject(parent),
    _status(status)
{

}

QString StatusAdapter::content()
{
    return _status->content;
}

QString StatusAdapter::formattedAuthor()
{
    return QString("%1 (@%2)").arg(
            _status->account->displayName,
            _status->account->acct
    );
}

QUrl StatusAdapter::avatarUrl()
{
    return QUrl(_status->account->avatar);
}

QString StatusAdapter::createdAt()
{
    return _status->createdAt.toTimeZone(QTimeZone::systemTimeZone()).toString(Qt::SystemLocaleLongDate);
}

QSharedPointer<v1::Status> StatusAdapter::status()
{
    return _status;
}
