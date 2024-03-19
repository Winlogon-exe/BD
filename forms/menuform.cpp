#include "menuform.h"

MenuForm::MenuForm()
{
    setupConnect();
    createUI();
}

void MenuForm::createUI()
{
    setupDisplay();
    setupLayouts();
    setupButtons();
    setupLabel();
    setupLineEdit();
}

void MenuForm::setupConnect()
{
    connect(this, &MenuForm::setState, &logic, &LogicMenu::s_setButtonState);
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
//    QVBoxLayout* layout = new QVBoxLayout(this);

//    // Создание виджета с вкладками
//    QTabWidget* tabWidget = new QTabWidget;
//    layout->addWidget(tabWidget);

//    // Добавление вкладок
//    tabWidget->addTab(new QLabel("Содержимое вкладки Сотрудники"), "Сотрудники");
//    tabWidget->addTab(new QLabel("Содержимое вкладки Проекты"), "Проекты");
//    // Здесь можно добавить больше вкладок

//    // Создание кнопок для переключения вкладок
//    layout->addWidget(buttonListUsers);
//    layout->addWidget(buttonListProjects);

//    // Соединение сигналов нажатия кнопок с слотами для переключения вкладок
//    connect(buttonListUsers, &QPushButton::clicked, [tabWidget]() { tabWidget->setCurrentIndex(0); });
//    connect(buttonListProjects, &QPushButton::clicked, [tabWidget]() { tabWidget->setCurrentIndex(1); });

}

void MenuForm::s_buttonClicked()
{

}

void MenuForm::setupLineEdit()
{

}
