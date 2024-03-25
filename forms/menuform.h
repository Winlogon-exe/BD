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

private:
    QTabWidget *usersTabWidget;
    QTabWidget* projectsTabWidget;

};

#endif // MENUFORM_H
