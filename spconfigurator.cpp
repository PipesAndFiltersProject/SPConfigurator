#include <sstream>

#include <nlohmann/json.hpp>
#include <g3log/g3log.hpp>

#include <ProcessorNode/Package.h>

#include "SPConfigurator.h"
#include "SPConfiguratorListener.h"

SPConfigurator::SPConfigurator(SPConfiguratorListener & l, int castPort)
   : listener(l), broadcasterSocket(io_service),
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
      nodes.clear();
      searchThread = std::thread([this] {
         try {
            // TODO: create Package with search payload so that uuid changes when new search is started with new uuid.
            // Nodes can then reply to first search package with the uuid, and ignore other identical search package uuids.
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

const std::vector<NodeView> & SPConfigurator::getNodes() const {
   return nodes;
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
   broadcasterSocket.async_receive_from(boost::asio::buffer(data, max_length), sender_endpoint_,
                              [this](boost::system::error_code ec, std::size_t bytes_recvd)
                              {
                                 if (!ec && bytes_recvd > 0) {
                                    std::string arrived(data, bytes_recvd);
                                    LOG(INFO) << "Got data: " << arrived;
                                    handleIncomingConfig(sender_endpoint_.address().to_string(), arrived);
                              }
                              doReceive();
                              });
}

void SPConfigurator::handleIncomingConfig(std::string nodeAddress, std::string configData) {
   try {
      nlohmann::json packageJSON = nlohmann::json::parse(configData);
      OHARBase::Package p = packageJSON.get<OHARBase::Package>();
      LOG(INFO) << "Package arrived";
      if (p.getType() == OHARBase::Package::Configuration) {
         LOG(INFO) << "It was configuration package!";
         std::string payload = p.getPayloadString();
         LOG(INFO) << "Payload: " << payload;
         nlohmann::json payloadJSON = nlohmann::json::parse(payload);
         if (payloadJSON.find("operation") != payloadJSON.end()) {
            std::string operation = payloadJSON["operation"].get<std::string>();
            LOG(INFO) << "Operation was found: " << operation;
            if (operation == "info") {
               nlohmann::json configs = payloadJSON["configitems"];
               NodeView view = configs.get<NodeView>();
               view.setAddress(nodeAddress);
               addOrUpdateNodeView(view);
               std::stringstream sstream;
               sstream << "Node found: " << view.getName() << " at " << view.getAddress() << ":" << view.getInputPort();
               std::string message = sstream.str();
            } else {
               LOG(INFO) << "No info operation, ignoring";
            }
         } else {
            LOG(INFO) << "No operation element in JSON";
         }
      } else {
         LOG(INFO) << "This was not a configuration package, ignored";
      }
   } catch (std::exception e) {
      std::stringstream sstream;
      sstream << "Error in incoming data handling: " << e.what();
      LOG(INFO) << sstream.str();
      listener.handleError(sstream.str());
   }
}

void SPConfigurator::addOrUpdateNodeView(NodeView & view) {
   // Try to find if we already got information about this node.
   NodeContainer::iterator iter = std::find(std::begin(nodes), std::end(nodes), view);
   // Yes, just update it just in case something changed at the remote node.
   if (iter != nodes.end()) {
      *iter = view;
   // Otherwise, it was a new node, so store it.
   } else {
      nodes.push_back(view);
      std::sort(nodes.begin(), nodes.end());
   }
   std::stringstream sstream;
   sstream << "System has " << nodes.size() << " nodes";
   listener.handleIncomingData(sstream.str());
}
