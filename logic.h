#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QMutex>

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
    ~Logic();

public:
    QStringList getAllFieldsFromTable(const QString &tableName);
    QString createSearchCondition(const QStringList &fields);
    QSqlQueryModel *getModel() const;

public:
    void initMap();
    void initThread();
    void connectToDatabase();

    void processState(QObject *sender, const QString &searchText);
    void createRequest(int page);
    void executeRequest(const QString &queryString);

    void executeDatabaseQuery(const QString &queryString);
    void searchDataFromDB();
    void nextPage();
    void backPage();
    void preLoadPages();
    void preloadRequest(const QString &queryString);

    void setButtonState(QObject *button, State state);
    void disconnectFromDatabase();
    void stopWorkerThread();

signals:
    void updateDB();
    void updateLabel(int currentPage);

private:
    int currentPage;
    int pageSize;
    int offset;

private:
    const QString dbFilename = "D:/QT_PROJECTS/BD/123.db";
    std::map<State, std::function<void()>> funcmap;

    std::map<QObject*, State> buttonStateMap;   
    //QQueue<QString> requestQueue;
    QVector<QSqlQueryModel*> preloadedModels;

    QSqlQueryModel *model;
    QThread workerThread;
    QString searchText;
    QMutex queueMutex;
    State state;
    QSqlDatabase db;
};

#endif // LOGIC_H
