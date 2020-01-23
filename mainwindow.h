#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "SPConfiguratorListener.h"

namespace Ui {
class MainWindow;
}
class SPConfigurator;

Q_DECLARE_METATYPE(QString)

class MainWindow : public QMainWindow, public SPConfiguratorListener
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = nullptr);
   ~MainWindow() override;

   virtual void handleIncomingData(std::string data) override;
   virtual void handleError(std::string error) override;
   
private:

   void showMessage(const QString & message);
   
signals:
   void incomingDataSignal(QString data);
   void errorSignal(QString error);
   
private slots:
   void onSearchButtonClicked();
   void doHandleIncomingData(QString data);
   void doHandleError(QString error);
   
private:
   Ui::MainWindow *ui;

   SPConfigurator * configurator;
};

#endif // MAINWINDOW_H
