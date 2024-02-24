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
    void processState(QObject *sender, const QString &searchText);  // Обработка состояния
    void setButtonState(QObject *button, State state);

    void executeRequest(const QString &queryString);  // Выполнение SQL-запроса
    void createRequest();  // Создание запроса
    void searchDataFromDB(const QString &searchText);

    void nextPage();
    void backPage();
    QSqlQueryModel *getModel() const;

signals:
    // Сигналы класса
    void updateDB();  // Сигнал для обновления базы данных
    void updateLabel(int currentPage);  // Сигнал для обновления метки с номером текущей страницы

private:
    const QString dbFilename = "D:/QT_PROJECTS/BD/test.db";
    int currentPage;  // Текущая страница
    int pageSize;  // Размер страницы
    int offset;  // Смещение

    std::map<QObject*, State> buttonStateMap;
    QSqlQueryModel *model;
    QSqlDatabase db;
    State state;
};

#endif // LOGIC_H
