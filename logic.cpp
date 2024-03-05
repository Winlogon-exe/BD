#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    offset(0),
    currentPage(0),
    pageSize(30),
    preload(3),
    model(new QStandardItemModel()),
    sqlmodel(new QSqlQueryModel())

{
    initThread();
    initMap();
    initDB();
    dataCache.setMaxCost(10); // Установка максимального размера кэша
}

void Logic::initThread()
{
    this->moveToThread(&workerThread);
    workerThread.start();
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
        createNewPagesRequest();
    }
    else
    {
        showError(db.lastError().text());
    }
}

//сделать bool
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
    emit updateLabel(currentPage);
}

//достаем из кеша и добавляем в модель
void Logic::executeRequest()
{
    model->clear();
    // Получаем из кэша (ключ - текущая страница)
    QList<QVariantMap>* pageData = dataCache.object(currentPage);

    if (!pageData)
    {
        // Обработка случая, когда данные отсутствуют в кэше
        createNewPagesRequest();
        qDebug() << "Данные для страницы" << currentPage << "не найдены в кэше.";
        return;
    }

    for (const auto &rowMap : *pageData)
    {
        QList<QStandardItem *> items;
        for (const auto &value : rowMap)
        {
            QStandardItem *item = new QStandardItem();
            item->setData(value.toString(), Qt::DisplayRole);
            items.append(item);
        }
        model->appendRow(items);
    }
}

//сохранение данных в кеше
void Logic::addDataToCache(const QString &queryString, int targetPage)
{
    QSqlQuery query(db);
    query.prepare(queryString);

    if (!query.exec())
    {
        showError(query.lastError().text());
        return;
    }

    QList<QVariantMap>* pageData = new QList<QVariantMap>;
    while (query.next())
    {
        QSqlRecord record = query.record();
        QVariantMap rowData;
        for(int column = 0; column < record.count(); ++column)
        {
            rowData.insert(record.fieldName(column), record.value(column));
        }
        pageData->append(rowData);// Добавляем строку данных в список текущей страницы
    }

    // Сохраняем данные целевой страницы в кеше
    dataCache.insert(targetPage, pageData);
    executeRequest();
}


QString Logic::buildQueryString(int page)
{
    offset = page * pageSize;
    return QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2")
        .arg(pageSize)
        .arg(offset);
}

void Logic::createNewPagesRequest()
{
    QString queryString = buildQueryString(currentPage);
    addDataToCache(queryString, currentPage);
    preloadPages(currentPage, preload);
}

void Logic::preloadPages(int startPage, int pageCount)
{
    for (int i = 0; i < pageCount; ++i)
    {
        int page = startPage + i;
        QString queryString = buildQueryString(page);
        addDataToCache(queryString, page);
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
    addDataToCache(queryString,currentPage);
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

void Logic::nextPage()
{
    // Переход на следующую страницу
    currentPage++;

    // Проверяем, есть ли данные для следующей страницы в кеше
    if (dataCache.contains(currentPage))
    {
        executeRequest(); // Используем данные из кэша
        qDebug() << "Страница" << currentPage << "загружена из кэша.";
    }
    else
    {
        // Если данных нет, загружаем их
        createNewPagesRequest();
        qDebug() << "Загрузка данных для страницы " << currentPage;
    }
}

void Logic::backPage()
{
    if (currentPage > 0)
    {
        qDebug() << "Страница" << currentPage << "загружена из кэша.";
        currentPage--;
        executeRequest(); //данные для предыдущих страниц всегда будут в кэше
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

QSqlQueryModel* Logic::getsqlModel() const
{
    return sqlmodel;
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
    dataCache.clear();
}
