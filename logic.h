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
#include<QMessageBox>
#include <QDebug>
#include<QThread>

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
    void connectToDatabase();
    void disconnectFromDatabase();
    void processRequest(QObject *sender,const QString &searchText);


    void setButtonState(QObject *button, State state);

    void loadDataFromDB();
    void searchDataFromDB(const QString &searchText);
    void updateOffset();
    void next();
    void back();
    QSqlQueryModel *getModel() const;

signals:
    // Сигнал для обновления базы данных
    void updateDB();
    void updateLabel(int currentPage);

private:
    const QString dbFilename = "D:/QT_PROJECTS/BD/test.db";

    int currentPage; // Текущая страница
    int pageSize; // Размер страницы
    int offset = currentPage * pageSize; // Смещение

    std::map<QObject *, State> buttonStateMap;
    QSqlQueryModel *model;
    QSqlDatabase db;
    State state;
};;

#endif // LOGIC_H
