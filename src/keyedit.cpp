#include "keyedit.h"

KeyEdit::KeyEdit(QWidget *parent)
 : QLineEdit(parent)
{
    setReadOnly(true);
}

KeyEdit::~KeyEdit()
{}

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
