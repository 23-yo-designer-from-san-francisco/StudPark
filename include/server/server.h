#ifndef INCLUDE_SERVER_SERVER_H_
#define INCLUDE_SERVER_SERVER_H_

#include <string>
#include <memory>
#include "boost/asio.hpp"
#include "ConnectionManager.h"
#include "Wrapper.h"

namespace async = boost::asio;
namespace net = async::ip;

class Server: private boost::asio::noncopyable {
 public:
    Server(const std::string& addr, const std::string& port, SQLWrapper &db);
    void startServer();
    void stopServer();

 private:
    void accept(const boost::system::error_code& error);
    void stop();

    ConnectionManager manager_;

    async::io_service service_;
    net::tcp::acceptor acceptor_;

    std::shared_ptr<Connection> connection_;
    SQLWrapper db_;
};

#endif  // INCLUDE_SERVER_SERVER_H_
