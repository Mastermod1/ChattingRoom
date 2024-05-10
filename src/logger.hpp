#pragma once

#include <fstream>
#include <string>

const std::string LOG_DIR = "/home/mastermod1/Projects/ChattingRoom/";

enum class LogLevel
{
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO
};

class FileLogger
{
  public:
    explicit FileLogger(const char* fname = "log.txt")
    {
        file_.open(fname);

        if (file_.is_open())
        {
            file_ << "Log file created\n";
        }
    }

    ~FileLogger()
    {
        if (file_.is_open())
        {
            file_.close();
        }
    }

    friend FileLogger& operator<<(FileLogger& logger, const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::LOG_ERROR:
                logger.file_ << "[ERROR]: ";
                break;

            case LogLevel::LOG_WARNING:
                logger.file_ << "[WARNING]: ";
                break;

            default:
                logger.file_ << "[INFO]: ";
                break;
        }
        return logger;
    }

    friend FileLogger& operator<<(FileLogger& logger, const char* text)
    {
        logger.file_ << text << std::endl;
        return logger;
    }

    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

  private:
    std::ofstream file_;
};

static FileLogger logger = FileLogger();

#define LOG_INFO() logger << LogLevel::LOG_INFO << " " << __PRETTY_FUNCTION__

// LOGGER->setFunctionName(__PRETTY_FUNCTION__);
