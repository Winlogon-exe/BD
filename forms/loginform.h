#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QObject>
#include <QWidget>
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QLabel>
#include"../forms/menuform.h"
#include"../logic/logicLogin.h"

class LoginForm :public QDialog
{
   Q_OBJECT
public:
   explicit LoginForm(QDialog *parent = nullptr);

   void createUI() ;
   void setupDisplay() ;
   void setupButtons() ;
   void setupLayouts() ;
   void setupConnect() ;
   void setupLineEdit();
   void setupLabel();
   QPushButton* createButton(const QString& text,StateButton state);

signals:
    void requestProcessState(QObject *sender,const QString &login,const QString &password) ;
    void setState(QObject* button,StateButton state);

public slots:
    void s_ButtonClicked();
    void s_openNextForm();

private:
    QLabel *nameFormLabel;
    QLabel *passwordLabel;
    QLabel *loginLabel;

private:
    LoginLogic logic;
    std::unique_ptr<MenuForm> menu;

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

private:
    QPushButton* loginButton;
    QPushButton* cancelButton;
};

#endif // LOGINFORM_H
