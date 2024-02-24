#include "logic.h"
#include<QLineEdit>

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    model(new QSqlQueryModel())

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
        qCritical() << "Ошибка при открытии базы данных:" << db.lastError().text();
        return;
    }
    qInfo() << "База данных успешно открыта.";
    createRequest();
}

void Logic::processState(QObject* sender,const QString &searchText)
{
    State state = buttonStateMap[sender]; // Получаем состояние из отправителя сигнала
    switch (state)
    {
    case Next:
        nextPage();
        break;
    case Back:
        backPage();
        break;
    case Search:
        searchDataFromDB(searchText);
        break;

    default:
         qWarning() << "Неизвестное действие:" << state;
        break;
    }

    //свернуть?
    emit updateLabel(currentPage);
    emit updateDB();
}

//добавление запроса в model
void Logic::executeRequest(const QString &queryString)
{
    QSqlQuery query(db);
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

//формирование запроса
void Logic::createRequest()
{
    QString queryString = QString("SELECT * FROM RUvideos LIMIT %1 OFFSET %2").arg(pageSize).arg(currentPage * pageSize);
    executeRequest(queryString);
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

void Logic::nextPage()
{
    currentPage++;
    createRequest();
    qDebug() << "Следующая страница";
}

void Logic::backPage()
{
    if (currentPage > 0)
    {
        currentPage--;
        createRequest();
        qDebug() << "Предыдущая страница";
    }
}

void Logic::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        qInfo() << "База данных закрыта.";
    }
    qWarning() << "База данных уже закрыта.";
}

QSqlQueryModel *Logic::getModel() const
{
    return model;
}

void Logic::setButtonState(QObject* button, State state)
{
    buttonStateMap[button] = state;
}
