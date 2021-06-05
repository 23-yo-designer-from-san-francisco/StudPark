#include "Wrapper.h"
#include <iostream>
#include <cstring>

std::string strError(PGconn *conn, PGresult *result, const char *exec) {
    std::ostringstream error;
    error << std::endl << "EXEC:   " << exec << std::endl
          << PQerrorMessage(conn) << std::endl
          << "STATUS: " << PQresultStatus(result) << std::endl;
    return error.str();
}

bool SQLWrapper::exec() {
    std::string query = os.str();
    clear();
    result = PQexec(conn, query.c_str());
    if (PQresultStatus(result) != PGRES_COMMAND_OK && PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cout << strError(conn, result, query.c_str()) << std::endl;
        disconnect();
        throw std::runtime_error("db error");
    }
    return true;
}

void SQLWrapper::disconnect() {
    close(PQsocket(conn));
}

void SQLWrapper::clear() {
    os.str("");
}

char *SQLWrapper::get_str(int field_num, int tup_num) {
    return PQgetvalue(result, tup_num, field_num);
}

int SQLWrapper::get_int(int field_num, int tup_num) {
    return std::stoi(PQgetvalue(result, tup_num, field_num));
}

bool SQLWrapper::get_bool(int field_num, int tup_num) {
    return PQgetvalue(result, tup_num, field_num)[0] == 't';
}

time_t SQLWrapper::get_time_t(int field_num, int tup_num) {
    auto res = PQgetvalue(result, tup_num, field_num);
    struct tm tm;
    strptime(res, "%Y-%m-%d %H:%M:%S", &tm);
    tm.tm_isdst = 0;
    return mktime(&tm);
}

int SQLWrapper::count_tuples() {
    return PQntuples(result);
}

void SQLWrapper::update_semester() {
//    std::string query_clear = "UPDATE \"group\" SET semester = semester + 1 WHERE semester < 10; SELECT setval('deadline_id_seq', 1, false) FROM public.deadline; SELECT setval('subject_id_seq', 1, false) FROM public.subject; DELETE FROM deadline; DELETE FROM subject;";
    std::string query_clear = "UPDATE \"group\" SET semester = semester + 1 WHERE semester < 10; DELETE FROM deadline; DELETE FROM subject;";
    result = PQexec(conn, query_clear.c_str());
    if (PQresultStatus(result) != PGRES_COMMAND_OK && PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cout << strError(conn, result, query_clear.c_str()) << std::endl;
        disconnect();
        throw std::runtime_error("db error");
    }
}
