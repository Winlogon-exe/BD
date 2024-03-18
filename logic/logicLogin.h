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

enum StateButton
{
    Login,
    Cancel
};

class LoginLogic : public QObject
{
    Q_OBJECT

public:
    explicit LoginLogic(QObject* parent = nullptr);
    void initDB();
    void initMap();
    bool connectToDatabase();
    void executeRequest(const QString &queryString);
    QString buildQueryString();

    void login();
    void cancel();

public slots:  
    void s_processState(QObject *sender,const QString &login,const QString &password);
    void s_setButtonState(QObject *button, StateButton state);

signals:
    void authenticationSuccess();
    void authenticationFailed();

private:
    std::map<StateButton, std::function<void()>> funcMap;
    std::map<QObject*, StateButton> buttonStateMap;
    QString dbFilename;
    StateButton state;
    QSqlDatabase db;

    QString userName;
    QString userPasword;
};

#endif // LOGICLOGIN_H
