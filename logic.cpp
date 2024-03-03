#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    model(new QStandardItemModel ())

{
    initThread();
    initMap();
    connectToDatabase();
}

void Logic::initMap()
{
    funcmap[Next]    = [this](){nextPage();};
    funcmap[Back]    = [this](){backPage();};
    funcmap[Search]  = [this](){searchDataFromDB();};
}

void Logic::initThread()
{
    this->moveToThread(&workerThread);
    workerThread.start();
}

//сделать bool
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

    createRequest(currentPage);
}

void Logic::processState(QObject* sender,const QString &search)
{
    State state = buttonStateMap[sender];
    auto it = funcmap.find(state);
    searchText = search;

    if (it != funcmap.end())
    {
        it->second();
    }

    emit updateLabel(currentPage);
}

//тут нужно извлечь данные и обновить model;
void Logic::executeRequest()
{
    model->clear();
    for (const QVariantMap &rowMap : cachedData)
    {
        QList<QStandardItem *> items;
        for (auto it = rowMap.constBegin(); it != rowMap.constEnd(); ++it)
        {
            items.append(new QStandardItem(it.value().toString()));
        }
        model->appendRow(items);
    }
}

//тут нужно выполнить запрос и добавить данные в контенейр
void Logic::addData(const QString &queryString)
{
    QSqlQuery query(db);
    query.prepare(queryString);

    // Выполняем запрос
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе: ") + query.lastError().text());
        return;
    }

    // Очищаем предыдущие данные
    cachedData.clear();

    // Загружаем данные в контейнер
    while (query.next())
    {
        QVariantMap row;
        for (int i = 0; i < query.record().count(); ++i)
        {
            row[query.record().fieldName(i)] = query.value(i);
        }
        cachedData.append(row);
    }
    executeRequest();
}

void Logic::createRequest(int page)
{
    QString queryString = QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2")
                              .arg(pageSize*3)
                              .arg(page * pageSize);
    addData(queryString);
}

void Logic::searchDataFromDB()
{
    if(searchText.isEmpty())
        return;

    // поиск начинается с 0 страницы
    currentPage = 0;

    // Получаем список полей для поиска
    QStringList fields = getAllFieldsFromTable("popular_tracks");

    // Формируем условие поиска
    QString searchCondition = createSearchCondition(fields);

    // Строим запрос с условием поиска
    QString queryString = "SELECT * FROM popular_tracks";
    if (!searchCondition.isEmpty())
    {
        queryString += " WHERE " + searchCondition;
    }

    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(currentPage * pageSize);

    // Выполняем запрос
    //SELECT * FROM popular_tracks WHERE field1 LIKE '%searchText%' OR field2 LIKE '%searchText%' OR ...
    addData(queryString);
}

QStringList Logic::getAllFieldsFromTable(const QString &tableName)
{
    //получаем все поля из бд для поиска
    QSqlQuery fieldQuery(db);
    fieldQuery.exec(QString("PRAGMA table_info(%1)").arg(tableName));

    QStringList fields;
    while (fieldQuery.next())
    {
        fields << fieldQuery.value(1).toString();
    }
    return fields;
}

QString Logic::createSearchCondition(const QStringList &fields)
{
    //условие поиска исходя из полей
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
    createRequest(currentPage);
}

void Logic::backPage()
{
    if (currentPage > 0)
    {
       currentPage--;
       createRequest(currentPage);
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

 QStandardItemModel *Logic::getModel() const
{
    return model;
}

void Logic::setButtonState(QObject* button, State state)
{
    //каждой кнопке делаем состояние
    buttonStateMap[button] = state;
}

void Logic::stopWorkerThread()
{
    workerThread.quit();
    workerThread.wait();
}

Logic::~Logic()
{
    stopWorkerThread();
}
