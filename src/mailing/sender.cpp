#include "sender.h"

void Scheduler::Scan() {
    std::vector<SendMail> data;
    SQLWrapper wrapper;
    data = SendMail::NeedSend(wrapper);
    if (!data.empty()) {
        EmailSender::Send(wrapper, data, accountsForMailing);
    }
}

size_t EmailSender::payloadSource(char *ptr, size_t size, size_t nmemb, void *userp) {
    auto *uploadCtx = (struct uploadStatus *)userp;
    const std::string subject_str = "Subject: " + uploadCtx->subject + "\r\n";
    const std::string account_str = "From: Studpark <" + uploadCtx->account + ">\r\n";
    const std::string body_str = uploadCtx->body + "\r\n";
    const char *payloadText[] = {
            account_str.c_str(),
            subject_str.c_str(),
            "\r\n",
            body_str.c_str(),
            nullptr
    };
    if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }
    const char *data = payloadText[uploadCtx->lines_read];
    if (data) {
        strcpy(ptr, data);
        uploadCtx->lines_read++;
        return strlen(data);
    }
    return 0;
}

void EmailSender::threadSendMail(const std::vector<SendMail> &data, const std::string &account, std::vector<int> &mails) {
    CURL *curl;
    CURLcode result;
    struct uploadStatus uploadCtx;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_USERNAME, account.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, std::getenv("MAIL_PASSWORD"));
    curl_easy_setopt(curl, CURLOPT_URL, std::getenv("MAIL_SERVER"));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    if (curl) {
        for (auto &vector : data) {
            struct curl_slist *recipients = nullptr;
            uploadCtx.subject = vector.head_mail;
            uploadCtx.body = vector.body_mail;
            uploadCtx.account = account;
            uploadCtx.lines_read = 0;
            recipients = curl_slist_append(recipients, vector.recipient.c_str());
            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
            curl_easy_setopt(curl, CURLOPT_READDATA, &uploadCtx);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            result = curl_easy_perform(curl);
            if (result != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(result));
            curl_slist_free_all(recipients);
            mails.push_back(vector.id);
            std::cout << "Email with id = " << vector.id << " successfully sent OK" << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

void EmailSender::Send(SQLWrapper &db, std::vector<SendMail> data, std::vector<std::string> &accountsForMailing) {
    const auto procsCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    size_t length = data.size() / procsCount;
    size_t remain = data.size() % procsCount;
    size_t begin = 0;
    size_t end = 0;
    std::vector<int> mailsIdForDelete;

    for (size_t i = 0; i < std::min<unsigned int>(procsCount, data.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        std::vector<SendMail> threadData = std::vector<SendMail>(data.begin() + begin, data.begin() + end);
        begin = end;
        std::string account = accountsForMailing[i];
        threads.emplace_back(
                std::thread(threadSendMail, threadData, account, std::ref(mailsIdForDelete)));
    }
    for (auto & thread : threads) {
        thread.join();
    }

    for (int i : mailsIdForDelete) {
        SendMail::DeleteFromQueue(db, i);
    }
}
