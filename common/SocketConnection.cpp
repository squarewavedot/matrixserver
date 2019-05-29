#include "SocketConnection.h"

#include <boost/log/trivial.hpp>

SocketConnection::SocketConnection(boost::asio::io_service &io_context) :
        io(io_context), socket(io), cobsDecoder(RECEIVE_BUFFER_SIZE) {
    receiveCallback = NULL;
}

boost::asio::generic::stream_protocol::socket &SocketConnection::getSocket() {
    return socket;
}


void SocketConnection::startReceiving() {
    this->doRead();
}

void SocketConnection::setReceiveCallback(
        std::function<void(std::shared_ptr<UniversalConnection>,
                           std::shared_ptr<matrixserver::MatrixServerMessage>)> callback) {
    receiveCallback = callback;
}

void SocketConnection::doRead() {
    BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Starting Read";
    socket.async_read_some(
            boost::asio::buffer(this->recv_buffer, RECEIVE_BUFFER_SIZE),
            [this](boost::system::error_code error, size_t bytes_transferred) {
                this->handleRead(error, bytes_transferred);
            });
}

void SocketConnection::handleRead(const boost::system::error_code &error, size_t bytes_transferred) {
    BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Handling Read";
    if (!error) {
        BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Received: " << bytes_transferred << " bytes";
        auto packets = cobsDecoder.insertBytesAndReturnDecodedPackets((uint8_t *) this->recv_buffer, bytes_transferred);
        for (auto packet : packets) {
            auto receiveMessage = std::make_shared<matrixserver::MatrixServerMessage>();
            if (receiveMessage->ParseFromString(packet)) {
                BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Recieved full Protobuf MatrixServerMessage";
                if (receiveCallback != NULL) {
                    receiveCallback(shared_from_this(), receiveMessage);
                }
            } else {
                if (message_buffer.size() > RECEIVE_BUFFER_SIZE) {
                    BOOST_LOG_TRIVIAL(debug) << "[SOCK CON] Message Buffer to big, resetting";
                }
            }
        }
        this->doRead();
    } else {
        BOOST_LOG_TRIVIAL(debug) << "[SOCK CON] Read Error: " << error.message();
        dead = true;
    }
}


void SocketConnection::sendMessage(std::shared_ptr<matrixserver::MatrixServerMessage> message) {
    auto sendBuffer = Cobs::encode(message->SerializeAsString());
    BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Starting Write of " << sendBuffer.size() << " bytes - last byte: " << std::hex << (int)sendBuffer.back();

    sendMutex.lock();
    boost::asio::async_write(socket,
                             boost::asio::buffer(sendBuffer.data(), sendBuffer.size()),
                             [this, sendBuffer](boost::system::error_code error, size_t bytes_transferred) {
                                 sendMutex.unlock();
                                 this->handleWrite(error, bytes_transferred, sendBuffer);
                             });
}


std::string getHexArrayString(const uint8_t *data, size_t length) {
    std::stringstream result;
    for (int i = 0; i < length; i++) {
        result << "0x" << std::hex << (int) data[i] << " ";
    }
    return result.str();
}

std::string getHexArrayString(std::string input) {
    return getHexArrayString((const uint8_t *) input.data(), input.size());
}


void SocketConnection::handleWrite(const boost::system::error_code &error, size_t bytes_transferred,
                                const std::string &message_encoded) {
    BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Handling Write";
    if (!error) {
        BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Written: " << bytes_transferred << " bytes from "
                                 << message_encoded.size();
    } else {
        BOOST_LOG_TRIVIAL(debug) << "[SOCK CON] Write Error: " << error.message();
        dead = true;
    }
    BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] written packet end "
                             << getHexArrayString((const uint8_t *) (message_encoded.data()+message_encoded.size()-10), 10);
    for (int i = 0; i < message_encoded.size(); i++) {
        if (message_encoded.at(i) == 0x00) {
            BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Found 0 in packet at " << i;
        }
    }
}

bool SocketConnection::isDead() {
    return dead;
}

SocketConnection::~SocketConnection() {
//    BOOST_LOG_TRIVIAL(trace) << "[SOCK CON] Shutdown socket";
//    this->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}

void SocketConnection::setDead(bool sDead) {
    dead = sDead;
}
