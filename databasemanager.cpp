#include "DatabaseManager.h"

QMap<QString, DatabaseManager*> DatabaseManager::instances; // Инициализация статического контейнера

DatabaseManager& DatabaseManager::instance(const QString& dbName)
{
    if (!instances.contains(dbName))
    {
        instances[dbName] = new DatabaseManager(databasePath(dbName));
    }
    return *instances[dbName];
}

DatabaseManager::DatabaseManager(const QString& dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE", dbName); // Используем уникальное имя для каждого соединения
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        qDebug() << "Error opening database" << dbName;
    }
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

QSqlDatabase DatabaseManager::database()
{
    return db;
}

QString DatabaseManager::databasePath(const QString& dbName)
{
    return QDir(QApplication::applicationDirPath()).filePath(dbName);
}
