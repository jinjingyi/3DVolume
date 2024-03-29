#include "Utils.h"

std::string splitAndGetLast(std::string s, const std::string &delimiter) {
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        s.erase(0, pos + delimiter.length());
    }
    return s;
}

std::string splitAndGetPrevious(std::string s, const std::string &delimiter) {
    size_t pos = 0;
    while ((pos = s.find_last_of(delimiter)) != std::string::npos) {
        s.erase(pos, s.length() - pos);
    }
    return s;
}

std::string getFileExtension(const std::string &s) {
    return splitAndGetLast(s, ".");
}

std::string getFileNameNoExtension(const std::string &s) {
    return splitAndGetPrevious(s, ".");
}


std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

std::string getCurrentDate() {
    time_t t = time(nullptr); // current date
    struct tm now {};
#ifdef _WIN32
    localtime_s(&now, &t);
#else
    localtime_r(&t, &now);
#endif
    return std::to_string(now.tm_year + 1900)
        + std::to_string(now.tm_mon + 1)
        + std::to_string(now.tm_mday)
        + std::to_string(now.tm_hour)
        + std::to_string(now.tm_min)
        + std::to_string(now.tm_sec);
}
