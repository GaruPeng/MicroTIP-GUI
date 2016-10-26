#ifndef INTERFACEDSPIC_H
#define INTERFACEDSPIC_H

#include <QDialog>

namespace Ui {
class InterfaceDSPIC;
}

class InterfaceDSPIC : public QDialog
{
    Q_OBJECT

public:
    explicit InterfaceDSPIC(QWidget *parent = 0);
    ~InterfaceDSPIC();

private:
    Ui::InterfaceDSPIC *ui;
};

#endif // INTERFACEDSPIC_H
