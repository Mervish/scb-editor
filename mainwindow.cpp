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
    QJsonObject json;
    auto filepath = std::filesystem::path(path.toStdString());
    json["name"] = QString::fromStdString(filepath.filename().string());
    json["msg"] = m_scb_holder.msg_data().toJson();
    filepath.replace_extension(".json");
    QFile json_file(filepath);
    json_file.open(QFile::WriteOnly);
    json_file.write(QJsonDocument(json).toJson());
    ui->actionSet_json->setEnabled(true);
    ui->actionSave->setEnabled(true);
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
    m_msg_holder.fromJson(json.object());
  });

  connect(ui->actionSet_json, &QAction::triggered, [this]{
    auto path = QFileDialog::getSaveFileName(this, "Select scb-file", "C:/Users/Mervish/Documents/Xenia/bna", "Idolmaster script file (*.scb)");
    if(path.isEmpty()) { return; }
    //m_scb_holder.saveToFile(path.toStdString());
  });

}

MainWindow::~MainWindow()
{
  delete ui;
}

