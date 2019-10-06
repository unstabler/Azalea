#pragma once

#include <QTextEdit>
#include <QCompleter>
#include "emojipixmapholder.hpp"

class TootEdit : public QTextEdit
{
        Q_OBJECT
        
    public:
        TootEdit(QWidget *parent = nullptr);
        ~TootEdit();
        
    public slots:
        void updateEmojiList();
        
    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void focusInEvent(QFocusEvent *event) override;
        
    private slots:
        void insertCompletion(const QString &completion);
    
    private:
        QCompleter _completer;
        EmojiPixmapHolder &_emojiPixmapHolder;
        
    
        QString textUnderCursor() const;
};

