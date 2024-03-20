#ifndef LOGICMENU_H
#define LOGICMENU_H

#include <QObject>
#include <QWidget>
#include <functional>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QApplication>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QtConcurrent/QtConcurrent>
#include<QSqlQuery>
#include <QDir>

enum StateButtonMenu
{
    Users,
    Projects
};

class LogicMenu:public QObject
{
    Q_OBJECT
public:
   explicit LogicMenu();
   void initMap();
   void users();
   void projects();

signals:
   void openFormUsers();
   void openFormProjects();

public slots:
    void s_setButtonState(QObject* sender, StateButtonMenu state);
    void s_processState(QObject* sender);

private:
    std::map<StateButtonMenu, std::function<void()>> funcMap;
    std::map<QObject*, StateButtonMenu> buttonStateMap;
};


#endif // MENUFORM_H
