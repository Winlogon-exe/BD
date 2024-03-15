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
    void login();
    void cancel();

public slots:
    void initMap();
    void setButtonState(QObject *button, StateButton state);

private:
    std::map<StateButton, std::function<void()>> funcmap;
    std::map<QObject*, StateButton> buttonStateMap;
    StateButton state;
};

#endif // LOGINLOGIC_H
