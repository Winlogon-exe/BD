#include "logic.h"

Logic::Logic(QObject *parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    preload(1),
    offset(0),
    totalPages(0)
{
    dbFilename = QDir(QApplication::applicationDirPath()).filePath("123.db");
    qRegisterMetaType<QSharedPointer<QSqlQueryModel>>("QSharedPointer<QSqlQueryModel>");
    qRegisterMetaType<State>("State");
}

void Logic::initDB()
{

    qDebug() << "\nИнициализация базы данных";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    if (connectToDatabase())
    {
        initMap();
        initModels();

        FieldsForFilter();
        executeRequest(buildQueryString(currentPage), models[Center]);

        (void)QtConcurrent::run([this](){ calculateTotalPages(); });
        (void)QtConcurrent::run([this](){ preloadPages(currentPage + preload, models[Right]); });

        emit updateTable(models[Center]);
    }
    else
    {
        showError(db.lastError().text());
    }
}

void Logic::initModels()
{
    qDebug() << "\nИнициализация моделей";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    models.resize(MODELS_COUNT);
    for (int i = 0; i < models.size(); ++i)
    {
        models[i] = QSharedPointer<QSqlQueryModel>::create();
    }
}

void Logic::initMap()
{
    qDebug() << "\nИнициализация карты функций";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    funcmap[Next]    = [this](){ nextPage(); };
    funcmap[Back]    = [this](){ backPage(); };
    funcmap[Search]  = [this](){ searchDataFromDB(); };
}

bool Logic::connectToDatabase()
{
    qDebug() << "\nПодключение к базе данных";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

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
    qDebug() << "\nРасчет общего количества страниц";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();
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
    totalPages = (totalRecords + pageSize) / pageSize - 1; // -1 для округления
    emit updateLabel(currentPage, totalPages);
}

void Logic::FieldsForFilter()
{

    qDebug() << "\nПолучение полей для фильтрации";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    fields = getAllFieldsFromTable(TABLE_NAME);
    emit updateFilter(fields);
}

void Logic::executeRequest(const QString &queryString, QSharedPointer<QSqlQueryModel> model) {
    QSqlQuery query(db);
    if (!query.prepare(queryString))
    {
        showError(query.lastError().text());
        return;
    }

    if (!query.exec())
    {
        showError(query.lastError().text());
        return;
    }

    model->setQuery(std::move(query));
}


void Logic::nextPage()
{

    qDebug() << "\nПереход на следующую страницу";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    currentPage++;
    models[Left]->setQuery(models[Center]->query());
    models[Center]->setQuery(models[Right]->query());
    (void)QtConcurrent::run([this](){ preloadPages(currentPage + preload, models[Right]); });
    emit updateLabel(currentPage, totalPages);
}

void Logic::backPage()
{

    qDebug() << "\nПереход на предыдущую страницу";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    if (currentPage > 0)
    {
        currentPage--;
        models[Right]->setQuery(models[Center]->query());
        models[Center]->setQuery(models[Left]->query());
        (void)QtConcurrent::run([this](){ preloadPages(currentPage - preload, models[Left]); });
        emit updateLabel(currentPage, totalPages);
    }
}

QString Logic::buildQueryString(int page)
{
    qDebug() << "\nПостроение SQL-запроса для страницы" << page;
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    QString queryString = "SELECT * FROM " + TABLE_NAME;
    offset = page * pageSize;
    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);

    return queryString;
}


void Logic::preloadPages(int page, QSharedPointer<QSqlQueryModel>model)
{
    qDebug() << "\nПредварительная загрузка страницы" << page;
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    QString queryString = buildQueryString(page);
    executeRequest(queryString, model);
}

void Logic::searchDataFromDB()
{

    qDebug() << "\nПоиск данных в базе данных";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    if (searchText.isEmpty())
    {
        qDebug() << "Поисковый запрос пуст";
        return;
    }
    QString condition = createSearchCondition(fields);

    int page = 0; // Начинаем поиск с первой страницы
    QString queryString = "SELECT * FROM " + TABLE_NAME;
    if (!condition.isEmpty())
    {
        queryString += " WHERE " + condition;
    }
    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(page * pageSize);

    executeRequest(queryString, models[Center]);
}

QStringList Logic::getAllFieldsFromTable(const QString &tableName)
{
    qDebug() << "\nПолучение списка полей из таблицы" << tableName;
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

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
    qDebug() << "\nСоздание условия поиска";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

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
    qDebug() << "\nУстановка состояния кнопки";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    buttonStateMap[button] = state;
}

void Logic::processState(QObject* sender,const QString &search,const QString filter)
{
    qDebug() << "\nОбработка состояния";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    State state = buttonStateMap[sender];
    auto it = funcmap.find(state);
    searchText = search;
    filterText = filter;

    if (it != funcmap.end())
    {
        it->second();
    }
}

void Logic::showError(const QString &errorText)
{
    qDebug() << "\nОтображение ошибки";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

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
