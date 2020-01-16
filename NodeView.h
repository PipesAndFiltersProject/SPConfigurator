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
   NodeView();
   NodeView(const NodeView & v);
   NodeView(NodeView && v);
   NodeView & operator = (const NodeView & v);
   NodeView & operator = (NodeView && v);
   bool operator == (const NodeView & v) const;
   virtual ~NodeView() = default;
   
   
   void setName(const std::string & name);
   void setAddress(const std::string & addr);
   void setInputPort(int port);
   void setConfigPort(int port);
   void setOutputAddrWithPort(const std::string & addr);
   
   const std::string & getName() const;
   const std::string & getAddress() const;
   int getInputPort() const;
   
private:
   std::string nodeName;
   std::string nodeAddress;
   int         inputPort;
   int         configPort;
   std::string outputAddressWithPort;
};

void to_json(nlohmann::json & j, const NodeView & node);
void from_json(const nlohmann::json & j, NodeView & node);
