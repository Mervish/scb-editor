#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editwindow.h"
#include "scb.h"

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
  Q_OBJECT

      public:
               MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  EditWindow m_edit_window;
  imas::SCB m_scb_holder;
  imas::MSG m_msg_holder;
};
#endif // MAINWINDOW_H
