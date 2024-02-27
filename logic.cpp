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

//открытие бд
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
        qDebug() << "Next";
        break;
    case Back:
        backPage();
        qDebug() << "Back";
        break;
    case Search:
        searchDataFromDB(searchText);
        qDebug() << "Search";
        break;
    default:
         qWarning() << "Неизвестное действие:" << state;
        break;
    }

    //свернуть?
    emit updateLabel(currentPage);
    emit updateDB();
}

//изменение запроса
void Logic::createRequest()
{
    QString queryString = QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2").arg(pageSize).arg(currentPage * pageSize);
    executeRequest(queryString);
}

//добавление запроса в model
void Logic::executeRequest(const QString &queryString)
{
    QSqlQuery query(db);
    query.prepare(queryString);

    if (query.exec())
    {
        model->setQuery(std::move(query));
        qDebug() << "Запрос выполнен";
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе "));
    }
}
//вывод поиска на 1 страницу все или на  пагинацию?
void Logic::searchDataFromDB(const QString &searchText)
{
    if(searchText.isEmpty())
        return;

    //currentPage = 0; // поиск начинается с 0 страницы

    // Получаем список полей для поиска
    QStringList fields = getAllTablesFromDB("popular_tracks");

    // Формируем условие поиска, используя LIKE для каждого поля
    QString searchCondition = createSearchCondition(fields, searchText);

    // Строим запрос с условием поиска, если оно не пустое
    QString queryString = "SELECT * FROM popular_tracks";
    if (!searchCondition.isEmpty())
    {
        queryString += " WHERE " + searchCondition;
    }
    //queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(currentPage * pageSize);

    // Выполняем запрос
    executeRequest(queryString);
}

//получаем все таблицы из бд
QStringList Logic::getAllTablesFromDB(const QString &tableName)
{
    QSqlQuery fieldQuery(db);
    fieldQuery.exec("PRAGMA table_info(popular_tracks);");

    QStringList fields;
    while (fieldQuery.next())
    {
        fields << fieldQuery.value(1).toString();
    }
    return fields;
}

//условие поиска исходя из таблиц
QString Logic::createSearchCondition(const QStringList &fields, const QString &searchText)
{
    QString searchCondition;
    for (const auto &field : fields)
    {
        if (!searchCondition.isEmpty())
            searchCondition += " OR ";

        searchCondition += QString("CAST(%1 AS TEXT) LIKE '%%2%'").arg(field).arg(searchText);
    }
    return searchCondition;
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
