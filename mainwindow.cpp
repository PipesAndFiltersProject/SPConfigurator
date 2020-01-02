#include <g3log/g3log.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spconfigurator.h"

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow), configurator(nullptr)
{
   LOG(INFO) << "Configurator main window setup starts";
   ui->setupUi(this);
   configurator = new SPConfigurator(*this, 10000, 10001);
   configurator->init();
   LOG(INFO) << "Configurator main window created";
}

MainWindow::~MainWindow()
{
   LOG(INFO) << "Configurator main window deninit commencing";
   configurator->deinit();
   LOG(INFO) << "Deleting configurator and ui objects";
   delete configurator;
   delete ui;
   LOG(INFO) << "MainWindow deleted";
}

void MainWindow::handleIncomingData(std::string data) {
   QString logEntry = QString::fromStdString(data);
   ui->LogView->appendPlainText(logEntry);
}

void MainWindow::handleError(std::string error) {
   QString logEntry = QString::fromStdString(error);
   ui->LogView->appendPlainText(logEntry);
}


void MainWindow::on_searchButton_clicked()
{
   if (configurator->isSearchingNodes()) {
      ui->LogView->appendPlainText("Stopped searching for nodes");
      LOG(INFO) << "Stopping node search broadcasting.";
      configurator->stopSearchingForNodes();
      ui->searchButton->setText("&Start searching");
   } else {
      ui->LogView->appendPlainText("Started searching for nodes");
      ui->searchButton->setText("&Stop searching");
      LOG(INFO) << "Starting node search broadcasting.";
      configurator->startSearchingForNodes();
   }
}
