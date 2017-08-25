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
    Error = 1,
    Warning = 2,
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

private:
  Level loglevel;
  std::vector<std::pair<Level, std::string> > stack;
  FILE* output;
  sf::Clock time;

  void print(const char* hdr, const char* string, const char* msg, char line_end);
public:
  Logger(FILE* output=stderr);
  void set_log_level (Level level);
  void enter (Level level, const char* block);
  void exit ();
  LogStream log (Level level);
  void log_str (Level level, const char* message);
  Progress progress(Level level, const char* desc, int max);
};

Logger& get_logger(); //singleton
