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

enum Models
{
    center,
    right,
    left
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
    QString buildQueryString(int page);
    QSqlQueryModel* getsqlModel() const;
    QStringList getFields() const;

public:
    bool connectToDatabase();
    void initDB();
    void initModels();
    void FieldsForFilter();

    void processState(QObject *sender, const QString &searchText,const QString filter);
    void executeRequest(const QString &queryString,QSqlQueryModel *model);
    void preloadPages(int page, QSqlQueryModel *model);

    void searchDataFromDB();
    void nextPage();
    void backPage();

    void calculateTotalPages();
    void showError(const QString &error);
    void disconnectFromDatabase();

public slots:
    void initMap();
    void setButtonState(QObject *button, State state);

signals:
    void updateLabel(int currentPage, int totalPages);
    //void updateDB();

public:
    int currentPage;
    int totalPages;

private:
    int pageSize;
    int preload;
    int offset;

private:
    const QString TABLE_NAME = "popular_tracks";
    const QString dbFilename = "D:/QT_PROJECTS/BD/123.db";
    std::map<State, std::function<void()>> funcmap;
    std::map<QObject*, State> buttonStateMap;
    QVector<QSqlQueryModel*> models;

    QStringList fields;
    QString filterText;

    QString searchText;
    QMutex queueMutex;
    QSqlDatabase db;
    State state;
};

#endif // LOGIC_H
