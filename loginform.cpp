#include "loginform.h"

LoginForm::LoginForm(QWidget  *parent)
    : QDialog(parent)
{
    createUI();
}


void LoginForm::createUI()
{
    setupDisplay();
    setupButtons();
    setupLayouts();
}

void LoginForm::setupDisplay()
{
    setWindowTitle("Вход");
    this->resize(350, 300);
}

void LoginForm::setupLayouts()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(cancelButton);
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

QPushButton* LoginForm::createButton(const QString &text, StateButton state)
{
    QPushButton* button = new QPushButton(text, this);
    //connect(button, &QPushButton::clicked, this, &View::s_ButtonClicked);
    return button;
}

