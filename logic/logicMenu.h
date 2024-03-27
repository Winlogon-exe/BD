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
#include<QStyleFactory>

//enum Theme
//{
//    Light,
//    Dark
//};

enum StateButtonMenu
{
    Users,
    Projects,
};

class LogicMenu:public QObject
{
    Q_OBJECT
public:
   explicit LogicMenu(QObject* parent = nullptr);
    ~LogicMenu();

   void initMap();
   void users();
   void projects();

signals:
   void openFormUsers();
   void openFormProjects();

public slots:
    void s_setButtonState(QObject* sender, StateButtonMenu state);
    void applyTheme(const QString &themeFilePath);
    void s_processState(QObject* sender);
    void loadThemeFromSettings();
    void s_blackTheme();
    void s_lightTheme();
    void s_toggleTheme();

private:
    std::map<StateButtonMenu, std::function<void()>> funcMap;
    std::map<QObject*, StateButtonMenu> buttonStateMap;

private:
    bool isDarkTheme = false;
    QSettings theme_Settings;
    StateButtonMenu state;

private:
    QString lightTheme = "light";
    QString darkTheme = "dark";
};


#endif // MENUFORM_H
