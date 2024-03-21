#include "DatabaseManager.h"

DatabaseManager& DatabaseManager::instance(const QString& dbName)
{
    static DatabaseManager instance(databasePath(dbName));
    return instance;
}

DatabaseManager::DatabaseManager(const QString& dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        qDebug()<<"error DatabaseManager";
    }
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

QSqlDatabase& DatabaseManager::database()
{
    return db;
}

QString DatabaseManager::databasePath(const QString& dbName)
{
    return QDir(QApplication::applicationDirPath()).filePath(dbName);
}
