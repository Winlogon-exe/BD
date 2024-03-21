#include "LogicView.h"

LogicView::LogicView(QObject* parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    preload(1),
    offset(0),
    totalPages(0)
{
    //имя базы и таблицы сделать параметров конструктора и вызывать нужную в зависимости от имени и таблицы?
    qRegisterMetaType<QSharedPointer<QSqlQueryModel>>("QSharedPointer<QSqlQueryModel>");
    qRegisterMetaType<StateButtonView>("StateButtonView");
    dbFilename = QDir(QApplication::applicationDirPath()).filePath("client.db");
}

void LogicView::s_initDB()
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
    }
    else
    {
        showError(db.lastError().text());
    }
}

void LogicView::initModels()
{
    qDebug() << "\nИнициализация моделей";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    models.resize(MODELS_COUNT);
    for (int i = 0; i < models.size(); ++i)
    {
        models[i] = QSharedPointer<QSqlQueryModel>::create();
    }
}

void LogicView::initMap()
{
    qDebug() << "\nИнициализация карты функций";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    funcmap[Next]    = [this](){ nextPage(); };
    funcmap[Back]    = [this](){ backPage(); };
    funcmap[Search]  = [this](){ searchDataFromDB(); };
}

bool LogicView::connectToDatabase()
{
    connectionName = "LogicViewConnection";
    db = QSqlDatabase::database("QSQLITE");
    if (QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(dbFilename);
    }

    if (!db.open())
    {
        return false;
    }
    return true;
}

void LogicView::calculateTotalPages()
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

void LogicView::FieldsForFilter()
{

    qDebug() << "\nПолучение полей для фильтрации";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    fields = getAllFieldsFromTable(TABLE_NAME);
    emit updateFilter(fields);
}

void LogicView::executeRequest(const QString &queryString, QSharedPointer<QSqlQueryModel> model)
{
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
    emit updateTable(models[Center]);
}


void LogicView::nextPage()
{
    if(currentPage < totalPages)
    {
        qDebug() << "\nПереход на следующую страницу";
        qDebug() << "Текущий поток:" << QThread::currentThreadId();

        currentPage++;
        models[Left]->setQuery(models[Center]->query());
        models[Center]->setQuery(models[Right]->query());
        (void)QtConcurrent::run([this](){ preloadPages(currentPage + preload, models[Right]); });
        emit updateLabel(currentPage, totalPages);
    }
}

void LogicView::backPage()
{    
    if (currentPage > 0)
    {
        qDebug() << "\nПереход на предыдущую страницу";
        qDebug() << "Текущий поток:" << QThread::currentThreadId();
        currentPage--;
        models[Right]->setQuery(models[Center]->query());
        models[Center]->setQuery(models[Left]->query());
        (void)QtConcurrent::run([this](){ preloadPages(currentPage - preload, models[Left]); });
        emit updateLabel(currentPage, totalPages);
    }
}

QString LogicView::buildQueryString(int page)
{
    qDebug() << "Текущий поток buildQueryString:" << QThread::currentThreadId();

    QString queryString = "SELECT * FROM " + TABLE_NAME;
    QString searchCondition = createSearchCondition(fields);

    if (!searchCondition.isEmpty())
    {
        queryString += " WHERE " + searchCondition;
    }

    offset = page * pageSize;
    queryString += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);

    return queryString;
}


void LogicView::preloadPages(int page, QSharedPointer<QSqlQueryModel>model)
{
    qDebug() << "\nПредварительная загрузка страницы" << page;
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    QString queryString = buildQueryString(page);
    executeRequest(queryString, model);
}

void LogicView::searchDataFromDB()
{
    qDebug() << "Текущий поток searchDataFromDB:" << QThread::currentThreadId();
    if(searchText.isEmpty())
        return;

    //поиск с 0 страницы начинается
    currentPage = 0;
    executeRequest(buildQueryString(currentPage), models[Center]);
}

QStringList LogicView::getAllFieldsFromTable(const QString &tableName)
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

QString LogicView::createSearchCondition(const QStringList &fields)
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


void LogicView::s_setButtonState(QObject* button, StateButtonView state)
{
    qDebug() << "\nУстановка состояния кнопки";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    buttonStateMap[button] = state;
}

void LogicView::s_processState(QObject* sender,const QString &search,const QString filter)
{
    qDebug() << "\nОбработка состояния";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    StateButtonView state = buttonStateMap[sender];
    auto it = funcmap.find(state);

    searchText = search;
    filterText = filter;

    if (it != funcmap.end())
    {
        it->second();
    }
}

void LogicView::showError(const QString &errorText)
{
    qDebug() << "\nОтображение ошибки";
    qDebug() << "Текущий поток:" << QThread::currentThreadId();

    QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе: ") + errorText);
}

void LogicView::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        QSqlDatabase::removeDatabase(connectionName); // Освобождаем подключение
        qInfo() << "База данных закрыта. LogicView";
    }
    else
    {
        qWarning() << "База данных уже закрыта.";
    }
}

LogicView::~LogicView()
{
    qDebug()<<"деструктор LogicView";
    disconnectFromDatabase();
}

