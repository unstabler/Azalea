//
// Created by cheesekun on 19/06/16.
//

#include "notificationstatusadapter.hpp"
#include <QTimeZone>

NotificationStatusAdapter::NotificationStatusAdapter(QObject *parent, QSharedPointer<v1::Notification> notification) :
    StatusAdapterBase(parent),
    _notification(notification)
{

}

QString NotificationStatusAdapter::id()
{
    return _notification->id;
}

QString NotificationStatusAdapter::content()
{
    if (_notification->type == "follow") {
        return tr("followed by %1").arg(_notification->account->username);
    } else if (_notification->status == nullptr) {
        // fixme
        return "";
    }
    return _notification->status->content;
}

/**
 * TODO: 이 부분은 StatusAdapterBase로 합치고
 * @return
 */
QString NotificationStatusAdapter::formattedAuthor()
{
    return formatAuthor(_notification->account);
}

QUrl NotificationStatusAdapter::avatarUrl()
{
    if (_notification->type == "reblog" ||
        _notification->type == "favourite") {
        return QUrl(_notification->status->account->avatar);
    }
    return QUrl(_notification->account->avatar);
}

QUrl NotificationStatusAdapter::interactAvatarUrl()
{
    if (_notification->type == "reblog" ||
        _notification->type == "favourite") {
        return QUrl(_notification->account->avatar);
    }
    
    return QUrl();
}

QString NotificationStatusAdapter::createdAt()
{
    return _notification->createdAt.toTimeZone(QTimeZone::systemTimeZone()).toString(Qt::SystemLocaleLongDate);
}
