#pragma once

#include <QFrame>
#include <ui/timeline/statusadapterbase.hpp>

namespace Ui {
    class ReplyIndicator;
}

class ReplyIndicator : public QFrame
{
        Q_OBJECT
        
    public:
        explicit ReplyIndicator(QWidget *parent);
        ~ReplyIndicator();
        
    signals:
        void cancel();
        void statusChanged();
        
    public slots:
        void setReplyTo(StatusAdapterBase *replyTo);
        
    private:
        QScopedPointer<Ui::ReplyIndicator> ui;
        
        StatusAdapterBase *_replyTo;
};

