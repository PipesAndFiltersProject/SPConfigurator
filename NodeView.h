#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace OHARBase { class NodeConfiguration; }

/**
 NodeView represents the data needed to visualize ProcessorNodes to screen for
 configuration management (visualization, modification).
 @author Antti Juustila
 */
class NodeView {
public:
   
private:
   OHARBase::NodeConfiguration * configuration;
};
