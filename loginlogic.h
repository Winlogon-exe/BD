#ifndef LOGINLOGIC_H
#define LOGINLOGIC_H

#include <QObject>
#include <QWidget>

enum State
{
    Login,
    Cancel,
};

class LoginLogic
{
    Q_OBJECT
public:
    LoginLogic();
};

#endif // LOGINLOGIC_H
