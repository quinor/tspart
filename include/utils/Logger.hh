#pragma once

#include <cstdio>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <SFML/System.hpp>


class Logger
{
public:
  enum class Level
  {
    None = 0,
    Fatal = 1,
    Error = 2,
    Warning = 3,
    Info = 10,
    Verbose = 20,
    Debug = 30,
    Max = 100
  };

  class Progress
  {
    Level level;
    const char* desc;
    int max;
    Logger* parent;
    int digits;
    char buffer[128];

    friend class Logger;
    Progress(Level level, const char* desc, int max, Logger* parent);
  public:
    void update (int amount);
    void finish ();
  };

  class LogStream
  {
    std::ostringstream *stream;
    Logger* parent;
    Level level;
    bool copied;
  public:
    LogStream(Logger* log, Level level);
    LogStream(LogStream&& other);
    ~LogStream();
    template <typename Type>
    std::ostringstream& operator<<(Type x)
    {
      (*stream) << x;
      return (*stream);
    }
  };

  class EnterGuard
  {
    Logger* parent;
  public:
    EnterGuard(Logger* log, Level level, const char* block);
    ~EnterGuard();
  };

private:
  Level loglevel;
  std::vector<std::pair<Level, std::string> > stack;
  FILE* output;
  sf::Clock time;

  void print_logline(const char* hdr, const char* string, const char* msg, char line_end);
  void enter_impl (Level level, const char* block);
  void log_str (Level level, const char* message);
public:
  Logger(FILE* output=stderr);
  void set_log_level (Level level);
  template<typename... Ts>
  EnterGuard enter_guard (Level level, Ts&&... args);
  template<typename... Ts>
  void enter (Level level, Ts&&... args);
  void exit ();
  LogStream log (Level level);
  template<typename... Ts>
  void log (Level level, Ts&&... args);
  Progress progress(Level level, const char* desc, int max);
};


Logger& get_logger(); //singleton

template<typename... Ts>
std::string print(Ts&&... args)
{
    std::ostringstream os;
    (os << ... << args);
    return os.str();
}

template<typename... Ts>
void Logger::log (Level level, Ts&&... args)
{
  log_str(level, print(args...).c_str());
}

template<typename... Ts>
Logger::EnterGuard Logger::enter_guard  (Level level, Ts&&... args)
{
  return EnterGuard(this, level, print(args...).c_str());
}

template<typename... Ts>
void Logger::enter (Level level, Ts&&... args)
{
  enter_impl(level, print(args...).c_str());
}
