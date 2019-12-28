#pragma once

#include <QAbstractItemModel>
#include <QSharedPointer>

#include "mastodon/apibase.hpp"
#include "mastodon/v1/entities/mastodon.hpp"

class EmojiCompletionModel : public QAbstractTableModel
{
    public:
        explicit EmojiCompletionModel(QSharedPointer<ResourceList<v1::Emoji>> emojiList);
    
        int rowCount(const QModelIndex &parent) const override;
        int columnCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        
    private:
        static const int COLUMN_INDEX_SHORTCODE;
        static const int COLUMN_INDEX_EMOJI_URL;
        
        QSharedPointer<ResourceList<v1::Emoji>> _emojiList;
};

