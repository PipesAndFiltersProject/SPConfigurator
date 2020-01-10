#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "spconfigurator.h"
#include "SPConfiguratorListener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SPConfiguratorListener
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = nullptr);
   ~MainWindow() override;

   virtual void handleIncomingData(std::string data) override;
   virtual void handleError(std::string error) override;

private slots:
   void on_searchButton_clicked();

private:
   Ui::MainWindow *ui;

   SPConfigurator * configurator;
};

#endif // MAINWINDOW_H