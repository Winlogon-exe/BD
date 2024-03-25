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
    Tab = new QTabWidget(this);
    connect(Tab, &QTabWidget::tabCloseRequested, this, &MenuForm::s_closeTab);

    layout->addWidget(Tab);
    layout->addStretch();
    Tab->setVisible(false);

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
    if (findTabIndexByWidget(users) != -1)
    {
       // Если вкладка с пользователем уже существует, переключаемся на неё
       Tab->setCurrentIndex(findTabIndexByWidget(users));
       return;
    }

    users = new ViewForm("client.db","users");

    Tab->addTab(users, "Список сотрудников");
    Tab->setTabsClosable(true);
    Tab->setVisible(true);
}

void MenuForm::s_closeTab(int index)
{
    QWidget* tab = Tab->widget(index);
    if (tab)
    {
        Tab->removeTab(index);
        delete tab;
    }

    if (Tab->count() == 0)
    {
        Tab->setVisible(false); // Скрываем, только если нет вкладок
    }
}

void MenuForm::s_updateProjects()
{
    if (findTabIndexByWidget(projects) != -1)
    {
       // Если вкладка с пользователем уже существует, переключаемся на неё
       Tab->setCurrentIndex(findTabIndexByWidget(projects));
       return;
    }

    projects = new ViewForm("123.db","popular_tracks");
    Tab->addTab(projects, "Список проектов");
    Tab->setTabsClosable(true);
    Tab->setVisible(true);
}

int MenuForm::findTabIndexByWidget(const QWidget* widget) const
{
    for (int i = 0; i < Tab->count(); ++i)
    {
        if (Tab->widget(i) == widget)
        {
            return i;
        }
    }
    return -1; // Вкладка не найдена
}

void MenuForm::setupLabel()
{

}

MenuForm::~MenuForm()
{
    qDebug()<<"деструктор MenuForm";
}
