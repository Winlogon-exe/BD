#include "loginform.h"

LoginForm::LoginForm()
{
    setupConnect();
    createUI();
}

void LoginForm::createUI()
{
    iniThread();
    setupDisplay();
    setupLabel();
    setupLineEdit();
    setupButtons();
    setupLayouts();
}

void LoginForm::iniThread()
{
    logicThread = new QThread(this);
    logic.moveToThread(logicThread);

    connect(logicThread, &QThread::started, &logic, &LoginLogic::s_initDB);
    connect(logicThread, &QThread::finished, logicThread, &QThread::deleteLater);
    logicThread->start();
}

void LoginForm::setupDisplay()
{
    setWindowTitle("Вход");
    QIcon icon(iconPath);
    this->setWindowIcon(icon);
    this->resize(350, 300);
    this->setMinimumSize(200,200);
}

void LoginForm::setupLabel()
{
    nameFormLabel = new QLabel("Вход");
    loginLabel    = new QLabel("Логин");
    passwordLabel = new QLabel("Пароль");

    QFont font = nameFormLabel->font();
    font.setPointSize(18);
    nameFormLabel->setFont(font);
}

void LoginForm::setupLineEdit()
{
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
}

void LoginForm::setupLayouts()
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(nameFormLabel, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(loginLabel, 1, 0);

    layout->addWidget(usernameEdit, 1, 1);
    layout->addWidget(passwordEdit, 2, 1);
    layout->addWidget(passwordLabel, 2, 0);

    layout->addWidget(loginButton, 3, 0, 1, 2);
    layout->addWidget(cancelButton, 4, 0, 1, 2);
}

void LoginForm::setupButtons()
{
    loginButton = createButton("Вход",Login);
    cancelButton = createButton("Отмена",Cancel);
}

void LoginForm::setupConnect()
{
    connect(this, &LoginForm::setState, &logic, &LoginLogic::s_setButtonState);
    connect(this, &LoginForm::requestProcessState, &logic, &LoginLogic::s_processState);

    connect(&logic, &LoginLogic::authenticationSuccess, this, &LoginForm::s_openNextForm);
    connect(&logic, &LoginLogic::authenticationFailed, this, &LoginForm::s_unknownUser);
}

QPushButton* LoginForm::createButton(const QString &text,StateButtonLogin state)
{
    QPushButton* button = new QPushButton(text, this);
    emit setState(button,state);
    connect(button, &QPushButton::clicked, this, &LoginForm::s_ButtonClicked);
    return button;
}

//передача в логику
void LoginForm::s_ButtonClicked()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    emit requestProcessState(sender(),username,password);
}

void LoginForm::s_openNextForm()
{
    menu.show();
    this->close();
}

void LoginForm::s_unknownUser()
{
    QMessageBox::information(nullptr, "Ошибка", "Пользователь не найден.");
}

LoginForm::~LoginForm()
{
    qDebug()<<"деструктор LoginForm";
    logicThread->quit();
    logicThread->wait();
}
