#include "mainwindow.h"
#include <QApplication>
#include <qdir.h>

#include <iostream>

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

int main(int argc, char *argv[])
{
     std::unique_ptr<g3::LogWorker> logworker{ g3::LogWorker::createLogWorker() };
     QString logDir = QDir::homePath() + "/StudentPassing/Logs";
     auto defaultHandler = logworker->addDefaultLogger(argv[0], logDir.toStdString());
     g3::initializeLogging(logworker.get());
     LOG(INFO) << "Launching Qt GUI Configurator app for StudentPassing";
     LOG(INFO) << "Startup parameter is: " << argv[0];
     QApplication a(argc, argv);
     MainWindow w;
     w.show();

     return a.exec();
}
