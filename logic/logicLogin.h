#ifndef LOGICLOGIN_H
#define LOGICLOGIN_H

#include <QObject>
#include <QWidget>
#include <functional>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQueryModel>

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
    void initMap();
    void login();
    void cancel();

public slots:  
    void s_processState(QObject *sender);
    void s_setButtonState(QObject *button, StateButton state);

private:
    std::map<StateButton, std::function<void()>> funcMap;
    std::map<QObject*, StateButton> buttonStateMap;
    StateButton state;
    QSqlDatabase db;
};

#endif // LOGICLOGIN_H
