#include "keyedit.h"

#include "QDebug"

KeyEdit::KeyEdit(QWidget *parent)
 : QLineEdit(parent)
{
    setReadOnly(true);
}

KeyEdit::~KeyEdit()
{

}

void KeyEdit::setKey(int keyID)
{
    if (keyID == Qt::Key_Alt)
        setText("Alt");
    else if (keyID == Qt::Key_AltGr)
        setText("Alt GR");
    else if (keyID == Qt::Key_Control)
        setText("Control");
    else if (keyID == Qt::Key_Shift)
        setText("Shift");
    else if (keyID == Qt::Key_Meta)
        setText("Meta");
    else
        setText(QKeySequence(keyID).toString());
}

void KeyEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);
  emit(focussed(true));
}

void KeyEdit::focusOutEvent(QFocusEvent *e)
{
  QLineEdit::focusOutEvent(e);
  emit(focussed(false));
}
