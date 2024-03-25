#include "menuform.h"

MenuForm::MenuForm(QWidget *parent)
    : QWidget(parent)
{
    setupConnect();
    createUI();
}

void MenuForm::createUI()
{
    //iniThread();
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
    tabWidget = new QTabWidget;
    layout->addWidget(tabWidget);
    layout->addStretch();
    tabWidget->setVisible(false);

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
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        if (tabWidget->widget(i) == users)
        {
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    // Если такой вкладки еще нет, создаем новую
    users = new ViewForm("client.db");
    //signal?
    users->createUI();

    tabWidget->addTab(users, "Список сотрудников");
    tabWidget->setTabsClosable(true);
    tabWidget->setVisible(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MenuForm::s_closeTab);
}

void MenuForm::s_closeTab(int index)
{
    QWidget* tab = tabWidget->widget(index);
    if (tab)
    {
        tabWidget->removeTab(index);
        tabWidget->setVisible(false);
        delete tab;
    }
}

void MenuForm::s_updateProjects()
{
    //Сделать конструктор по имени БД
  /*  projects = new ViewForm;
    projects->createUI();
    tabWidget->addTab(projects, "Список проектов");
    tabWidget->setVisible(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MenuForm::s_closeTab)*/;
}

void MenuForm::setupLabel()
{

}

MenuForm::~MenuForm()
{
    qDebug()<<"деструктор MenuForm";
}
