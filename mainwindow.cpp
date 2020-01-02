#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spconfigurator.h"

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow), configurator(nullptr)
{
   ui->setupUi(this);
   configurator = new SPConfigurator(*this, 10000, 10001);
   configurator->init();
}

MainWindow::~MainWindow()
{
   configurator->deinit();
   delete configurator;
   delete ui;
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
      configurator->stopSearchingForNodes();
   } else {
      ui->LogView->appendPlainText("Started searching for nodes");
      configurator->startSearchingForNodes();
   }
}
