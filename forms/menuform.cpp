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

}

void MenuForm::setupDisplay()
{
    this->resize(1024,768);
}

void MenuForm::setupButtons()
{

}

void MenuForm::setupLabel()
{

}

void MenuForm::setupLayouts()
{
    //QVBoxLayout *layout = new QVBoxLayout;

}

void MenuForm::setupLineEdit()
{

}
