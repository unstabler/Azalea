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
    return _notification->status->content;
}

/**
 * TODO: 이 부분은 StatusAdapterBase로 합치고
 * @return
 */
QString NotificationStatusAdapter::formattedAuthor()
{
    return QString("%1 (@%2)").arg(
            _notification->account->displayName,
            _notification->account->acct
    );
}

QUrl NotificationStatusAdapter::avatarUrl()
{
    return QUrl(_notification->account->avatar);
}

QString NotificationStatusAdapter::createdAt()
{
    return _notification->createdAt.toTimeZone(QTimeZone::systemTimeZone()).toString(Qt::SystemLocaleLongDate);
}
