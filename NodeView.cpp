
#include <utility>

#include <g3log/g3log.hpp>

#include "NodeView.h"

NodeView::NodeView()
: nodeName("[Unknown]"), nodeAddress("[Unknown]"), inputPort(0), configPort(0), outputAddressWithPort("[Unknown]")
{

}

NodeView::NodeView(const NodeView & v)
: nodeName(v.nodeName), nodeAddress(v.nodeAddress), inputPort(v.inputPort), configPort(v.configPort), outputAddressWithPort(v.outputAddressWithPort)
{
   
}

NodeView::NodeView(NodeView && v)
: nodeName(std::exchange(v.nodeName, "")), nodeAddress(std::exchange(v.nodeAddress,"")), inputPort(std::exchange(v.inputPort, 0)), configPort(std::exchange(v.configPort,0)), outputAddressWithPort(std::exchange(v.outputAddressWithPort, ""))
{
}

NodeView & NodeView::operator =(const NodeView & v) {
   if (this != &v) {
      nodeName = v.nodeName;
      nodeAddress = v.nodeAddress;
      inputPort = v.inputPort;
      configPort = v.configPort;
      outputAddressWithPort = v.outputAddressWithPort;
   }
   return *this;
}

NodeView & NodeView::operator =(NodeView && v) {
   if (this != &v) {
      nodeName = std::exchange(v.nodeName, "");
      nodeAddress = std::exchange(v.nodeAddress,"");
      inputPort = std::exchange(v.inputPort, 0);
      configPort = std::exchange(v.configPort,0);
      outputAddressWithPort = std::exchange(v.outputAddressWithPort, "");
   }
   return *this;
}

bool NodeView::operator == (const NodeView & v) const {
   return nodeName == v.nodeName;
}

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
   for (auto& [index, element] : j.items()) {
      if (element.contains("name")) {
         node.setName(element.value("name", "[Unnamed]"));
      } else if (element.contains("input")) {
         std::string port = element.value("input", "0");
         node.setInputPort(std::stoi(port));
      } else if (element.contains("config")) {
         node.setConfigPort(std::stoi(element.value("config", "0")));
      } else if (element.contains("output")) {
         node.setOutputAddrWithPort(element.value("output", "[Unknown]"));
      } else {
         // nada
      }
   }

}

