#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    preload(1),
    offset(0),
    totalPages(0)
{
    initModels();
    initMap();
    initDB();
    initThread();
}

void Logic::initThread()
{
    this->moveToThread(&workerThread);
    workerThread.start();
}

void Logic::initModels()
{
    models.resize(3);
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
        preloadPages(currentPage, models[center]); // Используем перечисление для доступа к модели
        preloadPages(currentPage + preload, models[right]);
        calculateTotalPages();
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

void Logic::processState(QObject* sender,const QString &search)
{
    State state = buttonStateMap[sender];
    auto it = funcmap.find(state);
    searchText = search;

    if (it != funcmap.end())
    {
        it->second();
    }
    emit updateLabel(currentPage, totalPages);
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

QString Logic::buildQueryString(int page)
{
    offset = page * pageSize;
    return QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2")
        .arg(pageSize)
        .arg(offset);
}

void Logic::preloadPages(int page, QSqlQueryModel *model)
{
    QString queryString = buildQueryString(page);
    executeRequest(queryString, model);
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
        models[center]->setQuery(models[left]->query());

        preloadPages(currentPage - preload, models[left]);
    }
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
    executeRequest(queryString, models[center]);
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
    qDebug() << "Запрос на загрузку данных для страницы" << currentPage;
    //условие поиска исходя из полей
    QStringList conditions;
    for (const auto &field : fields)
    {
        conditions << QString("%1 LIKE '%%2%'").arg(field).arg(searchText);

    }
    return conditions.join(" OR ");
}

void Logic::calculateTotalPages()
{
    QSqlQuery query("SELECT COUNT(*) FROM popular_tracks");
    int totalRecords = 0;
    if (query.next())
    {
        totalRecords = query.value(0).toInt();
    }
    totalPages = (totalRecords + pageSize) / pageSize;
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

void Logic::showError(const QString &errorText)
{
    QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе: ") + errorText);
}

Logic::~Logic()
{
    stopWorkerThread();
}
