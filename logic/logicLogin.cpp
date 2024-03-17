#include "logicLogin.h"

LoginLogic::LoginLogic(QObject* parent):
    QObject(parent)
{
    qRegisterMetaType<StateButton>("StateButton");
    dbFilename = QDir(QApplication::applicationDirPath()).filePath("test.db");
    initDB();
}

void LoginLogic::initDB()
{
    qDebug() << "\nИнициализация базы данных";
    if (connectToDatabase())
    {
        initMap();
        qDebug() << "succes";
    }
    else
    {
        qDebug() << "error";
    }
}

bool LoginLogic::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open())
    {
        return false;
    }
    return true;
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
