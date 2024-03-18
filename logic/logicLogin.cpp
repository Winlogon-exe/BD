#include "logicLogin.h"

LoginLogic::LoginLogic(QObject* parent):
    QObject(parent)
{
    qRegisterMetaType<StateButton>("StateButton");
    dbFilename = QDir(QApplication::applicationDirPath()).filePath("client.db");
    initDB();
}

void LoginLogic::initDB()
{
    qDebug() << "\nИнициализация базы данных";
    if (connectToDatabase())
    {
        initMap();
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

void LoginLogic::s_processState(QObject* sender,const QString &login,const QString &password)
{
    StateButton state = buttonStateMap[sender];
    auto it = funcMap.find(state);

    userName = login;
    userPasword = password;

    if (it != funcMap.end())
    {
        it->second();
    }
}

void LoginLogic::s_setButtonState(QObject* button, StateButton state)
{
    buttonStateMap[button] = state;
}

void LoginLogic::executeRequest(const QString &queryString)
{
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":username", userName);
    query.bindValue(":password", userPasword);

    if (query.exec())
    {
        if (query.next())
        {
            QString role = query.value(0).toString();
            qDebug() << "Аутентификация успешна. Роль пользователя:" << userName <<role;
            emit authenticationSuccess();
        }
        else
        {
            qDebug() << "Аутентификация не удалась. Пользователь с такими данными не найден.";
            QMessageBox::information(nullptr, "Ошибка", "Пользователь не найден.");
            //emit authenticationFailed();
        }
    }
}

QString LoginLogic::buildQueryString()
{
    QString queryString = "SELECT role FROM users WHERE username = :username AND password = :password";
    return queryString;
}

void LoginLogic::login()
{
    executeRequest(buildQueryString());
}

void LoginLogic::cancel()
{
    qDebug()<<"cancel";
    QApplication::quit();
}
