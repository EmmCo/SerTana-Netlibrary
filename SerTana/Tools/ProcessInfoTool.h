#ifndef SerTanaPROCESSINFOTOOL_H
#define SerTanaPROCESSINFOTOOL_H



#include <SerTana/Tools/StringPiece.h>
#include <SerTana/Tools/Types.h>
#include <SerTana/Tools/TimestampTool.h>
#include <vector>
#include <sys/types.h>

namespace SerTana
{

namespace ProcessInfoTool
{
  pid_t pid();
  string pidString();
  uid_t uid();
  string username();
  uid_t euid();
  TimestampTool startTime();
  int clockTicksPerSecond();
  int pageSize();
  bool isDebugBuild();  // constexpr

  string hostname();
  string procname();
  StringPiece procname(const string& stat);

  /// read /proc/self/status
  string procStatus();

  /// read /proc/self/stat
  string procStat();

  /// read /proc/self/task/tid/stat
  string threadStat();

  /// readlink /proc/self/exe
  string exePath();

  int openedFiles();
  int maxOpenFiles();

  struct CpuTime
  {
    double userSeconds;
    double systemSeconds;

    CpuTime() : userSeconds(0.0), systemSeconds(0.0) { }
  };
  CpuTime cpuTime();

  int numThreads();
  std::vector<pid_t> threads();
}

}



#endif // SerTanaPROCESSINFOTOOL_H
