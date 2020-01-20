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
   bool operator < (const NodeView & v) const;
   
   virtual ~NodeView() = default;
   
   void setName(const std::string & name);
   void setAddress(const std::string & addr);
   void setInputPort(const std::string & port);
   void setConfigPort(const std::string & port);
   void setOutputAddrWithPort(const std::string & addr);
   
   const std::string & getName() const;
   const std::string & getAddress() const;
   const std::string & getInputPort() const;
   const std::string & getOutputAddressWithPort() const;
   
   std::string getInputAddressWithPort() const;

private:
   std::string nodeName;
   std::string nodeAddress;
   std::string inputPort;
   std::string configPort;
   std::string outputAddressWithPort;
};

void to_json(nlohmann::json & j, const NodeView & node);
void from_json(const nlohmann::json & j, NodeView & node);
