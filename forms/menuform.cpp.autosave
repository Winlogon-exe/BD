#include "menuform.h"

MenuForm::MenuForm(QWidget *parent)
    : QWidget(parent)
{
    loadThemeFromSettings();
    setupConnect();
    createUI();
}


void MenuForm::iniThread()
{

}

void MenuForm::loadThemeFromSettings()
{
    QSettings settings("stil", "stil"); // Измените на имя вашей компании и приложения
    QString theme = settings.value("theme", "light").toString(); // Значение по умолчанию - "light"

    if (theme == "dark")
    {
        s_blackTheme();
        isDarkTheme = true;
    }
    else
    {
        s_lightTheme();
        isDarkTheme = false;
    }
}

void MenuForm::createUI()
{
    iniThread();
    setupDisplay();
    setupButtons();
    setupMenuBar();
    setupLabel();
    setupLayouts();
}

void MenuForm::setupMenuBar()
{
    menuBar = new QMenuBar(this);

    // Меню "Файл"
    QMenu *fileMenu = menuBar->addMenu(tr("Файл"));
    QAction *openAction = fileMenu->addAction(tr("Открыть"));
    QAction *exitAction = fileMenu->addAction(tr("Выход"));
    connect(openAction, &QAction::triggered, this, &MenuForm::s_open);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Меню "Вид"
    QMenu *viewMenu = menuBar->addMenu(tr("Вид"));
    QAction *viewUsersAction = viewMenu->addAction(tr("Список сотрудников"));
    QAction *viewProjectsAction = viewMenu->addAction(tr("Список проектов"));
    connect(viewUsersAction, &QAction::triggered, this, &MenuForm::s_updateUsers);
    connect(viewProjectsAction, &QAction::triggered, this, &MenuForm::s_updateProjects);

    // Меню "Настройки"
    QMenu *settingsMenu = menuBar->addMenu(tr("Настройки"));
    themeAction = settingsMenu->addAction(tr("Темная тема"), this, &MenuForm::s_toggleTheme);

    // Меню "Справка"
    QMenu *helpMenu = menuBar->addMenu(tr("Справка"));
    QAction *aboutAction = helpMenu->addAction(tr("О программе"));
    connect(aboutAction, &QAction::triggered, this, &MenuForm::s_openAboutDialog);
}

void MenuForm::s_blackTheme()
{
    // Создаем новую палитру для темной темы
    QPalette darkPalette;

    // Цвет фона
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));

    // Цвет виджетов
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);

    // Цвета для выделения (например, выделенный текст)
    darkPalette.setColor(QPalette::Highlight, QColor(42,130,218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::LinkVisited, QColor(42, 130, 218).darker());

    qApp->setPalette(darkPalette);
    qApp->setStyle(QStyleFactory::create("Fusion"));
}

void MenuForm::s_lightTheme()
{
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, Qt::white);
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, Qt::lightGray);
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, Qt::white);
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    lightPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);

    qApp->setPalette(lightPalette);
    qApp->setStyle(QStyleFactory::create("Fusion"));
}

void MenuForm::s_toggleTheme()
{
    isDarkTheme = !isDarkTheme;
    QSettings settings("stil", "stil");

    if (isDarkTheme)
    {
        s_blackTheme();
        themeAction->setText(tr("Светлая тема"));
        settings.setValue("theme", "dark");
    }
    else
    {
        s_lightTheme();
        themeAction->setText(tr("Темная тема"));
        settings.setValue("theme", "light");
    }
}

void MenuForm::s_openAboutDialog()
{
    // Здесь код для открытия диалогового окна "О программе"

}


void MenuForm::s_open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть базу данных"), "", tr("Базы данных (*.db);;Все файлы (*.*)"));
    if (!fileName.isEmpty())
    {
        //логика после открытия файла
        lastDatabasePath = fileName;
    }
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
    this->resize(1024,768);
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

    layout->addWidget(Tab,1);
    layout->addStretch();
    Tab->setVisible(false);

    layout->setMenuBar(menuBar);
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
        Tab->setCurrentIndex(findTabIndexByWidget(users));
        return;
    }

    // Используем lastDatabasePath вместо "client.db"
    users = new ViewForm("client.db", "users");

    int newIndex = Tab->addTab(users, tr("Список сотрудников"));
    Tab->setCurrentIndex(newIndex);
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
    int newIndex = Tab->addTab(projects, "Список проектов");
    Tab->setCurrentIndex(newIndex);
    Tab->setTabsClosable(true);
    Tab->setVisible(true);
}

//bool?
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
