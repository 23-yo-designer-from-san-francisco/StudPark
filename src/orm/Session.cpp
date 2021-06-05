#include "Session.h"
#include "Wrapper.h"
#include "utils_wrapper.hpp"

void Session::DeleteSession(SQLWrapper &db, const std::string &session) {
    db << "DELETE FROM session WHERE user_session = '" << session << "';";
    db.exec();
}

int Session::AddSession(SQLWrapper &db, int user_id, const std::string &session, time_t time_end) {
    db << "INSERT INTO session (user_id, user_session, time_end) values ('"
       << user_id << "', '"
       << session << "', to_timestamp(" << time_end << ")) returning id;";
    db.exec();

    return db.get_int(0);
}
