#ifndef LOGINLOGIC_H
#define LOGINLOGIC_H

#include <QObject>
#include <QWidget>
#include<functional>

enum StateButton
{
    Login,
    Cancel
};

class LoginLogic: public QObject
{
    Q_OBJECT

public:
    LoginLogic();
    void initMap();
    void login();
    void cancel();

public slots:  
    void s_processState(QObject *sender);
    void s_setButtonState(QObject *button, StateButton state);

private:
    std::map<StateButton, std::function<void()>> funcMap;
    std::map<QObject*, StateButton> buttonStateMap;
    StateButton state;
};

#endif // LOGINLOGIC_H
