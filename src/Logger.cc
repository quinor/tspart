#include "Logger.hh"
#include <map>
#include <string>

namespace {

std::map<Logger::Level, const char*> msg_headers;
std::map<std::string, const char*> msg_strings;

int init()
{
  msg_headers[Logger::Level::Error] =   "\033[1;31m[ Error    ]\033[0m";
  msg_headers[Logger::Level::Warning] = "\033[1;33m[ Warning  ]\033[0m";
  msg_headers[Logger::Level::Info] =    "\033[1;32m[ Info     ]\033[0m";
  msg_headers[Logger::Level::Verbose] = "\033[1;34m[ Verbose  ]\033[0m";
  msg_headers[Logger::Level::Debug] =   "\033[1;37m[ Debug    ]\033[0m";

  msg_strings["enter"] = "\033[1;36m[ Entering ]\033[0m";
  msg_strings["exit"]  = "\033[1;36m[ Exiting  ]\033[0m";
  msg_strings["empty"] = "\033[1;36m            \033[0m";
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

void Logger::print(const char* hdr, const char* string, const char* msg, char line_end='\n')
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

void Logger::enter(Logger::Level level, const char* block)
{
  if (level <= loglevel)
    print(msg_headers[level], msg_strings["enter"], block);
  stack.push_back({level, block});
}

void Logger::exit()
{
  auto top = stack.back();
  stack.pop_back();
  if (top.first <= loglevel)
    print(msg_headers[top.first], msg_strings["exit"], top.second.c_str());
}

void Logger::log(Logger::Level level, const char* message)
{
  if (level <= loglevel)
    print(msg_headers[level], msg_strings["empty"], message);
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
  parent->print(msg_headers[level], msg_strings["empty"], buffer);
}
void Logger::Progress::finish()
{
  if (level > parent->loglevel)
    return;
//  parent->print(msg_headers[level], msg_strings["empty"], "");
}

Logger& get_logger()
{
  static Logger _ = Logger();
  return _;
}
