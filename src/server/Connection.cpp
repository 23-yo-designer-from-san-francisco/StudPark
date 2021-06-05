#include "Connection.h"
#include "ConnectionManager.h"

namespace async = boost::asio;
namespace net = boost::asio::ip;

Connection::Connection(boost::asio::io_service& service, ConnectionManager& manager, SQLWrapper &db):
        socket_(service), manager_(manager), transportation_(db), buffer_() {}

void Connection::start() {
    socket_.async_read_some(
            boost::asio::buffer(buffer_),
            boost::bind(&Connection::doRead, shared_from_this(),
                        async::placeholders::error,
                        async::placeholders::bytes_transferred));
}

void Connection::doRead(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        Request request_(std::string(buffer_.begin(), buffer_.end()));

        Response response_;  // default 200
        if (request_.header("Host") != "studpark.ru") {
            response_.setStatus(status::BadRequest);  // not our host
        }
        transportation_.userTransportation(request_, response_);
        async::async_write(
                socket_,
                async::buffer(response_.str(), response_.str().max_size()),
                boost::bind(&Connection::doWrite, shared_from_this(), async::placeholders::error));
    } else if (error != async::error::operation_aborted) {
        manager_.stop(shared_from_this());
    }
}

void Connection::doWrite(const boost::system::error_code &e) {
    if (!e) {
        boost::system::error_code ignored_ec;
        socket_.shutdown(net::tcp::socket::shutdown_both, ignored_ec);
    }
    if (e != async::error::operation_aborted) {
        manager_.stop(shared_from_this());
    }
}
