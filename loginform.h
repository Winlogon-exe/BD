#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QObject>
#include <QWidget>
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QLabel>
#include"loginlogic.h"

class LoginForm :public QDialog
{
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
    void requestProcessState(QObject *sender);
    void setState(QObject* button);

public slots:
    void s_ButtonClicked();

private:
    QLabel *nameFormLabel;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;

    QPushButton* loginButton;
    QPushButton* cancelButton;
};

#endif // LOGINFORM_H
