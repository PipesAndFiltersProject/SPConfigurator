#include <g3log/g3log.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SPConfigurator.h"

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow), configurator(nullptr)
{
   LOG(INFO) << "Configurator main window setup starts";
   ui->setupUi(this);
   int broadcastPortNumber = 10000;
   if (QApplication::arguments().count() > 1) {
      QString portStr = QApplication::arguments().at(1);
      bool success;
      broadcastPortNumber = portStr.toInt(&success);
      if (!success) {
         showMessage("Startup parameter broadcast port was wrong!");
         broadcastPortNumber = 10000;
      }
   }
   showMessage("Using port " + QString::number(broadcastPortNumber) + " as broadcast port.");
   configurator = new SPConfigurator(*this, 10001);
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

void MainWindow::handleIncomingData(std::string message) {
   QString logEntry = QString::fromStdString(message);
   ui->LogView->appendPlainText(logEntry);
}

void MainWindow::handleError(std::string error) {
   QString logEntry = QString::fromStdString(error);
   ui->LogView->appendPlainText(logEntry);
}


void MainWindow::on_searchButton_clicked()
{
   if (configurator->isSearchingNodes()) {
      showMessage("Stopped searching for nodes");
      LOG(INFO) << "Stopping node search broadcasting.";
      configurator->stopSearchingForNodes();
      ui->searchButton->setText("&Start searching");
   } else {
      showMessage("Started searching for nodes");
      ui->searchButton->setText("&Stop searching");
      LOG(INFO) << "Starting node search broadcasting.";
      configurator->startSearchingForNodes();
   }
}

void MainWindow::showMessage(const QString & message)
{
   // TODO: examine also event (notification, warning, error) and display each differently.
   if (message.length() > 0) {
      ui->LogView->appendPlainText(message);
   }
}
