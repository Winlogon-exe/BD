#ifndef MENUFORM_H
#define MENUFORM_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../forms/viewForm.h"
#include "../logic/logicMenu.h"

class MenuForm : public ViewForm
{
    Q_OBJECT
public:
   explicit MenuForm();
   void createUI()      override;
   void setupConnect()  override;
   void setupDisplay()  override;
   void setupButtons()  override;
   void setupLineEdit() override;
   void setupLayouts()  override;
   void setupLabel()    override;
   QPushButton* createButton(const QString& text,StateButtonMenu state);

signals:
    void requestProcessState(QObject *sender);
    void setState(QObject* sender, StateButtonMenu state);

private:
   LogicMenu logic;

private:
    QPushButton* buttonListUsers;
    QPushButton* buttonListProjects;

public slots:
    void s_buttonClicked();

};

#endif // MENUFORM_H
