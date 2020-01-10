#include <nlohmann/json.hpp>
#include <g3log/g3log.hpp>

#include "spconfigurator.h"
#include "SPConfiguratorListener.h"

SPConfigurator::SPConfigurator(SPConfiguratorListener & l, int port, int castPort)
   : listener(l), broadcasterSocket(io_service), listeningSocket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
     broadcastPort(castPort), isRunning(false), isSearching(false) {
   LOG(INFO) << "Created SPConfigurator.";
}

SPConfigurator::~SPConfigurator() {
   LOG(INFO) << "Destroying SPConfigurator.";
   if (isSearching) {
      LOG(INFO) << "Search was ongoing, stopping it.";
      stopSearchingForNodes();
   }
   if (isRunning) {
      LOG(INFO) << "Is running, calling deinit.";
      deinit();
   }
   LOG(INFO) << "Exiting ~SPConfigurator.";
}

void SPConfigurator::init() {
   LOG(INFO) << "SPConfigurator::init";
   isRunning = true;
   ioServiceThread = std::thread([this] {
      return io_service.run();
   });
   doReceive();
   LOG(INFO) << "SPConfigurator::init done";
}

void SPConfigurator::deinit() {
   LOG(INFO) << "SPConfigurator::deinit";
   if (isRunning) {
      LOG(INFO) << "io_service not stopped, stopping...";
      isRunning = false;
      io_service.stop();
      LOG(INFO) << "io_service stopped, waiting for his thread...";
      ioServiceThread.join();
   }
   LOG(INFO) << "SPConfigurator::deinit done";
}

bool SPConfigurator::isSearchingNodes() const {
   return isSearching;
}

void SPConfigurator::startSearchingForNodes() {

   if (!isSearching)
   {
      LOG(INFO) << "Starting to search for nodes using broadcast socket";

      searchThread = std::thread([this] {
         try {
            // TODO: create package with search payload so that uuid changes when new search is started with new uuid.
            const nlohmann::json searchJSON = R"(
                                           { "package" : "123e4567-e89b-12d3-a456-426655440000",
                                             "type" : "configuration",
                                             "payload" : "read"
                                           })"_json;
            boost::system::error_code error;
            isSearching = true;
            broadcasterSocket.open(boost::asio::ip::udp::v4(), error);
            broadcasterSocket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
            broadcasterSocket.set_option(boost::asio::socket_base::broadcast(true));
            boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), broadcastPort);
            searchMessage = searchJSON.dump();
            while (isSearching) {
               LOG(INFO) << "Sending searchmessage: " << searchMessage;
               broadcasterSocket.send_to(boost::asio::buffer(searchMessage), senderEndpoint);
               std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            }
         } catch (const std::exception & e) {
            // isSearching = false;
            LOG(INFO) << "Exception in search thread: " << e.what();
            listener.handleError(e.what());
         }
      });
   } else {
      LOG(INFO) << "Already isSearching, no search started.";
   }
}

void SPConfigurator::stopSearchingForNodes() {
   if (isSearching) {
      try {
         LOG(INFO) << "Stopping searching.";
         isSearching = false;
         boost::system::error_code error;
         broadcasterSocket.close(error);
         searchThread.detach();
         LOG(INFO) << "Searching stopped.";
      } catch (const std::exception & e) {
         listener.handleError(e.what());
      }
   }
}

// TODO: actually call doReceive to receive responses. Store them for each node (by node name), std::map perhaps?
void SPConfigurator::doReceive() {
   broadcasterSocket.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                              [this](boost::system::error_code ec, std::size_t bytes_recvd)
                              {
                                 if (!ec && bytes_recvd > 0) {
                                    std::string arrived(data_, bytes_recvd);
                                    LOG(INFO) << "Got data: " << arrived;
                                    listener.handleIncomingData(arrived);
                              }
                              doReceive();
                              });
}
