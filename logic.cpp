#include "logic.h"
#include<QLineEdit>

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

void Logic::processRequest(QObject* sender,const QString &searchText)
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
        searchDataFromDB(searchText);
        break;

    default:
        qDebug()<<"what?";
        break;
    }

    emit updateLabel(currentPage);
    emit updateDB();
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
}

void Logic::searchDataFromDB(const QString &searchText)
{
    // Получение списка полей таблицы
    QSqlQuery fieldQuery(db);
    fieldQuery.exec("PRAGMA table_info(RUvideos);");

    QStringList fields;
    while (fieldQuery.next())
    {
        QString fieldName = fieldQuery.value(1).toString(); // Индекс 1 содержит имя поля
        fields << fieldName;
    }

    // Формирование строки условия поиска
    QString searchCondition;
    for (const auto &field: fields)
    {
        if (!searchCondition.isEmpty())
        {
            searchCondition += " OR ";
        }
        // Приведение каждого поля к тексту и поиск
        searchCondition += QString("CAST(%1 AS TEXT) LIKE '%%2%'").arg(field).arg(searchText);
    }

    // Выполнение поискового запроса
    model = new QSqlQueryModel();
    QString queryString = QString("SELECT * FROM RUvideos");
    if (!searchCondition.isEmpty())
    {
        queryString += " WHERE " + searchCondition;
    }
    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);

    QSqlQuery query(db);
    query.prepare(queryString);

    if (query.exec())
    {
        model->setQuery(std::move(query));
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе: %1").arg(query.lastError().text()));
    }
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
