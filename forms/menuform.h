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
   explicit MenuForm();
   void createUI();
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
    std::unique_ptr<ViewForm> view;
    LogicMenu logic;

private:
    QPushButton* buttonListUsers;
    QPushButton* buttonListProjects;

public slots:
    void s_buttonClicked();
    void updateUsers();
    void updateProjects();
    void closeTab(int index);

private:
    QTabWidget *tabWidget;

};

#endif // MENUFORM_H
