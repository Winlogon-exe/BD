#ifndef MENUFORM_H
#define MENUFORM_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include<QMenuBar>
#include<QFileDialog>
#include<QStyleFactory>
#include<QCursor>

#include "viewForm.h"
#include "../logic/logicMenu.h"

class MenuForm : public QWidget
{
    Q_OBJECT
public:
   explicit MenuForm(QWidget *parent = nullptr);
   ~MenuForm();

   void createUI();
   void iniThread();
   void setupConnect();
   void setupDisplay();
   void setupButtons();
   void setupLayouts();
   void setupLabel();
   void setupMenuBar();

   int findTabIndexByWidget(const QWidget* widget) const;
   QPushButton* createButton(const QString& text,StateButtonMenu state);

signals:
    void requestProcessState(QObject *sender);
    void setState(QObject* sender, StateButtonMenu state);

private:
    ViewForm* users;
    ViewForm* projects;
    LogicMenu logic;

private:
    QPushButton* buttonListUsers;
    QPushButton* buttonListProjects;

public slots:
    void s_buttonClicked();
    void s_updateUsers();
    void s_updateProjects();
    void s_closeTab(int index);
    void s_open();
    void s_openAboutDialog();
    void s_toggleTheme();
    void s_blackTheme();
    void s_lightTheme();

private:
    QTabWidget *Tab;
    QMenuBar *menuBar;
    QString lastDatabasePath;
    QAction *themeAction;
    bool isDarkTheme = false;
    QSettings settings;


};

#endif // MENUFORM_H
