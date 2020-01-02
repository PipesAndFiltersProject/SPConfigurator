#ifndef SPCONFIGURATOR_H
#define SPCONFIGURATOR_H

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class SPConfiguratorListener;

class SPConfigurator
{
public:
   SPConfigurator(SPConfiguratorListener & l, int port, int castPort);

   ~SPConfigurator();

   void init();
   void deinit();

   void startSearchingForNodes();
   void stopSearchingForNodes();
   bool isSearchingNodes() const;

private:
   void doReceive();

private:
   SPConfiguratorListener & listener;
   boost::asio::io_service io_service;
   std::thread ioServiceThread;
   boost::asio::ip::udp::socket broadcasterSocket;
   boost::asio::ip::udp::socket listeningSocket;
   int broadcastPort;
   std::string searchMessage;
   bool isRunning;
   bool isSearching;
   std::thread searchThread;

   boost::asio::ip::udp::endpoint sender_endpoint_;
   enum { max_length = 4096 };
   char data_[max_length];

};

#endif // SPCONFIGURATOR_H
