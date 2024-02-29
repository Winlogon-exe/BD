#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    model(new QSqlQueryModel())

{
    connectToDatabase();
}

void Logic::processState(QObject* sender,const QString &searchText)
{
    State state = buttonStateMap[sender];
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
         qWarning() << "Неизвестное действие:";
        break;
    }

    //свернуть?
    emit updateLabel(currentPage);
    emit updateDB();
}

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

//изменение запроса
void Logic::createRequest()
{
    QString queryString = QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2")
                              .arg(pageSize)
                              .arg(currentPage * pageSize);
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

    currentPage = 0; // поиск начинается с 0 страницы
    // Получаем список полей для поиска
    QStringList fields = getAllFieldsFromTable("popular_tracks");

    // Формируем условие поиска
    QString searchCondition = createSearchCondition(fields, searchText);

    //SELECT * FROM popular_tracks WHERE field1 LIKE '%searchText%' OR field2 LIKE '%searchText%' OR ...
    // Строим запрос с условием поиска
    QString queryString = "SELECT * FROM popular_tracks";
    if (!searchCondition.isEmpty())
    {
        queryString += " WHERE " + searchCondition;
    }

    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(currentPage * pageSize);
    // Выполняем запрос
    executeRequest(queryString);
}

//получаем все поля из бд для поиска
//если бд не будет изменяться тогда лучше один раз достать и сохранить?
QStringList Logic::getAllFieldsFromTable (const QString &tableName)
{
    QSqlQuery fieldQuery(db);
    fieldQuery.exec(QString("PRAGMA table_info(%1)").arg(tableName));

    QStringList fields;
    while (fieldQuery.next())
    {
        fields << fieldQuery.value(1).toString();
    }
    return fields;
}

//условие поиска исходя из полей
QString Logic::createSearchCondition(const QStringList &fields, const QString &searchText)
{
    QStringList conditions;
    for (const auto &field : fields)
    {
        conditions << QString("%1 LIKE '%%2%'").arg(field).arg(searchText);

    }
    return conditions.join(" OR ");
}

void Logic::nextPage()
{
    currentPage++;
    createRequest();
}

void Logic::backPage()
{
    if (currentPage > 0)
    {
        currentPage--;
        createRequest();
    }
}

void Logic::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        qInfo() << "База данных закрыта.";
    }
    else
    {
        qWarning() << "База данных уже закрыта.";
    }
}

QSqlQueryModel *Logic::getModel() const
{
    return model;
}

void Logic::setButtonState(QObject* button, State state)
{
    buttonStateMap[button] = state;
}
