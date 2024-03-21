#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QMap>
#include <QMutex>
#include <QString>
#include <QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager& instance(const QString& dbName  = "client.db");
    ~DatabaseManager();

    QSqlDatabase& database();

private:
    QSqlDatabase db;

    // Конструктор и операторы копирования и присваивания сделаны приватными, чтобы предотвратить создание экземпляров извне
    DatabaseManager(const QString& dbName);
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static QString databasePath(const QString& dbName);
};

#endif // DATABASEMANAGER_H
