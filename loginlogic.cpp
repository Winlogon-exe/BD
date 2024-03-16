#include "loginlogic.h"

LoginLogic::LoginLogic()
{
    initMap();
}

void LoginLogic::initMap()
{
    funcMap[Login]  = [this](){ login(); };
    funcMap[Cancel] = [this](){ cancel(); };
}

void LoginLogic::s_processState(QObject* sender)
{
    StateButton state = buttonStateMap[sender];
    auto it = funcMap.find(state);

    if (it != funcMap.end())
    {
        it->second();
    }
}

void LoginLogic::s_setButtonState(QObject* button, StateButton state)
{
    buttonStateMap[button] = state;
}

void LoginLogic::login()
{
    qDebug()<<"login";
}

void LoginLogic::cancel()
{
    qDebug()<<"cancel";
}
