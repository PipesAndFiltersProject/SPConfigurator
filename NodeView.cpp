
#include <utility>
#include <algorithm>

#include <g3log/g3log.hpp>

#include "NodeView.h"

/*

 - N1 n  j
 n N3 s  i
 - N2 r
 s N4 e
 e N5 w
 loppu
 r N6 -
 w N6 -

siirrä ne joilla ei output, loppuun. aseta loppu = viimeinen jolla output
for i = loppu, i >= 1, i--
  for j = i-1, j >= 0, j--
    jos i elementin output == j elementin input?
      siirrä i ennen j
    end jos
  end for
 end for
*/

NodeView::NodeView()
: nodeName("[Unknown]"), nodeAddress(""), inputPort(""), configPort(""), outputAddressWithPort("")
{

}

NodeView::NodeView(const NodeView & v)
: nodeName(v.nodeName), nodeAddress(v.nodeAddress), inputPort(v.inputPort), configPort(v.configPort), outputAddressWithPort(v.outputAddressWithPort)
{
   
}

NodeView::NodeView(NodeView && v)
: nodeName(std::exchange(v.nodeName, "")), nodeAddress(std::exchange(v.nodeAddress,"")), inputPort(std::exchange(v.inputPort, "")), configPort(std::exchange(v.configPort,"")), outputAddressWithPort(std::exchange(v.outputAddressWithPort, ""))
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
      inputPort = std::exchange(v.inputPort, "");
      configPort = std::exchange(v.configPort,"");
      outputAddressWithPort = std::exchange(v.outputAddressWithPort, "");
   }
   return *this;
}

bool NodeView::operator == (const NodeView & v) const {
   return nodeName == v.nodeName;
}

bool NodeView::operator < (const NodeView & v) const {
   // If node has no input, then put at the beginning.
   if (inputPort.length() == 0) {
      return true;
   }
   // If node has no output, it is not smaller to any other node >>> stays in the end.
   if (outputAddressWithPort.length() == 0) {
      return false;
   }
   // Otherwise, see if this node is before the other node, e.g. this ones output
   // is the other one's input
   bool result =  (outputAddressWithPort == v.getInputAddressWithPort());
   
   std::string judgmement;
   if (result) {
      judgmement = " before ";
   } else {
      judgmement = " not before ";
   }
   LOG(INFO) << "Node " << nodeName << " is " << judgmement << " node " << v.getName();
   return result;
}

void NodeView::setName(const std::string & name) {
   nodeName = name;
}

void NodeView::setAddress(const std::string & addr) {
   nodeAddress = addr;
}

void NodeView::setInputPort(const std::string & port) {
   inputPort = port;
}

void NodeView::setConfigPort(const std::string &  port) {
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

const std::string & NodeView::getInputPort() const {
   return inputPort;
}

std::string NodeView::getInputAddressWithPort() const {
   return nodeAddress + ":" + inputPort;
}

const std::string & NodeView::getOutputAddressWithPort() const {
   return outputAddressWithPort;
}

void to_json(nlohmann::json & j, const NodeView & node) {
   //TODO: implement
}

void from_json(const nlohmann::json & j, NodeView & node) {
   for (auto& [index, element] : j.items()) {
      if (element.contains("name")) {
         node.setName(element.value("name", "[Unnamed]"));
      } else if (element.contains("input")) {
         node.setInputPort(element.value("input", ""));
      } else if (element.contains("config")) {
         node.setConfigPort(element.value("config", ""));
      } else if (element.contains("output")) {
         node.setOutputAddrWithPort(element.value("output", ""));
      } else {
         // nada
      }
   }

}

