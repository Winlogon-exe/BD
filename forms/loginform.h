#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QObject>
#include <QWidget>
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QLabel>
#include"../logic/logicLogin.h"

class LoginForm :public QDialog
{
   Q_OBJECT
public:
   explicit LoginForm(QDialog *parent = nullptr);

   void createUI();
   void setupDisplayMain();
   void setupButtons();
   void setupLineEdit();
   void setupLabel();
   void setupLayouts();
   void setupConnect();
   QPushButton* createButton(const QString& text,StateButton state);

signals:
<<<<<<< Updated upstream:forms/loginform.h
    void requestProcessState(QObject *sender,const QString &login,const QString &password);
    void setState(QObject* button,StateButton state);
=======
    void requestProcessState(QObject *sender);
    void setState(QObject* button, StateButton state);
>>>>>>> Stashed changes:loginform.h

public slots:
    void s_ButtonClicked();

private:
    QLabel *nameFormLabel;
    QLabel *passwordLabel;
    QLabel *loginLabel;

private:
    LoginLogic logic;

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

private:
    QPushButton* loginButton;
    QPushButton* cancelButton;
};

#endif // LOGINFORM_H
