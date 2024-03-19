#include "menuform.h"

MenuForm::MenuForm()
{
    setupConnect();
    createUI();
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
}

void MenuForm::setupDisplay()
{
    this->setWindowTitle("Меню");
    this->resize(1024,768);
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

    view = new ViewForm();
    tabWidget->addTab(view, "Список сотрудников");
    tabWidget->setVisible(false);
    tabWidget->setTabsClosable(true);

}


void MenuForm::s_buttonClicked()
{
    emit requestProcessState(sender());
    QObject* senderButton = sender();
    if(senderButton == buttonListUsers)
    {
        tabWidget->setCurrentIndex(0); // Переключение на вкладку пользователей
    }
    else if(senderButton == buttonListProjects)
    {
        tabWidget->setCurrentIndex(1); // Переключение на вкладку проектов
    }
    tabWidget->setVisible(true); // Показываем tabWidget, если он был скрыт
}

void MenuForm::update()
{


}

void MenuForm::setupLabel()
{

}
