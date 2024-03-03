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
    QStandardItemModel *getModel() const;
    QString buildQueryString(int page);

public:
    bool connectToDatabase();
    void initThread();
    void initMap();
    void initDB();

    void processState(QObject *sender, const QString &searchText);
    void createNewPagesRequest();
    void executeRequest();

    void addData(const QString &queryString, int targetPage);
    void setButtonState(QObject *button, State state);
    void preloadPages(int startPage, int pageCount);
    void searchDataFromDB();
    void nextPage();
    void backPage();

    void showError(const QString &error);
    void stopWorkerThread();
    void disconnectFromDatabase();

signals:
    void updateLabel(int currentPage);
    void updateDB();

private:
    int currentPage;
    int pageSize;
    int offset;
    int preload;

private:
    const QString dbFilename = "D:/QT_PROJECTS/BD123.db";
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
