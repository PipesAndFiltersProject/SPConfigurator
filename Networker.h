//
//  Networker.h
//  PipesAndFiltersFramework
//
//  Created by Antti Juustila on 19.9.2013.
//  Copyright (c) 2013 Antti Juustila. All rights reserved.
//

#ifndef PipesAndFiltersFramework_Networker_h
#define PipesAndFiltersFramework_Networker_h

#include <string>
#include <thread>

namespace ohar_pipes {
	
	
	/** An abstract base class for implementing networking in the architecture.
	 This class contains the common features needed in sending and receiving of
	 data packages to/from other ProcessorNodes in the architecture.
	 Both sending and receiving of data happens in a separate thread. Thus, the
	 networking objects need to initialize and configure the threads and the networking.
	 The initialization happens in the constructors (where the addresses are set) and in
	 start() method.
	 @author Antti Juustila
	 @version $Revision $
	 @todo Update to using std lib networking classes, as well as IPv6.
	 */
	class Networker {
	public:
		Networker(const std::string & hostName);
		Networker(const std::string & hostName, int portNumber);
		virtual ~Networker();
		
		void setHost(const std::string & hostName);
		void setPort(int p);
		const std::string getHost() const;
		int getPort() const;
		
		/**
		 Starts the writing or reading networker, depending on the subclass implementation.
		 Note that here this is an abstract method.
		 */
		virtual void start() = 0;
		/**
		 Stops the writing or reading networker, depending on the subclass implementation.
		 Note that here this is an abstract method.
		 */
		virtual void stop() = 0;
		
		bool isRunning();
		
	private:
		Networker() = delete;
		Networker(const Networker &) = delete;
		const Networker & operator =(const Networker &) = delete;
		
	protected:
		/** Host name of the networker. If this is an object receiving data in the
		 ProcessorNode, this is the local IP address of the machine. If this is a sending
		 address, it is the IP address of the remote computer to send data to. */
		std::string host;
		/** Port number where data is read from (local machine, receiving data) or send to. */
		int port;
		
		/** If true, the object is running:<br />
		 <ul><li>NetworkReader is waiting for incoming packages from the network and
		 handling them as they arrive.</li>
		 <li>NetworkWriter is waiting for outbound packages and writing (sending) them to the remote
		 address (other ProcessorNode).</li>
		 </ul>
		 */
		bool running;
		/** The thread object within the context of which all networking code is executed.*/
		std::thread threader;
	};
	
	
} //namespace
#endif
