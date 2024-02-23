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
    QSqlQueryModel* setupModel(int pageNumber, int pageSize);
    QSqlQueryModel *getModel() const;

private:
    const QString dbFilename = "D:/QT_PROJECTS/BD/test.db";
    QSqlDatabase db;
    QSqlQueryModel *model;


};

#endif // LOGIC_H
