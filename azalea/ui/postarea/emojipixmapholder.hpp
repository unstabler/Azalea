#pragma once

#include <QObject>
#include <QMap>
#include <QPixmap>
#include <QNetworkAccessManager>

class EmojiPixmapHolder : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(EmojiPixmapHolder)
    
    public:
        explicit EmojiPixmapHolder();
        
        QSharedPointer<QPixmap> get(QString shortcode);
        bool has(QString shortcode);
        void fetch(QString shortcode, QString url);
        
    signals:
        void ready(QString shortcode, QSharedPointer<QPixmap> pixmap);
        
    
    private:
        QNetworkAccessManager _networkAccessManager;
        QSet<QString> _fetching;
        QMap<QString, QSharedPointer<QPixmap>> _emojis;
};