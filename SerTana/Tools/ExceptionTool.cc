

#include <SerTana/Tools/ExceptionTool.h>

//#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

using namespace SerTana;

ExceptionTool::ExceptionTool(const char* msg)
  : message_(msg)
{
  fillStackTrace();
}

ExceptionTool::ExceptionTool(const string& msg)
  : message_(msg)
{
  fillStackTrace();
}

ExceptionTool::~ExceptionTool() throw ()
{
}

const char* ExceptionTool::what() const throw()
{
  return message_.c_str();
}

const char* ExceptionTool::stackTrace() const throw()
{
  return stack_.c_str();
}

void ExceptionTool::fillStackTrace()
{
  const int len = 200;
  void* buffer[len];
  int nptrs = ::backtrace(buffer, len);
  char** strings = ::backtrace_symbols(buffer, nptrs);
  if (strings)
  {
    for (int i = 0; i < nptrs; ++i)
    {
      // TODO demangle funcion name with abi::__cxa_demangle
      stack_.append(strings[i]);
      stack_.push_back('\n');
    }
    free(strings);
  }
}


