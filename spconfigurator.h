#pragma once

#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "NodeView.h"

class SPConfiguratorListener;

/**
 A class to send configuration read request messages and handling the responses from
 ProcessorNodes. See project readme for JSON message documentation and additional information.
 */
class SPConfigurator
{
public:
   SPConfigurator(SPConfiguratorListener & l, int broadcastPort);

   ~SPConfigurator();

   void init();
   void deinit();

   void startSearchingForNodes();
   void stopSearchingForNodes();
   bool isSearchingNodes() const;

private:
   void doReceive();

   void handleIncomingConfig(std::string nodeAddress, std::string configData);
   
private:
   SPConfiguratorListener & listener;
   boost::asio::io_service io_service;
   std::thread ioServiceThread;
   boost::asio::ip::udp::socket broadcasterSocket;
   int broadcastPort;
   std::string searchMessage;
   bool isRunning;
   bool isSearching;
   std::thread searchThread;

   boost::asio::ip::udp::endpoint sender_endpoint_;
   enum { max_length = 4096 };
   char data_[max_length];

   std:vector<NodeView> nodes;
};

