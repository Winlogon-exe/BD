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
    Search,
    Filtr
};

class Logic : public QObject
{
    Q_OBJECT

public:
    explicit Logic(QObject *parent = nullptr);
    ~Logic();

public:
    void connectToDatabase();
    void disconnectFromDatabase();
    void createRequest();

    void initMap();
    void initThread();
    void searchDataFromDB();

    QString createSearchCondition(const QStringList &fields);
    QStringList getAllFieldsFromTable(const QString &tableName);
    void nextPage();

    void backPage();
    QSqlQueryModel *getModel() const;
    void stopWorkerThread();

    void processState(QObject *sender, const QString &searchText);
    void setButtonState(QObject *button, State state);
    void executeRequest(const QString &queryString);

signals:
    void updateDB();
    void updateLabel(int currentPage);

private slots:
    void loadNextThreePages();

private:
    int currentPage;
    int pageSize;
    int offset;

private:
    const QString dbFilename = "C:/Qt/projects/BD/123.db";
    std::map<State, std::function<void()>> funcmap;
    std::map<QObject *, State> buttonStateMap;
    QSqlQueryModel *model;
    QString searchText;
    QSqlDatabase db;
    State state;
    QThread workerThread;
    QMutex mutex;
};

#endif // LOGIC_H
