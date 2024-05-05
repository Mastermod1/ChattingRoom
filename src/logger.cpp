#include "logger.hpp"

#include <time.h>

#include <cstdio>

FileLogger::FileLogger() { file_.open(LOG_DIR + "log.txt"); }

FileLogger::~FileLogger() { file_.close(); }

void FileLogger::operator<<(const std::string& out)
{
    const time_t t = time(NULL);
    const std::string time = asctime(gmtime(&t));
    file_ << "[" << time.substr(0, time.size() - 1) << " " << name_ << "] " << out << "\n";
    file_.flush();
}
