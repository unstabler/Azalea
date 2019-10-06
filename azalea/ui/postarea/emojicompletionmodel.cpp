//
// Created by cheesekun on 2019/10/03.
//

#include "emojicompletionmodel.hpp"

const int EmojiCompletionModel::COLUMN_INDEX_SHORTCODE = 0;
const int EmojiCompletionModel::COLUMN_INDEX_EMOJI_URL = 1;

EmojiCompletionModel::EmojiCompletionModel(QSharedPointer<ResourceList<v1::Emoji>> emojiList) :
    _emojiList(std::move(emojiList))
{

}

int EmojiCompletionModel::rowCount(const QModelIndex &parent) const
{
    return _emojiList->length();
}

int EmojiCompletionModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant EmojiCompletionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !(role == Qt::DisplayRole || role == Qt::EditRole)) {
        return QVariant();
    }
    
    auto emoji = _emojiList->at(index.row());
    if (index.column() == COLUMN_INDEX_SHORTCODE) {
        // 이거 이래도 되는건가?
        return ":" + emoji->shortcode + ":";
    } else if (index.column() == COLUMN_INDEX_EMOJI_URL) {
        return emoji->staticUrl;
    }
    
    return QVariant();
}


