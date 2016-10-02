#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#define MAPPATH ../beijing-subway.txt
int main(int argc, char *argv[])
{
    if(argc == 2){
        string order = string(argv[1]);
        if(order == "-g"){
            QApplication a(argc, argv);
            MainWindow w;
            w.show();
           // while(true){
             //   QThread::msleep(100);
              //  w.repaint();
           // }
            return a.exec();
        }else{
            //run the main window
            cout << "Wrong Order!"<<endl;
        }
    }

}
