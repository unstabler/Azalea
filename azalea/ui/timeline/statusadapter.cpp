//
// Created by cheesekun on 19/02/10.
//

#include "statusadapter.hpp"
#include "statusformatter.hpp"

#include <QTimeZone>
#include <singleton.hpp>

StatusAdapterBase::StatusAdapterBase(QObject *parent) :
        QObject(parent)
{

}

StatusAdapter::StatusAdapter(QObject *parent, QSharedPointer<v1::Status> status) :
    StatusAdapterBase(parent),
    _status(status)
{

}

QString StatusAdapter::id()
{
    return _status->id;
}

QString StatusAdapter::content()
{
    auto statusFormatter = Singleton<StatusFormatter>::getInstance();
    return statusFormatter.toRichText(_status.get());
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

