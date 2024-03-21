#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QDir>
#include <QApplication>

class DatabaseManager {
public:
    static DatabaseManager& instance(const QString& dbName = "client.db");
    QSqlDatabase& database();

private:
    DatabaseManager(const QString& dbName);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase db;
    static QString databasePath(const QString& dbName);
};

#endif // DATABASEMANAGER_H
