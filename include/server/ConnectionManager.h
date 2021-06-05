#ifndef INCLUDE_SERVER_CONNECTIONMANAGER_H_
#define INCLUDE_SERVER_CONNECTIONMANAGER_H_

#include <set>
#include <memory>
#include "boost/noncopyable.hpp"
#include "Connection.h"

class ConnectionManager: private boost::noncopyable {
 public:
    ConnectionManager();

    void start(std::shared_ptr<Connection> connection);  // добавить соединение для pull
    void stop(std::shared_ptr<Connection> connection);   // закрыть соедниние и удалить из pull
    void stopAll();

    size_t connectionsNumber();
 private:
    std::set<std::shared_ptr<Connection>> connections_;
};

#endif  // INCLUDE_SERVER_CONNECTIONMANAGER_H_
