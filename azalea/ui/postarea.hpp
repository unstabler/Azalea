#ifndef POSTAREA_HPP
#define POSTAREA_HPP

#include <QWidget>

/** 상호 참조 */
class MainWindow;

namespace Ui {
    class PostArea;
}

class PostArea : public QWidget
{
        Q_OBJECT
        
    public:
        explicit PostArea(QWidget *parent);
        ~PostArea();
        
    public slots:
        void textChanged();
    
        /**
         * 포스트의 최대 길이를 설정합니다.
         *
         * @note 인스턴스마다 최대 포스팅 길이가 다르므로 인스턴스가 변경될 때마다 호출되어야 합니다
         */
        void setMaxPostLength(unsigned maxPostLength);
        void updatePostLength();

    private:
        Ui::PostArea *ui;
        MainWindow *mainWindow();
        
        unsigned _maxPostLength;
        unsigned _postLength;
};

#endif // POSTAREA_HPP
