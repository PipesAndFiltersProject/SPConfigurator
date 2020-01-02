#include <nlohmann/json.hpp>

#include "spconfigurator.h"
#include "SPConfiguratorListener.h"

SPConfigurator::SPConfigurator(SPConfiguratorListener & l, int port, int castPort)
   : listener(l), broadcasterSocket(io_service), listeningSocket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
     broadcastPort(castPort), isRunning(false), isSearching(false) {

}

SPConfigurator::~SPConfigurator() {
   if (isSearching) {
      stopSearchingForNodes();
   }
   if (isRunning) {
      deinit();
   }
}

void SPConfigurator::init() {
   ioServiceThread = std::thread([this] {
      return io_service.run();
   });
   doReceive();
}

void SPConfigurator::deinit() {
   if (!io_service.stopped()) {
      io_service.stop();
      isRunning = false;
      ioServiceThread.join();
   }
}

bool SPConfigurator::isSearchingNodes() const {
   return isSearching;
}

void SPConfigurator::startSearchingForNodes() {
   boost::system::error_code error;

   if (!error && !isSearching)
   {
      broadcasterSocket.open(boost::asio::ip::udp::v4(), error);
      broadcasterSocket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
      broadcasterSocket.set_option(boost::asio::socket_base::broadcast(true));
      isSearching = true;

      searchThread = std::thread([this] {
         try {
            boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), broadcastPort);
            nlohmann::json object;
            searchMessage = object.dump();
            while (isSearching) {
               std::this_thread::sleep_for(std::chrono::milliseconds(3000));
               broadcasterSocket.send_to(boost::asio::buffer(searchMessage), senderEndpoint);
            }
         } catch (const std::exception & e) {
            isSearching = false;
            listener.handleError(e.what());
         }
      });
   }
}

void SPConfigurator::stopSearchingForNodes() {
   if (isSearching) {
      try {
         isSearching = false;
         boost::system::error_code error;
         broadcasterSocket.close(error);
         searchThread.detach();
      } catch (const std::exception & e) {
         listener.handleError(e.what());
      }
   }
}

void SPConfigurator::doReceive() {
   listeningSocket.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                              [this](boost::system::error_code ec, std::size_t bytes_recvd)
                              {
                                 if (!ec && bytes_recvd > 0) {
                                    std::string arrived(data_, bytes_recvd);
                                    listener.handleIncomingData(arrived);
                                 } else {
                                    doReceive();
                                 }
                              });
}
