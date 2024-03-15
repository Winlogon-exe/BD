#include "loginform.h"

LoginForm::LoginForm(QDialog  *parent)
    : QDialog(parent)
{
    createUI();
}

void LoginForm::createUI()
{
    setupDisplayMain();
    setupLabel();
    setupLineEdit();
    setupButtons();
    setupLayouts();
}

void LoginForm::setupDisplayMain()
{
    setWindowTitle("Вход");
    this->resize(350, 300);
    this->setMinimumSize(200,200);
}

void LoginForm::setupLabel()
{
    nameFormLabel = new QLabel("Вход");
    nameFormLabel->setAlignment(Qt::AlignCenter);
}

void LoginForm::setupLineEdit()
{
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
}

void LoginForm::setupLayouts()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(nameFormLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(cancelButton);
}

void LoginForm::setupButtons()
{
    loginButton = createButton("Вход",Login);
    cancelButton = createButton("Отмена",Cancel);
}

void LoginForm::setupConnect()
{

}

QPushButton* LoginForm::createButton(const QString &text, StateButton state)
{
    QPushButton* button = new QPushButton(text, this);
    emit setState(button,state);
    connect(button, &QPushButton::clicked, this, &LoginForm::s_ButtonClicked);
    return button;
}

//передача в логику
void LoginForm::s_ButtonClicked()
{
    emit requestProcessState(sender());
}
