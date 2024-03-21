#include "LogicView.h"

LogicView::LogicView(QObject* parent) :
    QObject(parent),
    currentPage(0),
    pageSize(30),
    preload(1),
    offset(0),
    totalPages(0)
{
    initTypes();
}

void LogicView::initTypes()
{
    qRegisterMetaType<QSharedPointer<QSqlQueryModel>>("QSharedPointer<QSqlQueryModel>");
    qRegisterMetaType<StateButtonView>("StateButtonView");
}

void LogicView::s_initDB()
{
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
    models.resize(MODELS_COUNT);
    for (int i = 0; i < models.size(); ++i)
    {
        models[i] = QSharedPointer<QSqlQueryModel>::create();
    }
}

void LogicView::initMap()
{
    funcmap[Next]    = [this](){ nextPage(); };
    funcmap[Back]    = [this](){ backPage(); };
    funcmap[Search]  = [this](){ searchDataFromDB(); };
}

bool LogicView::connectToDatabase()
{
    auto& dbManager = DatabaseManager::instance("client.db");
    db = dbManager.database();
    if (!db.open())
    {
        return false;
    }
    return true;
}

void LogicView::calculateTotalPages()
{
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
        currentPage--;
        models[Right]->setQuery(models[Center]->query());
        models[Center]->setQuery(models[Left]->query());
        (void)QtConcurrent::run([this](){ preloadPages(currentPage - preload, models[Left]); });
        emit updateLabel(currentPage, totalPages);
    }
}

QString LogicView::buildQueryString(int page)
{
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
    QString queryString = buildQueryString(page);
    executeRequest(queryString, model);
}

void LogicView::searchDataFromDB()
{
    if(searchText.isEmpty())
        return;

    currentPage = 0;
    executeRequest(buildQueryString(currentPage), models[Center]);
}

QStringList LogicView::getAllFieldsFromTable(const QString &tableName)
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

QString LogicView::createSearchCondition(const QStringList &fields)
{
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
    buttonStateMap[button] = state;
}

void LogicView::s_processState(QObject* sender,const QString &search,const QString filter)
{
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
    QMessageBox::critical(nullptr, QObject::tr("Ошибка"), QObject::tr("Ошибка в запросе: ") + errorText);
}

void LogicView::disconnectFromDatabase()
{
    if (db.isOpen())
    {
        db.close();
        qInfo() << "База данных закрыта. LogicView";
    }
    else
    {
        qWarning() << "База данных уже закрыта.";
    }
}

LogicView::~LogicView()
{
    qWarning() << "Дестуктор LogicView";
}
