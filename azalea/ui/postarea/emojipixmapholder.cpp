//
// Created by cheesekun on 2019/10/06.
//

#include <QDebug>
#include <QtNetwork/QNetworkReply>
#include "emojipixmapholder.hpp"

EmojiPixmapHolder::EmojiPixmapHolder() :
    _networkAccessManager(this),
    _emojis()
{

}

QSharedPointer<QPixmap> EmojiPixmapHolder::get(QString shortcode)
{
    return _emojis[shortcode];
}

bool EmojiPixmapHolder::has(QString shortcode)
{
    return _emojis.contains(shortcode);
}

void EmojiPixmapHolder::fetch(QString shortcode, QString url)
{
    if (_fetching.contains(shortcode)) {
        return;
    }
    _fetching.insert(shortcode);
    
    QNetworkRequest request((QUrl(url)));
    auto *reply = _networkAccessManager.get(request);
    connect(reply, &QNetworkReply::finished, [this, shortcode, reply]() {
        QSharedPointer<QPixmap> pixmap(new QPixmap);
        pixmap->loadFromData(reply->readAll());
        _emojis.insert(shortcode, pixmap);
        
        emit ready(shortcode, pixmap);
    });
}
