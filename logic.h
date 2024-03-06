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
    QList<QVariantMap> fetchPageData(QSqlQuery &query);
    QString buildQueryString(int page);
    QSqlQueryModel* getsqlModel() const;

public:
    bool connectToDatabase();
    void initThread();
    void initMap();
    void initDB();

    void processState(QObject *sender, const QString &searchText);
    void loadCenterModel();
    void executeRequest(const QString &queryString,QSqlQueryModel *model);

    void addDataToCache(const QString &queryString, int targetPage);
    void setButtonState(QObject *button, State state);
    void preloadNextPages();
    void preloadPreviousPages();

    void searchDataFromDB();
    void nextPage();
    void backPage();

    void calculateTotalPages();
    void showError(const QString &error);
    void stopWorkerThread();
    void disconnectFromDatabase();

signals:
    void updateLabel(int currentPage, int totalPages);
   // void updateDB();

private:
    int currentPage;
    int pageSize;
    int preload;
    int offset;
    int totalPages;

private:
    const QString dbFilename = "D:/QT_PROJECTS/BD/123.db";
    std::map<State, std::function<void()>> funcmap;
    std::map<QObject*, State> buttonStateMap;

    QCache<int, QList<QSqlQueryModel*>> dataCache;
    QSqlQueryModel *modelCenter;
    QSqlQueryModel *modelRight;
    QSqlQueryModel *modelLeft;

    QThread workerThread;
    QString searchText;
    QMutex queueMutex;
    QSqlDatabase db;
    State state;
};

#endif // LOGIC_H
