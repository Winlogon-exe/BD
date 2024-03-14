#include "loginform.h"

LoginForm::LoginForm(QWidget  *parent)
    : QDialog(parent)
{
    setupButtons();
}


void LoginForm::createUI()
{

}

void LoginForm::setupDisplay()
{

}

void LoginForm::setupLayouts()
{

}

void LoginForm::setupButtons()
{
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = createButton("Вход",Login);
    cancelButton = createButton("Отмена",Cancel);
}

void LoginForm::setupConnect()
{

}

QPushButton* LoginForm::createButton(const QString &text, State state)
{
    QPushButton* button = new QPushButton(text, this);
    //connect(button, &QPushButton::clicked, this, &View::s_ButtonClicked);
    return button;
}

