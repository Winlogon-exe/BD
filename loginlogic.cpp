#include "loginlogic.h"

LoginLogic::LoginLogic()
{

}

void LoginLogic::initMap()
{
    funcmap[Login]  = [this](){ login(); };
    funcmap[Cancel] = [this](){ cancel(); };
}
void LoginLogic::setButtonState(QObject* button, StateButton state)
{
    buttonStateMap[button] = state;
}

void LoginLogic::login()
{

}

void LoginLogic::cancel()
{

}
