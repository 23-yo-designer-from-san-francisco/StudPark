#ifndef INCLUDE_MAILING_SENDER_H_
#define INCLUDE_MAILING_SENDER_H_

#include <algorithm>
#include <chrono>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>
#include <string>

#include "SendMail.h"

#define PERIOD 10

class EmailSender {
 public:
    static void Send(SQLWrapper &db, std::vector<SendMail> data, std::vector<std::string> &accountsForMailing);
 private:
    static void threadSendMail(const std::vector<SendMail> &data, const std::string &account, std::vector<int> &mails);
    static size_t payloadSource(char *ptr, size_t size, size_t nmemb, void *userp);
    struct uploadStatus {
        int lines_read;
        std::string subject;
        std::string body;
        std::string account;
    };
};

class Scheduler {
 public:
    Scheduler();
    void Scan();
 private:
    std::vector<std::string> accountsForMailing;
};

#endif  // INCLUDE_MAILING_SENDER_H_
