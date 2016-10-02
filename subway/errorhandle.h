#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H

#include <QDialog>
#include <QLabel.h>
namespace Ui {
class ErrorHandle;
}

class ErrorHandle : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorHandle(QWidget *parent = 0);
    ~ErrorHandle();
    void SetText(QString a);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ErrorHandle *ui;
    QLabel *label;
};

#endif // ERRORHANDLE_H
