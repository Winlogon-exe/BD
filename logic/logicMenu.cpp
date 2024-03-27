#include"logicMenu.h"

LogicMenu::LogicMenu(QObject* parent):
    QObject(parent), theme_Settings("stil", "stil")
{
    loadThemeFromSettings();
    initMap();
}

void LogicMenu::initMap()
{
    funcMap[Users]    = [this](){ users(); };
    funcMap[Projects] = [this](){ projects(); };
}

void LogicMenu::s_processState(QObject* sender)
{
    state = buttonStateMap[sender];
    auto it = funcMap.find(state);

    if (it != funcMap.end())
    {
        it->second();
    }
}

//тб узнавать передавать в ui?
void LogicMenu::users()
{
    emit openFormUsers();
}

void LogicMenu::projects()
{
    emit openFormProjects();
}

void LogicMenu::s_setButtonState(QObject* button, StateButtonMenu state)
{
    buttonStateMap[button] = state;
}

LogicMenu::~LogicMenu()
{
    qDebug()<<"деструктор LogicMenu";
}

void LogicMenu::loadThemeFromSettings()
{
    QString theme = theme_Settings.value("theme", lightTheme).toString(); // Значение по умолчанию - "light"
    if (theme == darkTheme)
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

void LogicMenu::s_toggleTheme()
{
    //?
    isDarkTheme = !isDarkTheme;
    if (isDarkTheme)
    {
        s_blackTheme();
        //themeAction->setText(tr("Светлая тема"));
        theme_Settings.setValue("theme", darkTheme);
    }
    else
    {
        s_lightTheme();
       // themeAction->setText(tr("Темная тема"));
        theme_Settings.setValue("theme", lightTheme);
    }
}

void LogicMenu::applyTheme(const QString &themeFilePath)
{
    QFile file(themeFilePath);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        QString stylesheet = stream.readAll();
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setStyleSheet(stylesheet);
        file.close();
    }
}

void LogicMenu::s_lightTheme()
{
    applyTheme("./styles/lighttheme.qss");
}

void LogicMenu::s_blackTheme()
{
    applyTheme("./styles/darktheme.qss");
}
