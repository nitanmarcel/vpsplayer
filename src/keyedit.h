#pragma once

#include <QLineEdit>

class KeyEdit : public QLineEdit
{
  Q_OBJECT

public:
  KeyEdit(QWidget *parent = 0);
  ~KeyEdit();
  void setKey(int keyID);

signals:
  void focussed(bool hasFocus);

protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);
};
