#include "view.h"
#include"loginform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginForm login;
    login.show();
//    View w;
//    w.show();
    return a.exec();
}
