#include "NodeView.h"

//TODO:
// 0. take Node IP address from received data, save as node info
// 1. receive data, parse from JSON > Package
// 2. get payload, parse from JSON > NodeConfiguration
// 3. add visual data members to class, like x/y coords etc.
// 4. Draw.


void NodeView::setName(const std::string & name) {
   nodeName = name;
}

void NodeView::setAddress(const std::string & addr) {
   nodeAddress = addr;
}

void NodeView::setInputPort(int port) {
   inputPort = port;
}
void NodeView::setConfigPort(int port) {
   configPort = port;
}

void NodeView::setOutputAddrWithPort(const std::string & addr) {
   outputAddressWithPort = addr;
}

const std::string & NodeView::getName() const {
   return nodeName;
}

const std::string & NodeView::getAddress() const {
   return nodeAddress;
}

int NodeView::getInputPort() const {
   return inputPort;
}

void to_json(nlohmann::json & j, const NodeView & node) {
   //TODO: implement
}

void from_json(const nlohmann::json & j, NodeView & node) {
   for (auto& [key, value] : j.items()) {
      if (key == "name") {
         node.setName(value.get<std::string>());
      } else if (key == "input") {
         node.setInputPort(value.get<int>());
      } else if (key == "config") {
         node.setConfigPort(value.get<int>());
      } else if (key == "output") {
         node.setOutputAddrWithPort(value.get<std::string>());
      } else {
         // nada
      }
   }

}

