#include "menuform.h"

MenuForm::MenuForm(QWidget *parent)
    : QWidget(parent)
{
    setupConnect();
    createUI();
}

void MenuForm::iniThread()
{

}

void MenuForm::createUI()
{
    iniThread();
    setupDisplay();
    setupButtons();
    setupLabel();
    setupLayouts();
}

void MenuForm::setupConnect()
{
    connect(this, &MenuForm::setState, &logic, &LogicMenu::s_setButtonState);
    connect(this, &MenuForm::requestProcessState, &logic, &LogicMenu::s_processState);

    connect(&logic, &LogicMenu::openFormUsers, this, &MenuForm::s_updateUsers);
    connect(&logic, &LogicMenu::openFormProjects, this, &MenuForm::s_updateProjects);
}

void MenuForm::setupDisplay()
{
    this->setWindowTitle("Меню");
    //1024,768
    this->resize(500,450);
}

void MenuForm::setupButtons()
{
    buttonListUsers = createButton("Список сотрудников",Users);
    buttonListProjects = createButton("Список проектов",Projects);
}

QPushButton* MenuForm::createButton(const QString& text,StateButtonMenu state)
{
    QPushButton* button = new QPushButton(text);
    emit setState(button,state);
    connect(button, &QPushButton::clicked, this, &MenuForm::s_buttonClicked);
    return button;
}

void MenuForm::setupLayouts()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    usersTabWidget = new QTabWidget;
    projectsTabWidget = new QTabWidget;

    layout->addWidget(usersTabWidget);
    layout->addStretch();
    usersTabWidget->setVisible(false);

    layout->addWidget(buttonListUsers);
    layout->addWidget(buttonListProjects);
    layout->addStretch();
}

void MenuForm::s_buttonClicked()
{
    emit requestProcessState(sender());
}

//здесь можно принимать роль от логики и вызывать доп функ. от формы
void MenuForm::s_updateUsers()
{
    // Проверяем, существует ли вкладка с таким же содержимым
    for (int i = 0; i < usersTabWidget->count(); ++i)
    {
        if (usersTabWidget->widget(i) == users)
        {
            usersTabWidget->setCurrentIndex(i);
            return;
        }
    }
    users = new ViewForm("client.db","users");
    usersTabWidget->addTab(users, "Список сотрудников");
    usersTabWidget->setTabsClosable(true);
    usersTabWidget->setVisible(true);
    connect(usersTabWidget, &QTabWidget::tabCloseRequested, this, &MenuForm::s_closeTab);
}

void MenuForm::s_closeTab(int index)
{
    QWidget* tab = usersTabWidget->widget(index);
    if (tab)
    {
        usersTabWidget->removeTab(index);
        usersTabWidget->setVisible(false);
        delete tab;
    }
}

void MenuForm::s_updateProjects()
{
    // Проверяем, существует ли вкладка с таким же содержимым
    for (int i = 0; i < projectsTabWidget->count(); ++i)
    {
        if (projectsTabWidget->widget(i) == projects)
        {
            projectsTabWidget->setCurrentIndex(i);
            return;
        }
    }
    projects = new ViewForm("123.db","popular_tracks");
    projectsTabWidget->addTab(projects, "Список проектов");
    projectsTabWidget->setTabsClosable(true);
    projectsTabWidget->setVisible(true);
}

void MenuForm::setupLabel()
{

}

MenuForm::~MenuForm()
{
    qDebug()<<"деструктор MenuForm";
}
