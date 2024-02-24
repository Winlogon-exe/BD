#include "logic.h"

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
    loadDataFromDB();
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
        searchDataFromDB();
        break;

    default:
        qDebug()<<"what?";
        break;
    }

    emit updateLabel(currentPage);
}

//запрос к бд
//рефактор
void Logic::loadDataFromDB()
{
    model = new QSqlQueryModel();
    QSqlQuery query(db);
    QString queryString = QString("SELECT * FROM RUvideos LIMIT %1 OFFSET %2")
                              .arg(pageSize)
                              .arg(offset);

    query.prepare(queryString);

    if (query.exec())
    {
        model->setQuery(std::move(query));
        qDebug()<<"Запрос";
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе "));
    }
     emit updateDB();
}

void Logic::searchDataFromDB()
{

}

void Logic::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        QMessageBox::information(nullptr, QObject::tr("Ошибка"), QObject::tr("БД закрыта"));
    }
    QMessageBox::information(nullptr, QObject::tr("Ошибка"), QObject::tr("БД уже закрыта"));
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
    loadDataFromDB(); // новый запрос
    qDebug() << "next";
}

void Logic::back()
{
    if (currentPage > 0)
    {
        currentPage--;
        updateOffset(); // Обновляем смещение для предыдущей страницы
        loadDataFromDB(); // новый запрос
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
