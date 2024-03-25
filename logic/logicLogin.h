#ifndef LOGICLOGIN_H
#define LOGICLOGIN_H

#include <QObject>
#include <QWidget>
#include <functional>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QApplication>
#include <QSqlQueryModel>
#include <QDir>
#include <QStandardItemModel>
#include <QtConcurrent/QtConcurrent>
#include<QSqlQuery>
#include "../databasemanager.h"

enum StateButtonLogin
{
    Login,
    Cancel
};

class LoginLogic : public QObject
{
    Q_OBJECT

public:
    explicit LoginLogic(QObject* parent = nullptr);   
    ~LoginLogic();

    void executeRequest(const QString &queryString);
    void disconnectFromDatabase();
    QString buildQueryString();
    bool connectToDatabase();
    void initMap();
    void initTypes();
    void login();
    void cancel();

public slots:  
    void s_initDB();
    void s_processState(QObject *sender,const QString &login,const QString &password);
    void s_setButtonState(QObject *button, StateButtonLogin state);

signals:
    void authenticationSuccess();
    void authenticationFailed();

private:
    std::map<StateButtonLogin, std::function<void()>> funcMap;
    std::map<QObject*, StateButtonLogin> buttonStateMap;
    StateButtonLogin state;
    QString dbFilename;
    QSqlDatabase db;

    QString userName;
    QString userPasword;
};

#endif // LOGICLOGIN_H
