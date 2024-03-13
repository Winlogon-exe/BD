#include "logic.h"
 //QThread::sleep(5); // Задержка на 5 секунд

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    preload(1),
    offset(0),
    totalPages(0)
{
    qRegisterMetaType<QSharedPointer<QSqlQueryModel>>("QSharedPointer<QSqlQueryModel>");
    qRegisterMetaType<State>("State");
}

void Logic::initDB()
{
    qDebug() << "Текущий поток initDB :" << QThread::currentThreadId();

    if(connectToDatabase())
    {
        initMap();
        initModels();
        FieldsForFilter();
        executeRequest(buildQueryString(currentPage), models[Center]);

        QtConcurrent::run(this, &Logic::calculateTotalPages);
        QtConcurrent::run(this, &Logic::preloadPages, currentPage + preload, models[Right]);
        emit updateTable(models[Center]);
    }
    else
    {
        showError(db.lastError().text());
    }
}

void Logic::initModels()
{
    qDebug() << "Текущий поток initModels:" << QThread::currentThreadId();
    for (int i = 0; i < 3; ++i)
    {
        models.append(QSharedPointer<QSqlQueryModel>::create());
    }
}

void Logic::initMap()
{
    qDebug() << "Текущий поток initMap :" << QThread::currentThreadId();

    funcmap[Next]    = [this](){ nextPage(); };
    funcmap[Back]    = [this](){ backPage(); };
    funcmap[Search]  = [this](){ searchDataFromDB(); };
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
    QThread::sleep(5);
    qDebug() << "Текущий поток calculateTotalPages:" << QThread::currentThreadId();
    QSqlQuery query(db);

    int totalRecords = 0;
    if (query.exec("SELECT COUNT(*) FROM " + TABLE_NAME))
    {
        if (query.next())
        {

            totalRecords = query.value(0).toInt();
        }
    }
    else
    {
        showError(db.lastError().text());
    }
    totalPages = (totalRecords + pageSize) / pageSize - 1;//-1 для округления
    emit updateLabel(currentPage, totalPages);
}

void Logic::FieldsForFilter()
{
    fields = getAllFieldsFromTable(TABLE_NAME);
    emit updateFilter(fields);
}

void Logic::executeRequest(const QString &queryString, QSharedPointer<QSqlQueryModel>model)
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
    qDebug() << "Текущий поток nextPage:" << QThread::currentThreadId();

    currentPage++;
    models[Left]->setQuery(models[Center]->query());
    models[Center]->setQuery(models[Right]->query());
    QtConcurrent::run(this, &Logic::preloadPages, currentPage + preload, models[Right]);
    emit updateLabel(currentPage, totalPages);
}

void Logic::backPage()
{
    qDebug() << "Текущий поток backPage:" << QThread::currentThreadId();

    if (currentPage > 0)
    {
        currentPage--;
        models[Right]->setQuery(models[Center]->query());
        models[Center]->setQuery(models[Left]->query());
        QtConcurrent::run(this, &Logic::preloadPages, currentPage - preload, models[Left]);
        emit updateLabel(currentPage, totalPages);
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

void Logic::preloadPages(int page, QSharedPointer<QSqlQueryModel>model)
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
    int currentPage = 0;
    executeRequest(buildQueryString(currentPage), models[Center]);
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
    qDebug() << "Текущий поток setButtonState:" << QThread::currentThreadId();
    buttonStateMap[button] = state;
}

void Logic::processState(QObject* sender,const QString &search,const QString filter)
{
    qDebug() << "\nТекущий поток processState:" << QThread::currentThreadId();
    State state = buttonStateMap[sender];
    auto it = funcmap.find(state);
    searchText = search;
    filterText = filter;

    if (it != funcmap.end())
    {
        it->second();
    }
    //emit updateTable(models[Center]);
   // QThread::sleep(5);
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

Logic::~Logic()
{

}
