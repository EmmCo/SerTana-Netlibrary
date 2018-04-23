

#ifndef SerTana_EXCEPTION_H
#define SerTana_EXCEPTION_H

#include <SerTana/Tools/Types.h>
#include <exception>

namespace SerTana
{

class ExceptionTool : public std::exception
{
 public:
  explicit ExceptionTool(const char* what);
  explicit ExceptionTool(const string& what);
  virtual ~ExceptionTool() throw();
  virtual const char* what() const throw();
  const char* stackTrace() const throw();

 private:
  void fillStackTrace();

  string message_;
  string stack_;
};

}




#endif // SerTana_EXCEPTION_H
