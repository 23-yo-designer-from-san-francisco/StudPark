#ifndef INCLUDE_ORM_UTILS_WRAPPER_HPP_
#define INCLUDE_ORM_UTILS_WRAPPER_HPP_

#include <string>
#include "Wrapper.h"

template<class Value>
int check_existence(SQLWrapper &db, std::string table, const std::string field, Value value) {
    db << "SELECT * FROM " + table +" WHERE " << field + " = '" << value << "';";
    db.exec();
    if (db.count_tuples() > 0) {
        return 0;
    }
    return 1;
}
#endif  // INCLUDE_ORM_UTILS_WRAPPER_HPP_
