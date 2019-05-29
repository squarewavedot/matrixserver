#include "UnixSocketClient.h"
#include <boost/log/trivial.hpp>

std::shared_ptr<SocketConnection>
UnixSocketClient::connect(boost::asio::io_service &io, std::string socketFile) {
    auto sockConnection = std::make_shared<SocketConnection>(io);
    boost::asio::local::stream_protocol::endpoint unix_endpoint{socketFile};
    try {
        sockConnection->getSocket().connect(unix_endpoint);
        if (sockConnection->getSocket().is_open()) {
            BOOST_LOG_TRIVIAL(debug) << "[UnixSocketClient] Connect successful to path: "
                                     << unix_endpoint.path();
            sockConnection->startReceiving();
        }
    } catch (boost::system::system_error e) {
        BOOST_LOG_TRIVIAL(debug) << "[UnixSocketClient] " << e.what();
        sockConnection->setDead(true);
    }
    return sockConnection;
}
