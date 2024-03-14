#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QObject>
#include <QWidget>
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>
#include"loginlogic.h"


class LoginForm :public QDialog
{
public:
   explicit LoginForm(QWidget *parent = nullptr);

   void createUI();
   void setupDisplay();
   void setupButtons();
   void setupLayouts();
   void setupConnect();
   QPushButton* createButton(const QString& text,State state);

signals:
    void requestProcessState(QObject *sender);
    void setState(QObject* button);

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

    QPushButton* loginButton;
    QPushButton* cancelButton;
};

#endif // LOGINFORM_H
