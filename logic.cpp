#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    preload(1),
    offset(0),
    totalPages(0)
{
    initThread();
    initModels();//!

    initDB();
    initMap();
}

void Logic::initThread()
{
    this->moveToThread(&workerThread);
    workerThread.start();
}

void Logic::initModels()
{
    models = QVector<QSqlQueryModel*>(3);

    for (int i = 0; i < models.size(); ++i)
    {
        models[i] = new QSqlQueryModel();
    }
}

void Logic::initMap()
{

    funcmap[Next]    = [this](){ nextPage(); };
    funcmap[Back]    = [this](){ backPage(); };
    funcmap[Search]  = [this](){ searchDataFromDB(); };
}

void Logic::initDB()
{
    if(connectToDatabase())
    {
        FieldsForFilter();
        calculateTotalPages();

        executeRequest(buildQueryString(currentPage), models[center]);
        executeRequest(buildQueryString(currentPage + preload), models[right]);
    }
    else
    {
        showError(db.lastError().text());
    }
}

bool Logic::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open())
    {
        return false;
    }
    qInfo() << "База данных успешно открыта.";
    return true;
}

void Logic::calculateTotalPages()
{
    QSqlQuery query("SELECT COUNT(*) FROM " + TABLE_NAME);
    int totalRecords = 0;
    if (query.next())
    {
        totalRecords = query.value(0).toInt();
    }
    totalPages = (totalRecords + pageSize) / pageSize;
}

void Logic::FieldsForFilter()
{
    fields = getAllFieldsFromTable(TABLE_NAME);
}

void Logic::executeRequest(const QString &queryString, QSqlQueryModel *model)
{
    QSqlQuery query(db);
    if (!query.exec(queryString))
    {
        showError(query.lastError().text());
        return;
    }
    model->setQuery(std::move(query));
}

void Logic::nextPage()
{
    if(currentPage < totalPages)
    {
        currentPage++;
        models[left]->setQuery(models[center]->query());
        models[center]->setQuery(models[right]->query());

        preloadPages(currentPage + preload, models[right]);
    }
}

void Logic::backPage()
{
    if (currentPage > 0)
    {
        currentPage--;
        models[right]->setQuery(models[center]->query());
        models[center]->setQuery(std::move(models[left]->query()));

        preloadPages(currentPage - preload, models[left]);
    }
}

QString Logic::buildQueryString(int page)
{
    QString queryString = "SELECT * FROM " + TABLE_NAME;

    QStringList fields = getAllFieldsFromTable(TABLE_NAME);
    QString searchCondition = createSearchCondition(fields);
    if (!searchCondition.isEmpty())
    {
        queryString += " WHERE " + searchCondition;
    }

    offset = page * pageSize;
    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);

    return queryString;
}

void Logic::preloadPages(int page, QSqlQueryModel *model)
{
    QString queryString = buildQueryString(page);
    executeRequest(queryString, model);
}

void Logic::searchDataFromDB()
{
    if(searchText.isEmpty())
        return;

    //поиск с 0 страницы начинается
    currentPage = 0;
    executeRequest(buildQueryString(currentPage), models[center]);
}

QStringList Logic::getAllFieldsFromTable(const QString &tableName)
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

QString Logic::createSearchCondition(const QStringList &fields)
{
    //!!
    if (searchText.isEmpty())
        return "";

    QStringList conditions;
    if (filterText == "All")
    {
        for (const auto &field : fields)
        {
            conditions << QString("%1 LIKE '%%2%'").arg(field).arg(searchText);
        }
    }
    else
    {
        conditions << QString("%1 LIKE '%%2%'").arg(filterText).arg(searchText);
    }

    return conditions.join(" OR ");
}

void Logic::setButtonState(QObject* button, State state)
{
    buttonStateMap[button] = state;
}

void Logic::processState(QObject* sender,const QString &search,const QString filter)
{
    State state = buttonStateMap[sender];
    auto it = funcmap.find(state);
    searchText = search;
    filterText = filter;

    if (it != funcmap.end())
    {
        it->second();
    }
    emit updateLabel(currentPage, totalPages);
}

void Logic::stopWorkerThread()
{
    if(workerThread.isRunning())
    {
        workerThread.quit();
        workerThread.wait();
    }
}

void Logic::showError(const QString &errorText)
{
    QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе: ") + errorText);
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

QSqlQueryModel* Logic::getsqlModel() const
{
    return models[center];
}

QStringList Logic::getFields() const
{
    return fields;
}

Logic::~Logic()
{
    stopWorkerThread();
}
