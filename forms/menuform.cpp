#include "menuform.h"

MenuForm::MenuForm()
{
    createUI();
    setupConnect();
}

void MenuForm::createUI()
{
    setupDisplay();
    setupButtons();
    setupLabel();
    setupLayouts();
}

void MenuForm::setupConnect()
{
    connect(this, &MenuForm::setState, &logic, &LogicMenu::s_setButtonState);
    connect(this, &MenuForm::requestProcessState, &logic, &LogicMenu::s_processState);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MenuForm::closeTab);

    connect(&logic, &LogicMenu::openFormUsers, this, &MenuForm::updateUsers);
    connect(&logic, &LogicMenu::openFormProjects, this, &MenuForm::updateProjects);
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
    QPushButton* button = new QPushButton(text, this);
    emit setState(button,state);
    connect(button, &QPushButton::clicked, this, &MenuForm::s_buttonClicked);
    return button;
}

void MenuForm::setupLayouts()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    tabWidget = new QTabWidget;
    layout->addWidget(tabWidget);
    layout->addWidget(buttonListUsers);
    layout->addWidget(buttonListProjects);
}

void MenuForm::s_buttonClicked()
{
    emit requestProcessState(sender());
}

//здесь можно принимать роль от логики и вызывать доп функ. от формы
void MenuForm::updateUsers()
{
    // Проверяем, существует ли вкладка с таким же содержимым
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        if (tabWidget->widget(i) == view)
        {
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    // Если такой вкладки еще нет, создаем новую
    view = new ViewForm();
    view->createUI();

    tabWidget->addTab(view, "Список сотрудников");
    tabWidget->setTabsClosable(true);
    tabWidget->setVisible(true);

}

void MenuForm::closeTab(int index)
{
    QWidget* tab = tabWidget->widget(index);
    if (tab)
    {
        tabWidget->removeTab(index);
    }
}

void MenuForm::updateProjects()
{


}

void MenuForm::setupLabel()
{

}
