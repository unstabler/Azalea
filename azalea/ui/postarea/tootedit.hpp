#pragma once

#include <QTextEdit>
#include <QCompleter>

class TootEdit : public QTextEdit
{
        Q_OBJECT
        
    public:
        TootEdit(QWidget *parent = nullptr);
        ~TootEdit();
        
    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void focusInEvent(QFocusEvent *event) override;
        
    private slots:
        void insertCompletion(const QString &completion);
    
    private:
        QCompleter _completer;
    
        QString textUnderCursor() const;
};

