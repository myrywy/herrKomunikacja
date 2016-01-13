#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include <QtMessageHandler>
#include <QByteArray>
#include <QString>
#include <fstream>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void stateInfo(QString str);

class DebugInfo
{
    DebugInfo();
    static DebugInfo* di;
    FILE* file;
    std::ofstream stateFile;
public:
    static DebugInfo* init();
    static DebugInfo* getDi();
    FILE* getFile();
    void addStateInfo(QString str);
};

DebugInfo& debugInfo();

#endif // DEBUGINFO_H
