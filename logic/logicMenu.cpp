#include"logicMenu.h"

LogicMenu::LogicMenu()
{
    initMap();
}

void LogicMenu::initMap()
{
    funcMap[Users]  = [this](){ users(); };
    funcMap[Projects] = [this](){ projects(); };
}

void LogicMenu::s_processState(QObject* sender)
{
    StateButtonMenu state = buttonStateMap[sender];
    auto it = funcMap.find(state);

    if (it != funcMap.end())
    {
        it->second();
    }
}

void LogicMenu::users()
{
    emit openFormUsers();
}

void LogicMenu::projects()
{
    emit openFormProjects();
}

void LogicMenu::s_setButtonState(QObject* button, StateButtonMenu state)
{
    buttonStateMap[button] = state;
}

LogicMenu::~LogicMenu()
{
    qDebug()<<"деструктор LogicMenu";
}
