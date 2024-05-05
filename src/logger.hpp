#pragma once

#include <fstream>
#include <memory>
#include <string>

const std::string LOG_DIR = "/home/mastermod1/Projects/ChattingRoom/";

class Logger
{
  public:
    virtual void setFunctionName(const std::string& name) = 0;
    virtual void operator<<(const std::string& out) = 0;
    virtual ~Logger() = default;
};

class FileLogger : public Logger
{
  public:
    FileLogger();
    void setFunctionName(const std::string& name) override { name_ = name; }
    void operator<<(const std::string& out) override;
    ~FileLogger();

  private:
    std::ofstream file_;
    std::string name_ = "";
};

const static std::unique_ptr<Logger> LOGGER = std::make_unique<FileLogger>();

#define LOG_INFO()                                \
    LOGGER->setFunctionName(__PRETTY_FUNCTION__); \
    *LOGGER
