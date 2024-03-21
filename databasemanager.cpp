#include "DatabaseManager.h"
#include <QApplication>
#include <QDir>

DatabaseManager& DatabaseManager::instance(const QString& dbName)
{
    static QMap<QString, DatabaseManager*> instances;

    if (!instances.contains(dbName))
    {
        qDebug() << "instances";
        instances[dbName] = new DatabaseManager(databasePath(dbName));
    }
    return *instances[dbName];
}

DatabaseManager::DatabaseManager(const QString& dbName)
{
    qDebug() << "DatabaseManager";
    db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    db.setDatabaseName(dbName);
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
