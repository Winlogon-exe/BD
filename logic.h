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

enum State
{
    Next,
    Back,
    Search
};


class Logic : public QObject
{
    Q_OBJECT

public:
   explicit Logic(QObject *parent = nullptr);

public:
    void connect();
    void disconnect();
    void search(QObject* sender);

    void setButtonState(QObject* button, State state);
    void setupModel();
    QSqlQueryModel *getModel() const;

private:
    std::map<QObject*, State> buttonStateMap;
    const QString dbFilename = "D:/QT_PROJECTS/BD/test.db";
    QSqlQueryModel *model;
    QSqlDatabase db;
    State state;
};

#endif // LOGIC_H
