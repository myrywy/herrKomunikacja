#ifndef TASK_H
#define TASK_H

#include <QThread>

class Task : public QThread
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = 0);
    void runEventLoop();
signals:

public slots:
};

#endif // TASK_H
