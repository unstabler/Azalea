#ifndef POSTAREA_HPP
#define POSTAREA_HPP

#include <QWidget>

namespace Ui {
    class PostArea;
}

class PostArea : public QWidget
{
        Q_OBJECT

    public:
        explicit PostArea(QWidget *parent);
        ~PostArea();

    private:
        Ui::PostArea *ui;
};

#endif // POSTAREA_HPP
