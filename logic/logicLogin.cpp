#include "logicLogin.h"

LoginLogic::LoginLogic(QObject* parent):
    QObject(parent)
{
    initTypes();
}

void LoginLogic::initTypes()
{
    qRegisterMetaType<StateButtonLogin>("StateButtonLogin");
}

void LoginLogic::s_initDB()
{
    if (connectToDatabase())
    {
        qDebug() << "\nИнициализация базы данных";
        initMap();
    }
    else
    {
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка открытия БД");
    }
}

bool LoginLogic::connectToDatabase()
{
    db = DatabaseManager::instance("client.db").database();
    if (!db.isOpen())
    {
        qDebug() << "закрыто";
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
    StateButtonLogin state = buttonStateMap[sender];
    auto it = funcMap.find(state);

    userName = login;
    userPasword = password;

    if (it != funcMap.end())
    {
        it->second();
    }
}

void LoginLogic::s_setButtonState(QObject* button, StateButtonLogin state)
{
    buttonStateMap[button] = state;
}

void LoginLogic::executeRequest(const QString &queryString)
{
    if (!db.isOpen())
    {
        qDebug() << "База данных не открыта.переподключение...";
        if (!connectToDatabase())
        {
            qDebug() << "Не удалось подключиться к базе данных.";
            return;
        }
    }

    QSqlQuery query(db); // Убедитесь, что используете правильное соединение
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
            emit authenticationFailed();
        }
    }
    else
    {
        qDebug() << "Не удалось сделать запрос к БД";
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
    QApplication::quit();
}

void LoginLogic::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        qInfo() << "База данных закрыта. LoginLogic";
    }
    else
    {
        qWarning() << "База данных уже закрыта.";
    }
}

LoginLogic::~LoginLogic()
{
    qDebug()<<"деструктор LoginLogic";
}
