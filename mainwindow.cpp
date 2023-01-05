#include "mainwindow.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "./ui_mainwindow.h"

namespace  {

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setCentralWidget(&m_edit_window);

  connect(ui->actionOpen, &QAction::triggered, [this]{
    auto path = QFileDialog::getOpenFileName(this, "Select scb-file", "C:/Users/Mervish/Documents/Xenia/bna", "Idolmaster script file (*.scb)");
    if(path.isEmpty()) { return; }
    m_scb_holder.loadFromFile(path.toStdString());
    //QJsonObject json;
    msg_path = std::filesystem::path(path.toStdString());
    msg_path.replace_extension(".msg");
    m_scb_holder.msg_data().saveToFile(msg_path.string());
    ui->actionSet_json->setEnabled(true);
    ui->actionSave->setEnabled(true);
  });

  connect(ui->actionRebuild_scb_test, &QAction::triggered, [this]{
      auto path = QFileDialog::getOpenFileName(this, "Select scb-file", "C:/Users/Mervish/Documents/Xenia/bna", "Idolmaster script file (*.scb)");
      if(path.isEmpty()) { return; }
      m_scb_holder.loadFromFile(path.toStdString());
      auto filepath = std::filesystem::path(path.toStdString());
      filepath.replace_filename(filepath.stem().string() + "_rebuilt" + filepath.extension().string());
      m_scb_holder.saveToFile(filepath.string());
  });

  connect(ui->actionOpen_msg_debug, &QAction::triggered, [this]{
    auto const path = QFileDialog::getOpenFileName(this, "Select scb-file", "C:/Users/Mervish/Documents/Xenia/bna", "Idolmaster message file (*.msg)");
    if(path.isEmpty()) { return; }
    m_msg_holder.loadFromFile(path.toStdString());
    auto const path_rebuild = path + "_rebuild";
    m_msg_holder.saveToFile(path_rebuild.toStdString());
  });

  connect(ui->actionSet_json, &QAction::triggered, [this]{
    auto path = QFileDialog::getOpenFileName(this, "Select json-file", "C:/Users/Mervish/Documents/Xenia/bna", "json file (*.json)");
    if(path.isEmpty()) { return; }
    QFile json_file(path);
    json_file.open(QFile::ReadOnly);
    auto json = QJsonDocument::fromJson(json_file.readAll());
    m_scb_holder.msg_data().fromJson(json.object());
    auto translated_msg = msg_path;
    translated_msg.replace_filename(translated_msg.stem().string() + "_rebuilt" + translated_msg.extension().string());
    m_scb_holder.msg_data().saveToFile(translated_msg.string());
  });

  connect(ui->actionSave, &QAction::triggered, [this]{
    auto path = QFileDialog::getSaveFileName(this, "Select scb-file", "C:/Users/Mervish/Documents/Xenia/bna", "Idolmaster script file (*.scb)");
    if(path.isEmpty()) { return; }
    m_scb_holder.rebuild();
    m_scb_holder.saveToFile(path.toStdString());
  });
}

MainWindow::~MainWindow()
{
  delete ui;
}

