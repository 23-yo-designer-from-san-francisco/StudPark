#ifndef INCLUDE_SERVER_RESPONSE_H_
#define INCLUDE_SERVER_RESPONSE_H_

#include <unordered_map>
#include <vector>
#include <string>

#define HTTP_VERSION "HTTP/1.1"

namespace status {
    enum statusCode {
        Continue = 100,
        SwitchingProtocol = 101,
        Processing = 102,
        EarlyHints = 103,
        OK = 200,
        Created = 201,
        Accepted = 202,
        NonAuthoritativeInformation = 203,
        NoContent = 204,
        ResetContent = 205,
        PartialContent = 206,
        MultipleChoice = 300,
        MovedPermanently = 301,
        Found = 302,
        SeeOther = 303,
        NotModified = 304,
        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        RequestTimeout = 408,
        InternalServerError = 500,
        NotImplemented = 501,
        BadGateway = 502
    };
}  // namespace status

class Response {
 public:
    explicit Response(const int &status = status::OK);
    explicit Response(const std::string & html, const int &status = status::OK);
    Response& operator=(Response&& other);
    Response(const Response& other);

    std::string str();
    void setCookie(const std::string &key, const std::string &value);
    void setHeader(const std::string &key, const std::string &value);
    void setStatus(const int &status);
    void redirect(const std::string &url) {
        setStatus(301);
        setHeader("Location", url);
    }

 private:
    void setDate();
    std::string statusToStr() const;
    void startLineToStream(std::stringstream & ss);
    void headersToStream(std::stringstream & ss);
    void cookiesToStream(std::stringstream & ss);

    std::unordered_multimap<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> cookies;
    std::string body;
    int statusCode;
};

#endif  // INCLUDE_SERVER_RESPONSE_H_
