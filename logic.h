#ifndef LOGIC_H
#define LOGIC_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include<QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QApplication>
#include<QSqlQueryModel>
#include <QSqlTableModel>

class Logic : public QObject
{
    Q_OBJECT

public:
   explicit Logic(QObject *parent = nullptr);

public:
    void connect();
    void disconnect();
    void setupModel();
    void loadData(int pageNumber, int pageSize);

    QSqlQueryModel *getModel() const;

signals:
    void dataChanged(QSqlQueryModel *model);

private:
    const QString dbFilename = "D:/bd/123.db";
    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // LOGIC_H
