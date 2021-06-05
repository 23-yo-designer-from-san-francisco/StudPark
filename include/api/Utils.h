#ifndef INCLUDE_API_UTILS_H_
#define INCLUDE_API_UTILS_H_

#include <string>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>

#define MAX_RANDOM 10000

std::string sha256(const std::string &str);
std::string randomString(size_t length);

#endif  // INCLUDE_API_UTILS_H_
