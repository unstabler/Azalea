#ifndef POSTAREA_HPP
#define POSTAREA_HPP

#include <QWidget>
#include <QShortcut>
#include <QFrame>

#include <ui/timeline/statusadapterbase.hpp>

class MainWindow;

namespace Ui {
    class PostArea;
}

class PostArea : public QFrame
{
        Q_OBJECT
        
    public:
        explicit PostArea(QWidget *parent);
        ~PostArea();
    
        bool isFocused();
        
    public slots:
        void textChanged();
    
        /**
         * 포스트의 최대 길이를 설정합니다.
         *
         * @note 인스턴스마다 최대 포스팅 길이가 다르므로 인스턴스가 변경될 때마다 호출되어야 합니다
         */
        void setMaxPostLength(unsigned maxPostLength);
        void updatePostLength();
    
        void setReplyTo(StatusAdapterBase *replyTo);
        void setText(QString text);
        
        /**
         * 현재 입력중인 포스트를 전송합니다
         */
        void submitPost();
        
        void focusPostArea();
        
        /**
         * 입력 창의 포커스를 해제합니다.
         */
        void blurPostArea();
        
private:
        Ui::PostArea *ui;
        QShortcut *_postShortcut;
        StatusAdapterBase *_replyTo;
        
        unsigned _maxPostLength;
        unsigned _postLength;
        
        MainWindow* getMainWindow();
};

#endif // POSTAREA_HPP
