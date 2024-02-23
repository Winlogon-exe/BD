#include "logic.h"
#include <QDebug>
#include<QThread>

Logic::Logic(QObject *parent) : QObject(parent)
{
    connect();
    setupModel();
}

//открытие к бд
void Logic::connect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open())
    {
        qDebug() << "Ошибка при открытии базы данных:" << db.lastError().text();

    }
    qDebug() << "База данных успешно открыта.";
}

void Logic::search(QObject* sender)
{
    State state = buttonStateMap[sender]; // Получаем состояние из отправителя сигнала

    switch (state)
    {
    case Next:
        qDebug()<<"next";
        break;

    case Back:
        qDebug()<<"back";
        break;

    case Search:
        qDebug()<<"search";
        break;

    default:
        qDebug()<<"what?";
        break;
    }
}

//запрос к бд
void Logic::setupModel()
{
    model = new QSqlQueryModel();
    QSqlQuery query(db);
    int pageNumber = 1;
    int pageSize = 30;
    int offset = pageNumber * pageSize;

    QString queryString = QString("SELECT * FROM RUvideos LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);
    query.prepare(queryString);

    if (query.exec())
    {
        model->setQuery(query);
        qDebug() << "Запрос к БД выполнен успешно.";
    }
    else
    {
        qDebug() << "Ошибка при выполнении запроса к БД:" << query.lastError().text();
    }
}

//закрытие бд
void Logic::disconnect()
{
    if (db.isOpen())
    {
        db.close();
        qDebug() << "База данных успешно закрыта.";

    }
    qDebug() << "База данных уже закрыта или не была открыта.";
}

QSqlQueryModel *Logic::getModel() const
{
    return model;
}

void Logic::setButtonState(QObject* button, State state)
{
    buttonStateMap[button] = state;
}
