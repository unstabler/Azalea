//
// Created by cheesekun on 2019/10/06.
//
#include <QPainter>
#include <QDebug>
#include <QAbstractItemModel>

#include "singleton.hpp"
#include "emojicompletiondelegate.hpp"


EmojiCompletionDelegate::EmojiCompletionDelegate(QObject *parent) :
        QStyledItemDelegate(parent),
        _holder(Singleton<EmojiPixmapHolder>::getInstance())
{

}

void
EmojiCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem optionCopy = option;
    initStyleOption(&optionCopy, index);
    
    const auto &palette = optionCopy.palette;
    const auto &rect = optionCopy.rect;
    
    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
    auto font = optionCopy.font;
    
    auto shortcode = optionCopy.text;
    
    
    
    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    
    painter->fillRect(rect, (optionCopy.state & QStyle::State_Selected) ?
                palette.highlight().color() :
                palette.light().color());
    
    
    if (_holder.has(shortcode)) {
        auto pixmap = _holder.get(shortcode);
        painter->drawPixmap(rect.left(), rect.top(), 24, 24, *pixmap);
    } else {
        auto url = index.siblingAtColumn(1).data(Qt::DisplayRole);
        _holder.fetch(shortcode, url.toString());
    }
    
    auto textArea = rect;
    textArea.moveLeft(28);
    
    painter->drawText(textArea, Qt::TextSingleLine, optionCopy.text);
    painter->restore();
}

QSize EmojiCompletionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(320, 24);
}
