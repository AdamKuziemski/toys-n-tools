/** local **/
#include "logger.hpp"
/** STL **/
#include <sstream>
#include <chrono>
#include <ctime>
using std::chrono::system_clock;

Logger::Logger()
{
  filename = "LOG_" + timestamp() + ".html";
  file.open(filename.c_str());

  file <<"<!DOCTYPE html>\n<html>\n<head>\n"
       <<"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n"
       <<"  <style media=\"screen\" type=\"text/css\">\n"
       <<"    body { font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", sans-serif; background-color: #D6D6D6; }\n"
       <<"    table, th, td { border: 1px solid black; border-collapse: collapse; padding: 5px; }\n"
       <<"  </style>"
       <<"\n</head>\n<body>";
}

void Logger::addEvent(LogEvent e)
{
  file <<"\n  <p><font color=#"<< adjustColor(e.getColor()) <<">"
       << e.getPrefix() << e.getContent() <<"</font></p>";
}

void Logger::addEvent(std::string s)
{
  addEvent(LogEvent(s));
}

void Logger::addEvent(std::string s, bool b)
{
  std::ostringstream sout;
  sout << s <<" = "<< std::boolalpha << b;
  addEvent(LogEvent(sout.str()));
}

void Logger::addEvent(std::string s, int i)
{
  std::ostringstream sout;
  sout << s <<" = "<< i;
  addEvent(LogEvent(sout.str()));
}

void Logger::addEvent(std::string s, double d)
{
  std::ostringstream sout;
  sout << s <<" = "<< d;
  addEvent(LogEvent(sout.str()));
}

void Logger::separator()
{
  file <<"\n  <hr>";
}

void Logger::close()
{
  file <<"\n</body>\n</html>"<< std::endl;
  file.close();
}

std::string Logger::adjustColor(int color)
{
  std::ostringstream cstr;
  cstr << std::hex << color;
  std::string col = cstr.str();

  while(col.size() < 6) // insert zeros so the final string will look like "0000FF" instead of "FF"
    col.insert(0, "0");

  return col;
}

std::string Logger::timestamp()
{
  // convert time
  time_t now = system_clock::to_time_t(system_clock::now());
  std::ostringstream sout;
  sout << ctime(&now);

  std::string time = sout.str();

  // strip colons
  size_t pos = time.find(":");
  while(pos != std::string::npos)
  {
    time.replace(pos, 1, "_");
    pos = time.find(":");
  }

  // strip spaces
  pos = time.find(" ");
  while(pos != std::string::npos)
  {
    time.replace(pos, 1, "-");
    pos = time.find(" ");
  }

  return time.substr(0, time.size() - 1); // strip '\n' character
}

DumpEvent::DumpEvent(const std::map<std::string, std::string>& dump, std::string title, bool showQuotes)
: LogEvent("[DUMP] " + title + "<br>\n", 0x000000, "")
{
  std::string quote = showQuotes ? "\"" : "";

  content = "    <table>\n      <tr><th><b>Key</b></th><th><b>Value</b></th></tr>\n";

  for(auto it = dump.begin(); it != dump.end(); ++it)
    content += "      <tr><td>" + quote + it->first + quote + "</td><td>" + quote + it->second + quote + "</td></tr>\n";

  content += "    </table>\n  ";
}
