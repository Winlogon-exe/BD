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
    setupLineEdit();
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

void MenuForm::setupLabel()
{

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
    QTabWidget* tabWidget = new QTabWidget;
    layout->addWidget(tabWidget);

}

void MenuForm::s_buttonClicked()
{
    emit requestProcessState(sender());
}

void MenuForm::setupLineEdit()
{

}
