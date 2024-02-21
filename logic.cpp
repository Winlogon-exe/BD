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

//запрос к бд
void Logic::setupModel()
{
    model = new QSqlQueryModel();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM products"); // Замените "вашаТаблица" на имя вашей таблицы
    if (query.exec())
    {
        model->setQuery(query);
        qDebug() << "Запрос к БД выполнен успешно.";
    }
    else
    {
        qDebug() << "Ошибка при выполнении запроса к БД:" << query.lastError().text();
    }

   emit dataChanged(model); // Испускаем сигнал с новой моделью
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
