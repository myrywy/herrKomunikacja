#include "task.h"

Task::Task(QObject *parent) : QThread(parent)
{

}

void Task::runEventLoop()
{
    exec();
}
