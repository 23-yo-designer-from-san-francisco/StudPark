#include "sender.h"

Scheduler::Scheduler() {
    const int accountsAmount = 8;
    for (int i = 0; i < accountsAmount; ++i) {
        std::string envVariable = "MAIL_USER_" + std::to_string(i + 1);
        accountsForMailing.emplace_back(std::getenv(envVariable.c_str()));
    }
}

int main() {
    Scheduler scheduler;
    SQLWrapper wrapper;
    int testEmailsAmount = 2;
    int secondsCounter = 0;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(PERIOD));
        secondsCounter += PERIOD;
        scheduler.Scan();
        std::cout << secondsCounter << " seconds" << std::endl;
    }

    /*Scheduler sch;
    // Data for testing
    SQLWrapper wrapper;

    int testEmailsAmount = 2;
    for (int i = 0; i < testEmailsAmount; ++i) {
        SendMail::AddInQueue(
                wrapper,
                "studparksender@gmail.com",
                "Заголовок тестового письма",
                "Тело тестового письма",
                time(nullptr)
                );
    }

    sch.Scan();*/
}
