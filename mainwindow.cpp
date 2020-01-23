#include <g3log/g3log.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SPConfigurator.h"

static const int DEFAULT_PORT = 10'001;

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow), configurator(nullptr)
{
   qRegisterMetaType<QString> ();
   
   LOG(INFO) << "Configurator main window setup starts";
   ui->setupUi(this);
   
   connect(ui->searchButton, SIGNAL(clicked(bool)), this, SLOT(onSearchButtonClicked()));
   connect(this, SIGNAL(incomingDataSignal(QString)), this, SLOT(doHandleIncomingData(QString)), Qt::QueuedConnection);
   connect(this, SIGNAL(errorSignal(QString)), this, SLOT(doHandleError(QString)), Qt::QueuedConnection);

   int broadcastPortNumber = DEFAULT_PORT;
   if (QApplication::arguments().count() > 1) {
      QString portStr = QApplication::arguments().at(1);
      bool success;
      broadcastPortNumber = portStr.toInt(&success);
      if (!success) {
         showMessage("Startup parameter broadcast port was wrong!");
         broadcastPortNumber = DEFAULT_PORT;
      }
   }
   showMessage("Using port " + QString::number(broadcastPortNumber) + " as broadcast port.");
   configurator = new SPConfigurator(*this, broadcastPortNumber);
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
   emit incomingDataSignal(logEntry);
}

void MainWindow::handleError(std::string error) {
   QString logEntry = QString::fromStdString(error);
   emit errorSignal(logEntry);
}


void MainWindow::doHandleIncomingData(QString logEntry) {
   showMessage(logEntry);
   const SPConfigurator::NodeContainer & nodes = configurator->getNodes();
   ui->LogView->clear();
   std::for_each(std::begin(nodes), std::end(nodes), [this](const NodeView & node) {
      std::string description = node.getInputAddressWithPort() + "\t" + node.getName() + "\t" + node.getOutputAddressWithPort();
      QString logEntry = QString::fromStdString(description);
      ui->LogView->appendPlainText(logEntry);
   });
}

void MainWindow::doHandleError(QString error) {
   showMessage(error);
}

void MainWindow::onSearchButtonClicked()
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
      ui->statusbar->showMessage(message);
   }
}
