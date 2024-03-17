#include "forms/viewForm.h"
#include"forms/loginform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginForm login;
    login.setModal(true);
    login.show();

    // ViewForm v;
    // v.show();
    return a.exec();
}
