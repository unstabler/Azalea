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
    if (_status->reblog != nullptr) {
        return formatAuthor(_status->reblog->account);
    }
    return formatAuthor(_status->account);
}

QUrl StatusAdapter::avatarUrl()
{
    if (_status->reblog != nullptr) {
        return QUrl(_status->reblog->account->avatar);
    }
    return QUrl(_status->account->avatar);
}

QUrl StatusAdapter::interactAvatarUrl()
{
    if (_status->reblog != nullptr) {
        return QUrl(_status->account->avatar);
    }
    return QUrl();
}

QString StatusAdapter::createdAt()
{
    return _status->createdAt.toTimeZone(QTimeZone::systemTimeZone()).toString(Qt::SystemLocaleLongDate);
}

bool StatusAdapter::isBoosted()
{
    return _status->reblogged;
}

void StatusAdapter::setBoosted(bool isBoosted)
{
    _status->reblogged = isBoosted;
    emit boostToggled(isBoosted);
}

bool StatusAdapter::isFavourited()
{
    return _status->favourited;
}

void StatusAdapter::setFavourited(bool isFavourited)
{
    _status->favourited = isFavourited;
    emit favouriteToggled(isFavourited);
}

QSharedPointer<v1::Status> StatusAdapter::status()
{
    return _status;
}
