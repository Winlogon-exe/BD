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
#include"../forms/viewForm.h"

class LoginForm :public ViewForm
{
   Q_OBJECT

public:
   explicit LoginForm();

   void createUI()      override;
   void iniThread()     override;
   void setupDisplay()  override;
   void setupButtons()  override;
   void setupLayouts()  override;
   void setupConnect()  override;
   void setupLineEdit() override;
   void setupLabel()    override;
   QPushButton* createButton(const QString& text,StateButtonLogin state);

signals:
    void requestProcessState(QObject *sender,const QString &login,const QString &password) ;
    void setState(QObject* button,StateButtonLogin state);

public slots:
    void s_ButtonClicked();
    void s_openNextForm();
    void s_unknownUser();

private:
    QLabel *nameFormLabel;
    QLabel *passwordLabel;
    QLabel *loginLabel;

private:
    std::unique_ptr<MenuForm> menu;
    LoginLogic logic;

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

private:
    QPushButton* loginButton;
    QPushButton* cancelButton;

private:
    QString appDir = QCoreApplication::applicationDirPath();
    QString iconPath = QDir::toNativeSeparators(appDir + "/icons/icon.png");
};

#endif // LOGINFORM_H
