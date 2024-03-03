#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include<QStandardItemModel>

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
    QStandardItemModel *getModel() const;

public:
    void initMap();
    void initThread();
    void connectToDatabase();

    void processState(QObject *sender, const QString &searchText);
    void createRequest();
    void executeRequest();

    void addData(const QString &queryString, int targetPage);
    void searchDataFromDB();
    void nextPage();
    void backPage();
    void preloadPages(int startPage, int pageCount);
    QList<QVariantMap> fetchPageData(QSqlQuery &query);

    void setButtonState(QObject *button, State state);
    void disconnectFromDatabase();
    void stopWorkerThread();
    void showError(const QString &error);

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

    QMap<int, QList<QVariantMap>> dataCache;
    QStandardItemModel *model;

    QThread workerThread;
    QString searchText;
    QMutex queueMutex;
    State state;
    QSqlDatabase db;
};

#endif // LOGIC_H
