#include "logic.h"
 //QThread::sleep(5); // Задержка на 5 секунд

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(200),
    preload(1),
    offset(0),
    totalPages(0)
{

    initModels();
    initMap();
}

void Logic::initModels()
{
    qDebug() << "Текущий поток initModels:" << QThread::currentThreadId();
    models = QVector<QSqlQueryModel*>(3);

    for (int i = 0; i < models.size(); ++i)
    {
        models[i] = new QSqlQueryModel();
    }
}

void Logic::initMap()
{
    qDebug() << "initMap вызван в потоке:" << QThread::currentThreadId();

    funcmap[Next]    = [this](){ nextPage(); };
    funcmap[Back]    = [this](){ backPage(); };
    funcmap[Search]  = [this](){ searchDataFromDB(); };
}

void Logic::initDB()
{
    qDebug() << "Текущий поток initDB :" << QThread::currentThreadId();
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
    qDebug() << "Текущий поток connectToDatabase:" << QThread::currentThreadId();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilename);
    if (!db.open())
    {
        return false;
    }
    return true;
}

void Logic::calculateTotalPages()
{
    qDebug() << "Текущий поток calculateTotalPages:" << QThread::currentThreadId();

    QSqlQuery query("SELECT COUNT(*) FROM " + TABLE_NAME);
    int totalRecords = 0;
    if (query.next())
    {
        totalRecords = query.value(0).toInt();
    }
    totalPages = (totalRecords + pageSize) / pageSize - 1;
}

void Logic::FieldsForFilter()
{
    fields = getAllFieldsFromTable(TABLE_NAME);
}

void Logic::executeRequest(const QString &queryString, QSqlQueryModel *model)
{
    qDebug() << "Текущий поток executeRequest:" << QThread::currentThreadId();

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
    qDebug() << "\nТекущий поток nextPage:" << QThread::currentThreadId();

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
    qDebug() << "Текущий поток backPage:" << QThread::currentThreadId();

    if (currentPage > 0)
    {
        currentPage--;
        models[right]->setQuery(models[center]->query());
        models[center]->setQuery(models[left]->query());

        preloadPages(currentPage - preload, models[left]);
    }
}

QString Logic::buildQueryString(int page)
{
    qDebug() << "Текущий поток buildQueryString:" << QThread::currentThreadId();

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
    qDebug() << "Текущий поток preloadPages: " << QThread::currentThreadId();

    QString queryString = buildQueryString(page);
    executeRequest(queryString, model);
}

void Logic::searchDataFromDB()
{
    qDebug() << "Текущий поток searchDataFromDB:" << QThread::currentThreadId();
    if(searchText.isEmpty())
        return;

    //поиск с 0 страницы начинается
    currentPage = 0;
    executeRequest(buildQueryString(currentPage), models[center]);
}

QStringList Logic::getAllFieldsFromTable(const QString &tableName)
{
    qDebug() << "Текущий поток getAllFieldsFromTable:" << QThread::currentThreadId();
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
    qDebug() << "Текущий поток createSearchCondition:" << QThread::currentThreadId();
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
    qDebug() << "setButtonState вызван в потоке:" << QThread::currentThreadId();
    buttonStateMap[button] = state;
}

void Logic::processState(QObject* sender,const QString &search,const QString filter)
{
    qDebug() << "processState вызван в потоке:" << QThread::currentThreadId();
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
    qDebug() << "Текущий поток getsqlModel:" << QThread::currentThreadId();
    return models[center];
}

QStringList Logic::getFields() const
{
    qDebug() << "Текущий поток getFields:" << QThread::currentThreadId();
    return fields;
}

Logic::~Logic()
{

}
