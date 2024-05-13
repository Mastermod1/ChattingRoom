#pragma once

#include <filesystem>
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
    explicit FileLogger()
    {
        if (is_initialzied_)
        {
            return;
        }
        is_initialzied_ = true;
        const std::string path = ".";
        const std::string log_file_prefix = "log";
        int file_index = 1;
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (not entry.is_regular_file())
            {
                continue;
            }
            const auto file_name = entry.path().filename().string();
            auto index = file_name.find(log_file_prefix);
            auto extension_index = file_name.find(".txt");
            if (index != std::string::npos)
            {
                auto num = file_name.substr(index + log_file_prefix.size(), extension_index - index - 3);
                file_index = std::stoi(num) + 1;
            }
        }
        const std::string fname = log_file_prefix + std::to_string(file_index) + ".txt";
        file_.open(fname);

        if (file_.is_open())
        {
            file_ << "Log file created";
            file_.flush();
        }
    }

    void flush() { file_.flush(); }

    ~FileLogger()
    {
        if (file_.is_open())
        {
            file_.close();
        }
    }

    friend FileLogger& operator<<(FileLogger& logger, const LogLevel level)
    {
        logger << "\n";
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
        logger.flush();
        return logger;
    }

    friend FileLogger& operator<<(FileLogger& logger, const char* text)
    {
        logger.file_ << text;
        logger.flush();
        return logger;
    }

    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;
    static bool is_initialzied_;

  private:
    std::ofstream file_;
};

static FileLogger logger = FileLogger();

#define LOG_INFO() logger << LogLevel::LOG_INFO << " " << __PRETTY_FUNCTION__ << " "
