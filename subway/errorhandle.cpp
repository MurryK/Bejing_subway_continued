#include "errorhandle.h"
#include "ui_errorhandle.h"

ErrorHandle::ErrorHandle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorHandle)
{
    ui->setupUi(this);
}

ErrorHandle::~ErrorHandle()
{
    delete ui;
}
void ErrorHandle::SetText(QString a){
    label=new QLabel(this);
    label->setText(a);
    label->setGeometry(QRect(10, 5, 200, 30));
    label->setAlignment(Qt::AlignHCenter);
}

void ErrorHandle::on_pushButton_clicked()
{
    this->deleteLater();
}
