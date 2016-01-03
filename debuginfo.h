#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QtMessageHandler>
#include <QByteArray>
#include <QString>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);


class DebugInfo
{
    DebugInfo();
    static DebugInfo* di;
    FILE* file;
public:
    static DebugInfo* init();
    static DebugInfo* getDi();
    FILE* getFile();
};

DebugInfo& debugInfo();

#endif // DEBUGINFO_H
