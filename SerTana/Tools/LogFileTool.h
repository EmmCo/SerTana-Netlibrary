#ifndef SerTana_LOGFILETOOL_H
#define SerTana_LOGFILETOOL_H



#include <SerTana/Tools/Mutex.h>
#include <SerTana/Tools/Types.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace SerTana
{

namespace FileUtil
{
class AppendFile;
}

class LogFileTool : boost::noncopyable
{
 public:
  LogFileTool(const string& basename,
          off_t rollSize,
          bool threadSafe = true,
          int flushInterval = 3,
          int checkEveryN = 1024);
  ~LogFileTool();

  void append(const char* logline, int len);
  void flush();
  bool rollFile();

 private:
  void append_unlocked(const char* logline, int len);

  static string getLogFileName(const string& basename, time_t* now);

  const string basename_;
  const off_t rollSize_;
  const int flushInterval_;
  const int checkEveryN_;

  int count_;

  boost::scoped_ptr<MutexLock> mutex_;
  time_t startOfPeriod_;
  time_t lastRoll_;
  time_t lastFlush_;
  boost::scoped_ptr<FileUtil::AppendFile> file_;

  const static int kRollPerSeconds_ = 60*60*24;
};

}



#endif // SerTana_LOGFILETOOL_H
