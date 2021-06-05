#ifndef INCLUDE_SERVER_TRANSPORTATION_H_
#define INCLUDE_SERVER_TRANSPORTATION_H_

#include <string>
#include <iostream>
#include "Response.h"
#include "Request.h"

#include "GroupAPI.h"
#include "PostAPI.h"
#include "UserAPI.h"
#include "Wrapper.h"

class Transportation {
 public:
    Transportation(SQLWrapper &db) : sqlWrapper(db), userApi(db), groupApi(db), postApi(db) {}
    void userTransportation(Request request, Response& response);
 private:
    SQLWrapper &sqlWrapper;
    UserAPI userApi;
    GroupAPI groupApi;
    PostAPI postApi;
};

#endif  // INCLUDE_SERVER_TRANSPORTATION_H_
