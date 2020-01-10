#ifndef SPCONFIGURATORLISTENER_H
#define SPCONFIGURATORLISTENER_H

#include <string>

class SPConfiguratorListener {
public:
   virtual ~SPConfiguratorListener() { /*empty*/ }
   virtual void handleIncomingData(std::string message) = 0;
   virtual void handleError(std::string error) = 0;
};

#endif // SPCONFIGURATORLISTENER_H
