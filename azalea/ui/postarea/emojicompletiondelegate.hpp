#pragma once

#include <QStyledItemDelegate>

#include "emojipixmapholder.hpp"

class EmojiCompletionDelegate : public QStyledItemDelegate
{
    public:
        static int EMOJI_SIZE;
        static int EMOJI_PADDING;
    
        EmojiCompletionDelegate(QObject *parent = nullptr);
    
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        
    private:
        EmojiPixmapHolder &_holder;
};