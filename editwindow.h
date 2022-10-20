#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>

namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget
{
  Q_OBJECT

      public:
               explicit EditWindow(QWidget *parent = nullptr);
  ~EditWindow();

private:
  Ui::EditWindow *ui;
};

#endif // EDITWINDOW_H
