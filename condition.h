#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <functional>

class Condition : public QObject
{
    Q_OBJECT
public:
    explicit Condition(QObject *parent = 0);
    bool check();
    std::function<bool ()> getCheckFunction() const;
    void setCheckFunction(const std::function<bool ()> &value);

protected:
    std::function<bool()>checkFunction;
};

//#include <QList>
//typedef QList<Condition> Situation;

#endif // CONDITION_H
