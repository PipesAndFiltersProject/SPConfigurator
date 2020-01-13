#include "mainwindow.h"
#include <QApplication>
#include <qdir.h>

#include <iostream>

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

int main(int argc, char *argv[])
{
   if (argc < 2) {
      std::cout << "Broadcast port number not specified, using port 10000." << std::endl;
   }
   std::unique_ptr<g3::LogWorker> logworker{ g3::LogWorker::createLogWorker() };
   QString logDir = QDir::homePath() + "/StudentPassing/Logs";
   auto defaultHandler = logworker->addDefaultLogger("SPConfigurator", logDir.toStdString());
   g3::initializeLogging(logworker.get());
   LOG(INFO) << "Launching Qt GUI Configurator app for StudentPassing";
   QApplication a(argc, argv);
   MainWindow w;
   w.show();
   return a.exec();
}
