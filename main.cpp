#include "forms/viewForm.h"
#include "forms/loginform.h"
#include "forms/menuform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginForm login;
    login.show();
    return a.exec();
}
