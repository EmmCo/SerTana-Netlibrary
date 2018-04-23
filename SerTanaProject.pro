#-------------------------------------------------
#
# Project created by QtCreator 2018-04-05T13:31:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = SerTanaProject
CONFIG   += console
CONFIG   -= app_bundle


TEMPLATE = app


SOURCES += \
    SerTana/Tools/FileUtilTool.cc \
    SerTana/Tools/LogFileTool.cc \
    SerTana/Tools/LogStreamTool.cc \
    SerTana/Tools/ThreadPoolTool.cc \
    SerTana/Tools/ThreadTool.cc \
    SerTana/Tools/TimeZoneTool.cc \
    SerTana/Tools/ExceptionTool.cc \
    SerTana/Tools/DateTool.cc \
    SerTana/Tools/CountDownLatchTool.cc \
    SerTana/Tools/ConditionTool.cc \
    SerTana/Tools/AsyncLoggingTool.cc \
    SerTana/Tools/TimestampTool.cc \
    SerTana/Tools/ProcessinfoTool.cc \
    SerTana/Tools/LoggingTool.cc \
    SerTana/net/Acceptor.cc \
    SerTana/net/boilerplate.cc \
    SerTana/net/Buffer.cc \
    SerTana/net/Channel.cc \
    SerTana/net/Connector.cc \
    SerTana/net/EventLoop.cc \
    SerTana/net/EventLoopThread.cc \
    SerTana/net/EventLoopThreadPool.cc \
    SerTana/net/InetAddress.cc \
    SerTana/net/Poller.cc \
    SerTana/net/Socket.cc \
    SerTana/net/SocketsOps.cc \
    SerTana/net/TcpClient.cc \
    SerTana/net/TcpConnection.cc \
    SerTana/net/TcpServer.cc \
    SerTana/net/Timer.cc \
    SerTana/net/TimerQueue.cc \
    SerTana/net/poller/DefaultPoller.cc \
    SerTana/net/poller/EPollPoller.cc \
    SerTana/net/poller/PollPoller.cc \
    SerTana/net/http/HttpContext.cc \
    SerTana/net/http/HttpResponse.cc \
    SerTana/net/http/HttpServer.cc \
    SerTana/net/P2P/P2PClient.cc \
    SerTana/net/P2P/P2PServer.cc \
    main.cc

HEADERS += \
    SerTana/Tools/ConditionTool.h \
    SerTana/Tools/CountDownLatchTool.h \
    SerTana/Tools/CurrentThread.h \
    SerTana/Tools/Mutex.h \
    SerTana/Tools/ThreadTool.h \
    SerTana/Tools/TimeZoneTool.h \
    SerTana/Tools/Types.h \
    SerTana/Tools/DateTool.h \
    SerTana/Tools/LogStreamTool.h \
    SerTana/Tools/StringPiece.h \
    SerTana/Tools/Atomic.h \
    SerTana/Tools/Singleton.h \
    SerTana/Tools/ExceptionTool.h \
    SerTana/Tools/LogFileTool.h \
    SerTana/Tools/FileUtilTool.h \
    SerTana/Tools/AsyncLoggingTool.h \
    SerTana/Tools/ThreadPoolTool.h \
    SerTana/Tools/BlockingQueue.h \
    SerTana/Tools/BoundedBlockingQueue.h \
    SerTana/Tools/WeakCallback.h \
    SerTana/Tools/ThreadLocal.h \
    SerTana/Tools/ThreadLocalSingleton.h \
    SerTana/Tools/copyable.h \
    SerTana/Tools/LoggingTool.h \
    SerTana/Tools/ProcessInfoTool.h \
    SerTana/Tools/TimestampTool.h \
    SerTana/net/Acceptor.h \
    SerTana/net/boilerplate.h \
    SerTana/net/Buffer.h \
    SerTana/net/Callbacks.h \
    SerTana/net/Channel.h \
    SerTana/net/Connector.h \
    SerTana/net/Endian.h \
    SerTana/net/EventLoop.h \
    SerTana/net/EventLoopThread.h \
    SerTana/net/EventLoopThreadPool.h \
    SerTana/net/InetAddress.h \
    SerTana/net/Poller.h \
    SerTana/net/Socket.h \
    SerTana/net/SocketsOps.h \
    SerTana/net/TcpClient.h \
    SerTana/net/TcpConnection.h \
    SerTana/net/TcpServer.h \
    SerTana/net/Timer.h \
    SerTana/net/TimerId.h \
    SerTana/net/TimerQueue.h \
    SerTana/net/ZlibStream.h \
    SerTana/net/poller/EPollPoller.h \
    SerTana/net/poller/PollPoller.h \
    SerTana/net/http/HttpContext.h \
    SerTana/net/http/HttpRequest.h \
    SerTana/net/http/HttpResponse.h \
    SerTana/net/http/HttpServer.h \
    SerTana/net/P2P/P2PServer.h \
    SerTana/net/P2P/P2PTools.h \
    SerTana/net/P2P/P2PClient.h

OTHER_FILES += \
    SerTana/net/premake4.lua \
    SerTana/net/CMakeLists.txt \
    SerTana/net/http/CMakeLists.txt
