#ifndef LOGICVIEW_H
#define LOGICVIEW_H

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

enum StateButtonView
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

class LogicView : public QObject
{
    Q_OBJECT

public:
    explicit LogicView(const QString &db,const QString &table,QObject* parent = nullptr);
    ~LogicView();

public:
    QStringList getAllFieldsFromTable(const QString &tableName);
    QString createSearchCondition(const QStringList &fields);
    QString buildQueryString(int page);

public:
    void initMap();
    void initModels();
    void initTypes();
    bool connectToDatabase();

    void FieldsForFilter();   
    void executeRequest(const QString &queryString,QSharedPointer<QSqlQueryModel>model);
    void preloadPages(int page, QSharedPointer<QSqlQueryModel>model);

    void searchDataFromDB();
    void nextPage();
    void backPage();

    void calculateTotalPages();
    void showError(const QString &error);
    void disconnectFromDatabase();

public slots:
    void s_initDB();
    void s_setButtonState(QObject *button, StateButtonView state);
    void s_processState(QObject *sender, const QString &searchText,const QString filter);

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
    const QString TABLE_NAME;//users
    QString dbFilename;//client.db

    std::map<StateButtonView, std::function<void()>> funcmap;
    std::map<QObject*, StateButtonView> buttonStateMap;
    QVector<QSharedPointer<QSqlQueryModel>> models;

    StateButtonView state;
    QStringList fields;
    QString filterText;
    QString searchText;
    QSqlDatabase db;
};

#endif // LOGICVIEW_H
