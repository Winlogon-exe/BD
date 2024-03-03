#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    model(new QStandardItemModel())

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
        showError(db.lastError().text());
        return;
    }
    qInfo() << "База данных успешно открыта.";

    createRequest();
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
    model->clear(); // Очищаем модель от предыдущих данных

    // Проверяем, есть ли в кеше данные для текущей страницы
    if (dataCache.contains(currentPage))
    {
        // Извлекаем данные для текущей страницы из кеша
        const auto &pageData = dataCache[currentPage];
        for (const auto &rowMap : pageData)
        {
            QList<QStandardItem *> items;
            for (const auto &value : rowMap)
            {
                items.append(new QStandardItem(value.toString()));
            }
            model->appendRow(items);
        }
    }
}

//выполнение запроса и добавления в кеш
void Logic::addData(const QString &queryString, int targetPage)
{
    QSqlQuery query(db);
    query.prepare(queryString);

    if (!query.exec())
    {
        showError(query.lastError().text());
        return;
    }

    QList<QVariantMap> pageData; // Данные для целевой страницы

    while (query.next())
    {
        QVariantMap rowData;
        for (int column = 0; column < query.record().count(); ++column)
        {
            rowData[query.record().fieldName(column)] = query.value(column);
        }
        pageData.append(rowData); // Добавляем строку данных в список текущей страницы
    }

    // Сохраняем данные целевой страницы в кеше
    dataCache[targetPage] = pageData;
    executeRequest();
}

void Logic::createRequest()
{
    // Загружаем данные для текущей страницы
    int queryOffset = currentPage * pageSize;
    QString queryString = QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2")
                              .arg(pageSize)
                              .arg(queryOffset);
    addData(queryString, currentPage);

    // Предварительно загружаем данные для следующих страниц
    int preloadPageCount = 3;
    preloadPages(currentPage, preloadPageCount);
}

void Logic::preloadPages(int startPage, int pageCount)
{
    for (int i = 0; i < pageCount; ++i)
    {
        int page = startPage + i;
        int offset = page * pageSize;
        QString queryString = QString("SELECT * FROM popular_tracks LIMIT %1 OFFSET %2")
                                  .arg(pageSize)
                                  .arg(offset);
        addData(queryString, page);
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
    addData(queryString,currentPage);
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
        // Если данные есть, используем их
        executeRequest(); // Используем данные из кэша
        qDebug() << "Страница" << currentPage << "загружена из кэша.";
    }
    else
    {
        // Если данных нет, загружаем их
        createRequest(); // Загружаем данные для текущей и следующей страницы
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
