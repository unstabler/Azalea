//
// Created by cheesekun on 2019/10/03.
//

#include <QDebug>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QScrollBar>

#include "tootedit.hpp"

TootEdit::TootEdit(QWidget *parent) :
        QTextEdit(parent),
        _completer({":test:", ":blobowoevil:"}, this)
{
    _completer.setWidget(this);
    _completer.setCompletionMode(QCompleter::PopupCompletion);
    
    connect(&_completer, QOverload<const QString &>::of(&QCompleter::activated), this, &TootEdit::insertCompletion);
}

TootEdit::~TootEdit()
{
}

void TootEdit::keyPressEvent(QKeyEvent *event)
{
    if (_completer.popup()->isVisible()) {
        switch (event->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                event->ignore();
                return;
            default:
                break;
        }
    }
    
    bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space);
    if (!isShortcut) {
        QTextEdit::keyPressEvent(event);
    }
    
    const bool ctrlOrShift = event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (ctrlOrShift && event->text().isEmpty()) {
        return;
    }
    
    // TODO: 코드 정리.. X(
    static QString eow("~!@#$%^&*()_+{}|\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    
    // wordUnderCursor로는 :같은 특수문자가 붙지 않기 때문에 우선 pos 구하고 (posStart - 1, posEnd)로 컴플리터 띄움
    auto cursor = this->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    int startPos = cursor.selectionStart();
    int endPos   = cursor.selectionEnd();
    qDebug () << startPos << ", " << endPos;
    if (startPos >= 1) {
        cursor.setPosition(startPos - 1);
        cursor.setPosition(endPos, QTextCursor::KeepAnchor);
    }
    
    QString completionPrefix = cursor.selectedText();
    
    if (!completionPrefix.startsWith(":")) {
        _completer.popup()->hide();
        return;
    }

    if (!isShortcut && (hasModifier || event->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(event->text().right(1)))) {
        _completer.popup()->hide();
        return;
    }
    
    qDebug() << completionPrefix;
    if (completionPrefix != _completer.completionPrefix()) {
        _completer.setCompletionPrefix(completionPrefix);
        _completer.popup()->setCurrentIndex(_completer.completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(_completer.popup()->sizeHintForColumn(0)
                + _completer.popup()->verticalScrollBar()->sizeHint().width());
    _completer.complete(cr); // popup it up!
}

void TootEdit::focusInEvent(QFocusEvent *event)
{
    QTextEdit::focusInEvent(event);
}

void TootEdit::insertCompletion(const QString &completion)
{
    QTextCursor cursor = this->textCursor();
    int extra = completion.length() - _completer.completionPrefix().length();
    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::EndOfWord);
    cursor.insertText(completion.right(extra));
    cursor.insertText(" ");
    this->setTextCursor(cursor);
}

QString TootEdit::textUnderCursor() const
{
    QTextCursor cursor = this->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}
