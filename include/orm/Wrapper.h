#ifndef INCLUDE_ORM_WRAPPER_H_
#define INCLUDE_ORM_WRAPPER_H_

#include <iostream>
#include <postgresql/libpq-fe.h>
#include <vector>
#include <sstream>
#include <zconf.h>
#include <string>

class SQLWrapper {
 public:
    SQLWrapper() : os(""), result(nullptr) {
        std::string conn_info;
        std::string host = std::string(std::getenv("HOST"));
        conn_info = "host=" + host;
        conn_info = conn_info + " port=" + std::string(std::getenv("PORT"));
        conn_info = conn_info + " dbname=" + std::string(std::getenv("DBNAME"));
        conn_info = conn_info + " user=" + std::string(std::getenv("DBUSER"));
        conn_info = conn_info + " password=" + std::string(std::getenv("PASSWORD"));
        conn = PQconnectdb(conn_info.c_str());
    }

    SQLWrapper(SQLWrapper const &wrapper) {
        conn = wrapper.conn;
    }

    ~SQLWrapper() {close(PQsocket(conn));}

    template<class Value>
    std::ostream &operator<<(const Value &value) {
        os << value;
        return os;
    }

    void clear();
    void disconnect();

    bool exec();
    int count_tuples();
    char *get_str(int field_num, int tup_num = 0);
    int get_int(int field_num, int tup_num = 0);
    bool get_bool(int field_num, int tup_num = 0);
    time_t get_time_t(int field_num, int tup_num = 0);
    void update_semester();

 private:
    PGconn *conn;
    std::ostringstream os;
    PGresult *result;
};

#endif  // INCLUDE_ORM_WRAPPER_H_
