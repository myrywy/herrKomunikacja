#include "debuginfo.h"
#include <stdio.h>

DebugInfo* DebugInfo::di=0;

DebugInfo::DebugInfo()
{
    file=fopen("log.txt","a");
}

DebugInfo *DebugInfo::init()
{
    if(di==0){
        di=new DebugInfo();
        qInstallMessageHandler(myMessageOutput);
    }
    return di;
}

DebugInfo *DebugInfo::getDi()
{
    return di;
}

FILE *DebugInfo::getFile()
{
    return file;
}


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(debugInfo().getFile(), "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

DebugInfo &debugInfo()
{
    return *DebugInfo::init();
}
