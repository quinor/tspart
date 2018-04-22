#include "utils/Logger.hh"
#include <map>
#include <string>

namespace {

std::map<Logger::Level, const char*> msg_headers;
std::map<std::string, const char*> msg_strings;

int init()
{
  msg_headers[Logger::Level::Fatal] =   "\033[1;30m[ Fatal    ]\033[0m";
  msg_headers[Logger::Level::Error] =   "\033[1;31m[ Error    ]\033[0m";
  msg_headers[Logger::Level::Warning] = "\033[1;33m[ Warning  ]\033[0m";
  msg_headers[Logger::Level::Info] =    "\033[1;32m[ Info     ]\033[0m";
  msg_headers[Logger::Level::Verbose] = "\033[1;34m[ Verbose  ]\033[0m";
  msg_headers[Logger::Level::Debug] =   "\033[1;37m[ Debug    ]\033[0m";

  msg_strings["enter"] = "\033[1;36m[ Entering ]\033[0m";
  msg_strings["exit"]  = "\033[1;35m[ Exiting  ]\033[0m";
  msg_strings["empty"] =           "            ";
  return 0;
}
const int _ = init();

}


Logger::Logger(FILE* out)
: loglevel(Logger::Level::Info)
, output(out)
{}

void Logger::set_log_level(Logger::Level level)
{
  loglevel = level;
}

void Logger::print_logline(const char* hdr, const char* string, const char* msg, char line_end='\n')
{
  fprintf(
    output,
    "[% 9.3f ] %s %s %s%s%c",
    time.getElapsedTime().asSeconds(),
    hdr,
    string,
    std::string(4*stack.size(), ' ').c_str(),
    msg,
    line_end
  );
}

void Logger::enter_impl(Logger::Level level, const char* block)
{
  if (level <= loglevel)
    print_logline(msg_headers[level], msg_strings["enter"], block);
  stack.push_back({level, block});
}

void Logger::exit()
{
  if (stack.size() == 0)
    throw "Too deep exits!";
  auto top = stack.back();
  if (top.first <= loglevel)
    print_logline(msg_headers[top.first], msg_strings["exit"], top.second.c_str());
  stack.pop_back();
}

void Logger::log_str(Logger::Level level, const char* message)
{
  if (level <= loglevel)
    print_logline(msg_headers[level], msg_strings["empty"], message);
}

Logger::LogStream Logger::log(Logger::Level level)
{
  return LogStream(this, level);
}

Logger::Progress Logger::progress(Logger::Level level, const char* desc, int max)
{
  return Logger::Progress(level, desc, max, this);
}

Logger::Progress::Progress (Logger::Level level_, const char* desc_, int max_, Logger* parent_)
: level(level_)
, desc(desc_)
, max(max_)
, parent(parent_)
{
  int d = 1;
  int power = 1;
  while (max >= power)
  {
    d++;
    power*=10;
  }
  digits = d;
}

void Logger::Progress::update(int amount)
{
  if (level > parent->loglevel)
    return;
  sprintf(
    buffer,
    "%s: [\033[1;36m% *d\033[0m / \033[1;34m%d\033[0m ]",
    desc,
    digits,
    amount,
    max
    );
  parent->print_logline(msg_headers[level], msg_strings["empty"], buffer);
}

void Logger::Progress::finish()
{
  if (level > parent->loglevel)
    return;
}

Logger::LogStream::LogStream(Logger* parent_, Logger::Level level_)
: stream(new std::ostringstream)
, parent(parent_)
, level(level_)
, copied(false)
{}

Logger::LogStream::LogStream(Logger::LogStream&& other)
: stream(other.stream)
, parent(other.parent)
, level(other.level)
, copied(true)
{
  other.stream = nullptr;
}

Logger::LogStream::~LogStream()
{
  if (!copied)
    parent->log(level, stream->str().c_str());
  delete stream;
}

Logger::EnterGuard::EnterGuard(Logger* log, Logger::Level level, const char* block)
: parent(log)
{
  parent->enter(level, block);
}

Logger::EnterGuard::~EnterGuard()
{
  parent->exit();
}


Logger& get_logger()
{
  static Logger _ = Logger();
  return _;
}
