#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QDir>
#include<QStandardItemModel>
#include <QtConcurrent/QtConcurrent>

enum State
{
    Next,
    Back,
    Search
};

enum Models
{
    Center,
    Right,
    Left
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

public:
    bool connectToDatabase();
    void initDB();
    void initModels();

    void FieldsForFilter();
    void processState(QObject *sender, const QString &searchText,const QString filter);
    void executeRequest(const QString &queryString,QSharedPointer<QSqlQueryModel>model);
    void preloadPages(int page, QSharedPointer<QSqlQueryModel>model);

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
    void updateFilter(const QStringList &fields);
    void updateTable(QSharedPointer<QSqlQueryModel>model);

private:
    int currentPage;
    int totalPages;
    int pageSize;
    int preload;
    int offset;

private:
    static const int MODELS_COUNT = 3;
    const QString TABLE_NAME = "popular_tracks";
    QString dbFilename;
    QMutex mutex;
    QMutex mutex2;
    std::map<State, std::function<void()>> funcmap;
    std::map<QObject*, State> buttonStateMap;
    QVector<QSharedPointer<QSqlQueryModel>> models;

    QStringList fields;
    QString filterText;
    QString searchText;
    QMutex queueMutex;
    QSqlDatabase db;
    State state;
};

#endif // LOGIC_H
