#include "InterfaceDSPIC.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InterfaceDSPIC w;
    w.show();

    return a.exec();
}
