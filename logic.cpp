#include "logic.h"
#include <QDebug>
#include<QThread>

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30)
{
    connectToDatabase();
}

//открытие к бд
void Logic::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open())
    {
        qDebug() << "Ошибка при открытии базы данных:" << db.lastError().text();

    }
    qDebug() << "База данных успешно открыта.";
    setupModel();
}

void Logic::search(QObject* sender)
{
    State state = buttonStateMap[sender]; // Получаем состояние из отправителя сигнала
    //рефактор
    switch (state)
    {
    case Next:
        next();
        break;
    case Back:
        back();
        break;
    case Search:
        qDebug()<<"search";
        break;

    default:
        qDebug()<<"what?";
        break;
    }
    emit updateDB();
    emit updateLabel(currentPage);
}

//запрос к бд
void Logic::setupModel()
{
    model = new QSqlQueryModel();
    QSqlQuery query(db);
    QString queryString = QString("SELECT * FROM RUvideos LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);
    query.prepare(queryString);

    if (query.exec())
    {
        model->setQuery(query);
    }
    else
    {
        qDebug() << "Ошибка при выполнении запроса к БД:" << query.lastError().text();
    }
}

void Logic::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        qDebug() << "База данных  закрыта.";

    }
    qDebug() << "База данных уже закрыта или не была открыта.";
}

//закрытие бд
void Logic::updateOffset()
{
   offset = currentPage * pageSize; //  смещение
}

void Logic::next()
{
    currentPage++;
    updateOffset();
    setupModel(); // новый запрос
    qDebug() << "next";
}

void Logic::back()
{
    if (currentPage > 0)
    {
        currentPage--;
        updateOffset(); // Обновляем смещение для предыдущей страницы
        setupModel(); // новый запрос
    }
    qDebug()<<"back";
}


QSqlQueryModel *Logic::getModel() const
{
    return model;
}

void Logic::setButtonState(QObject* button, State state)
{
    buttonStateMap[button] = state;
}
