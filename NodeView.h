#pragma once

#include <string>
#include <map>

#include <nlohmann/json.hpp>


/**
 NodeView represents the data needed to visualize ProcessorNodes to screen for
 configuration management (visualization, modification).
 @author Antti Juustila
 */
class NodeView {
public:
   NodeView() = default;
   NodeView(const NodeView &) = default;
   NodeView & operator =(const NodeView &) = default;
   virtual ~NodeView() = default;
   
   
   void setName(const std::string & name);
   void setAddress(const std::string & addr);
   void setInputPort(int port);
   void setConfigPort(int port);
   void setOutputAddrWithPort(const std::string & addr);
   
private:
   std::string nodeName;
   std::string nodeAddress;
   int         inputPort;
   int         configPort;
   std::string outputAddressWithPort;
   NodeView * nextNode = nullptr;
};

void to_json(nlohmann::json & j, const NodeView & node);
void from_json(const nlohmann::json & j, NodeView & node);
