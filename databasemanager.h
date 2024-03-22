#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QDir>
#include <QApplication>
#include <QMap>

class DatabaseManager
{
public:
    static DatabaseManager& instance(const QString& dbName);
    QSqlDatabase database();

private:
    DatabaseManager(const QString& dbName);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase db;
    static QMap<QString, DatabaseManager*> instances; // Статический контейнер для хранения экземпляров
    static QString databasePath(const QString& dbName);
};

#endif // DATABASEMANAGER_H

