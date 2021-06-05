cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(studpark CXX)
set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

add_compile_options(-O3 -march=native -mtune=native -Wno-deprecated-declarations)

include_directories(
	include/templates
	include/mailing
	${PostgreSQL_INCLUDE_DIRS}
	include/orm
	include/server
	include/api
	/usr/local/boost_1_76_0
	/usr/local/include/inja
)

link_directories(
	/usr/local/boost_1_76_0/stage/lib
)

add_executable(studpark
	${PROJECT_SOURCE_DIR}/main.cpp
	src/api/GroupAPI.cpp
	src/api/PostAPI.cpp
	src/api/UserAPI.cpp
	src/api/Utils.cpp
	src/orm/Comment.cpp
	src/orm/Contact.cpp
	src/orm/Deadline.cpp
	src/orm/File.cpp
	src/orm/Group.cpp
	src/orm/Post.cpp
	src/orm/SendMail.cpp
	src/orm/Session.cpp
	src/orm/Share.cpp
	src/orm/Student.cpp
	src/orm/Subject.cpp
	src/orm/Wrapper.cpp
	src/server/Connection.cpp
	src/server/ConnectionManager.cpp
	src/server/Request.cpp
	src/server/Response.cpp
	src/server/server.cpp
	src/server/Transportation.cpp
	src/templates/template_engine.cpp
)

add_executable(sender
	src/mailing/sender_daemon.cpp
	src/mailing/sender.cpp
	src/orm/SendMail.cpp
	src/orm/Wrapper.cpp
)

target_link_libraries(studpark
	PostgreSQL::PostgreSQL
	pthread
	boost_system
	boost_locale
	OpenSSL::Crypto
	curl
	gcov
)

target_link_libraries(sender
	PostgreSQL::PostgreSQL
	curl
	pthread
	gcov
)
