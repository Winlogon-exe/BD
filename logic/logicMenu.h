#ifndef LOGICMENU_H
#define LOGICMENU_H

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

public slots:
    void s_setButtonState(QObject* sender, StateButtonMenu state);

};


#endif // MENUFORM_H
