#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QApplication>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include<QLineEdit>

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
    // Методы класса
    void connectToDatabase();
    void disconnectFromDatabase();
    void processState(QObject *sender, const QString &searchText);

    void setButtonState(QObject *button, State state);
    void executeRequest(const QString &queryString);
    void createRequest();

    void searchDataFromDB(const QString &searchText);
    QString  createSearchCondition(const QStringList &fields, const QString &searchText);
    QStringList getAllFieldsFromTable (const QString &tableName);

    void nextPage();
    void backPage();
    QSqlQueryModel *getModel() const;

signals:
    // Сигналы класса
    void updateDB();
    void updateLabel(int currentPage);

private:
    // Приватные члены класса
    const QString dbFilename = "D:/QT_PROJECTS/BD/123.db";
    int currentPage;
    int pageSize;
    int offset;

    std::map<QObject*, State> buttonStateMap;
    QSqlQueryModel *model;
    QSqlDatabase db;
    State state;
};


#endif // LOGIC_H
