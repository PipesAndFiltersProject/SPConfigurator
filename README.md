# StudentPassing Configurator

This app enables remote configuration of the [StudentPassing](https://bitbucket.org/anttijuu/studentpassing)  app Nodes. 

It may work also with other apps build on top of the [ProcessorNode](https://bitbucket.org/anttijuu/processornode) library, but haven't yet been tested with any.

This is **work in progress**. 

Currently the Configurator is able to send configuration read requests to the Nodes and handles the responses from the Nodes. Handling just means that the existing Nodes are displayed as a list.

Nodes need to be configured to receive and respond to configuration messages.

What remains to be implemented:

1. Display a graphical view of the Nodes,
2. Enable editing the configuration of the nodes, including order of the Nodes in an installation,
3. Sending the changed configurations to the Nodes, and then Nodes reconfiguring themselves runtime.

## Dependencies

| Component | Version        | Purpose |
| --------------|---------------|-----------|
| [Boost](https://boost.org)          | 1.70.0+        | Networking (Boost::asio) |
| [g3logger](https://github.com/KjellKod/g3log)     | 1.3+             | Logging actions |
| [nlohmann::json](https://github.com/nlohmann/json) | 3.2+       | For parsing and creating JSON from/to objects when sending/receiving configuration messages. |
| [Qt](https://www.qt.io) Widgets, Core and Gui |    | Implementation of the app GUI |
| [ProcessorNode](https://bitbucket.org/anttijuu/processornode)  | Current | For configuration of the Nodes |

## Configuration messages

For documentation on the JSON configuration messages, see [ProcessorNode](https://bitbucket.org/anttijuu/processornode)  documentation.


## Who do I talk to? ##

Repo is owned and maintained by Antti Juustila (antti.juustila at oulu.fi)

## License ##

(c) Antti Juustila 2014-2020. This software is licensed in [GPL version 2 or later](https://opensource.org/licenses/gpl-2.0.php).



