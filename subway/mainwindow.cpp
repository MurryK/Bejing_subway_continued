#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <qstring.h>
#include <QTextCodec>
#define ButtonWidth 10
#define MOVESTANDARD 10
#define ZOOMSTANDARD 1
#define ZOOMMAX 10
#define ZOOMMIN 0
DWORD WINAPI Thread(LPVOID lpParam)
{
    MainWindow *n = (MainWindow*)lpParam;
    n->printPath();
    return 0;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    if(!buttonimage.load(":/button/button.jpg"))
        cout<<"image fail"<<endl;
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    //ui->menuBar->setFixedHeight();
   // ui->groupBox->setWindowFlags(Qt::WindowStaysOnTopHint);
    //this->resize(800,1000);
    ui->setupUi(this);
    area=new PaintWidget(this);
    zoom=zoom_last=1;

    //area->resize(QSize(800,500));
   // this->setCentralWidget(area);
    ui->verticalLayout->addWidget(area);

    //buttons in the left side of window
    QPixmap butImage;


    //zoom button:
    butImage.load(":/button/add_button.png");
    ui->butZoomIn->setText("");
    ui->butZoomIn->setStyleSheet("QPushButton{border:0px;}");
    ui->butZoomIn->setIcon(butImage);
    ui->butZoomIn->setIconSize(butImage.size());
    ui->butZoomIn->resize(butImage.size());

    butImage.load(":/button/sub_button.png");
    ui->butZoomOut->setText("");
    ui->butZoomOut->setStyleSheet("QPushButton{border:0px;}");
    ui->butZoomOut->setIcon(butImage);
    ui->butZoomOut->setIconSize(butImage.size());
    ui->butZoomOut->resize(butImage.size());


    butImage.load(":/button/lens.jpg");
    ui->butFindWay->setText("");
    ui->butFindWay->setStyleSheet("QPushButton{border:0px;}");
    ui->butFindWay->setIcon(butImage);
    ui->butFindWay->setIconSize(butImage.size());
    ui->butFindWay->resize(butImage.size());
    //input line edit



    QMenu *file = menuBar()->addMenu(tr("File"));
    QMenu *action=menuBar()->addMenu(tr("Action"));
    QMenu *help=menuBar()->addMenu(tr("Help"));
    QAction *openAction= new QAction(tr("&Import"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Import your map."));
    connect(openAction,SIGNAL(triggered()),this,SLOT(Import_clicked()));
    file->addAction(openAction);
    QAction *helpMenu=new QAction(tr("About.."),this);
    helpMenu->setStatusTip(tr("About this soft."));
    help->addAction(helpMenu);
    QAction *zoom_in=new QAction(tr("Zoom in"),this);
    zoom_in->setStatusTip(tr("Enlarge"));
    connect(zoom_in,SIGNAL(triggered()),this,SLOT(Zoom_in()));
    action->addAction(zoom_in);
    QAction *zoom_out=new QAction(tr("Zoom out"),this);
    zoom_out->setStatusTip(tr("Reduce"));
    connect(zoom_out,SIGNAL(triggered()),this,SLOT(Zoom_out()));
    action->addAction(zoom_out);
    city=NULL;
    start=end=-1;
    QAction *shortest=new QAction("ShortestPath",this);
    action->setStatusTip("Print the shortestpath between two points you had clicked.");
    connect(shortest,SIGNAL(triggered()),this,SLOT(ShortestPath()));
    action->addAction(shortest);
    QAction *change=new QAction("ShortestChange",this);
    change->setStatusTip("Print the shortest-change-path between two points you had clicked.");
    connect(change,SIGNAL(triggered()),this,SLOT(ShortestChange()));
    action->addAction(change);
    connect(this,SIGNAL(send()),this,SLOT(got()));

    nowbutton=NULL;
    //Import_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        lastpoint=event->pos();
        endpoint=event->pos();
    }
}
void MainWindow::wheelEvent(QWheelEvent * event){
    if(event->delta()>0)
        Zoom_in();
    else
        Zoom_out();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons()&Qt::LeftButton){
        endpoint=event->pos();
        positionx=positionx+endpoint.rx()-lastpoint.rx();
        positiony=positiony+endpoint.ry()-lastpoint.ry();
//        if(((endpoint.rx()-lastpoint.rx())*(endpoint.rx()-lastpoint.ry())+(endpoint.ry()-lastpoint.ry())*(endpoint.ry()-lastpoint.ry()))<100)
//            return;
        lastpoint=endpoint;
        repaint();
    }
}
void MainWindow::repaint(){
    if(city==NULL)
        return;
    int i,j,k,len;
    len=list.size();
    QPushButton *button;
    QLabel *label;
    if(zoom!=zoom_last){
    for(i=0;i<len;i++){
        button=list.back();
        button->deleteLater();
       // label=labellist.back();
       // label->deleteLater();
        list.pop_back();
        nolist.pop_back();
      //  labellist.pop_back();
    }
    }

    Point p,q;
    if(zoom==zoom_last){
        for(i=0;i<len;i++){
            button=list.at(i);
            k=nolist.at(i);
            p=city->getloc(k);
            button->move(zoom*p.x+positionx-ButtonWidth*zoom/2,zoom*p.y+positiony-ButtonWidth*zoom/2);
        }
    }
    area->clear();
    QString name;
    buttonimage=buttonimage.scaled(zoom*ButtonWidth,zoom*ButtonWidth);
    int *sta,add;
    int sublinenum;
    sublinenum=city->getlinenum();
    i=0;

    for(i=0;i<sublinenum;i++){
        sta=city->getline(i);
        for(j=0,add=1;sta[j]!=-1;j++){
            p=city->getloc(sta[j]);
            if(zoom!=zoom_last){
            button=new QPushButton("",area);
            if(add==0&&(zoom*p.x+positionx+ButtonWidth*zoom/2)<0||(positiony+zoom*p.y+ButtonWidth*zoom/2)<20)
                   continue;
            button->setGeometry(zoom*p.x+positionx-ButtonWidth*zoom/2,zoom*p.y+positiony-ButtonWidth*zoom/2,ButtonWidth*zoom,ButtonWidth*zoom);

            button->clearMask();
            button->setBackgroundRole( QPalette::Base);
            button->setMask(buttonimage.createHeuristicMask());
            button->setIcon(buttonimage);
            button->setIconSize(buttonimage.size());
            button->show();

            /*
            button->setText("");
            button->setStyleSheet("QPushButton{border:0px;}");
            button->setIcon(buttonimage);
            button->setIconSize(buttonimage.size());
            button->resize(buttonimage.size());
            button->show();
            */
            connect(button,SIGNAL(released()),this,SLOT(Button_clicked()));
            list.push_back(button);
            nolist.push_back(sta[j]);
            }
            for(k=0;sta[k]!=-1;k++){
                if(city->connected(sta[k],sta[j])==1){
                    q=city->getloc(sta[k]);
                    area->paintpath(zoom*q.x+positionx,zoom*q.y+positiony,zoom*p.x+positionx,zoom*p.y+positiony,zoom,add);
                    add=0;
                }
            }
        }
        delete sta;
    }

    //paint the original station and destination circle
    Point point;
    //cout << "In repaint : start = "<<start <<endl;
    if(start!= -1){
        point=city->getloc(start);
        area->paintcircle(point.x*zoom+positionx,point.y*zoom+positiony,zoom, true);
    }
    if(end != -1){
        point=city->getloc(end);
        area->paintcircle(point.x*zoom+positionx,point.y*zoom+positiony,zoom, false);
    }

    //以下为画出强调路线

        QPushButton * tempButton;
        while(stressLineButton.size()!=0){
            cout<<stressLineButton.size();
            tempButton = stressLineButton.back();
            tempButton->deleteLater();;
            stressLineButton.pop_back();
        }
        if(stressLine != -1){
            cout<<"*";
            int * lines = city->getline(stressLine);
            int j=0;
            while(lines[j]!=-1){
                cout<<"--"<<lines[j]<<"--";
                Point p=city->getloc(lines[j]);
                QPushButton * tempButton = new QPushButton("", area);

                buttonimage=buttonimage.scaled(zoom*ButtonWidth*2,zoom*ButtonWidth*2);
                tempButton=new QPushButton("",area);
                tempButton->setGeometry(zoom*p.x+positionx-ButtonWidth*zoom,zoom*p.y+positiony-ButtonWidth*zoom,ButtonWidth*zoom*2,ButtonWidth*zoom*2);
                tempButton->clearMask();
                tempButton->setBackgroundRole( QPalette::Base);
                tempButton->setMask(buttonimage.createHeuristicMask());
                tempButton->setIcon(buttonimage);
                tempButton->setIconSize(buttonimage.size());
                stressLineButton.push_back(tempButton);
                tempButton->show();
                j++;
            }
        }

//    //以下为画出最短路的路径
    {
        len=pointlist.size();
        int stanum;
        if(len==0)
            return;
        QPushButton *button;
        p=city->getloc(pointlist.at(0));
        int no=pointlist.at(0);
        for(i=1;i<len;i++){
            q=city->getloc(pointlist.at(i));
            area->paintpath(zoom*q.x+positionx,zoom*q.y+positiony,zoom*p.x+positionx,zoom*p.y+positiony,zoom,-1);
            p=q;
        }
        for(i=0;i<len;i++)
            if(i==nowpos)
                p=city->getloc(pointlist.at(i));
        stanum=path.size();
        if(nowpos<pointlist.size()){
            for(i=0;i<(stanum-1);i++){
                if(path.at(i)==pointlist.at(nowpos))
                    break;
            }
            if((i+1)<stanum&&i!=(stanum-1)){
                ui->textBrowser->append(QString::fromLocal8Bit(city->id2str(path.at(i))));
                path.at(i)=Max_Sta;
                if(path.at(i+1)<=0){

                    ui->textBrowser->append("换乘"+QString::fromLocal8Bit(city->id2line(0-path.at(i+1))));
                    path.at(i+1)=Max_Sta;
               }
            }
        }
        if(nowpos<len){
            buttonimage.load(":/button/button.jpg");
            buttonimage=buttonimage.scaled(zoom*ButtonWidth*2,zoom*ButtonWidth*2);
            if(nowbutton!=NULL)
                nowbutton->deleteLater();
            nowbutton=new QPushButton("",area);
            nowbutton->setGeometry(zoom*p.x+positionx-ButtonWidth*zoom,zoom*p.y+positiony-ButtonWidth*zoom,ButtonWidth*zoom*2,ButtonWidth*zoom*2);
            nowbutton->clearMask();
            nowbutton->setBackgroundRole( QPalette::Base);
            nowbutton->setMask(buttonimage.createHeuristicMask());
            nowbutton->setIcon(buttonimage);
            nowbutton->setIconSize(buttonimage.size());
            nowbutton->show();


        }
        else if(nowpos==len){
            if(nowbutton!=NULL)
                 nowbutton->deleteLater();
            nowbutton=NULL;
        }
    }


}
void MainWindow::ShortestPath(){
    ui->textBrowser->clear();
    if(start<0||end<0||start==end)
        return;
    Point p,q;
    int add,i,j,len;
    int wat[Max_Sta];
    int *l;
    len=pointlist.size();
    for(i=0;i<len;i++)
        pointlist.pop_back();
    len=path.size();
    for(i=0;i<len;i++)
        path.pop_back();
    l=city->shortestway(city->id2str(start),city->id2str(end));
    for(i=0;i<Max_Sta&&l[i]!=Max_Sta;i++){
        wat[i]=l[i];
        path.push_back(wat[i]);
    }
    wat[i]=Max_Sta;
    path.push_back(wat[i]);
    cout<<wat[0]<<" "<<wat[1]<<" "<<i<<endl;
    add=-1;
    repaint();
    if(wat[0]==Max_Sta)
        return ;
    for(i=j=1;wat[i]!=Max_Sta;i++){
        //QThread::msleep(500);
        if(wat[i]<=0){
            j++;
            continue;
        }
        cout<<wat[i]<<endl;
        p=city->getloc(wat[i-j]);
        if(j>1)
            j=1;
        q=city->getloc(wat[i]);
        if(pointlist.size()==0)
            pointlist.push_back(wat[i-j]);
        pointlist.push_back(wat[i]);
        area->paintpath(zoom*q.x+positionx,zoom*q.y+positiony,zoom*p.x+positionx,zoom*p.y+positiony,zoom,add);
    }
    {
        //std::thread na(printPath);
        //na.join();
        connect(this,SIGNAL(send()),this,SLOT(got()));
        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread,(LPVOID)this,0,&ThreadID);
    }
}
void MainWindow::ShortestChange(){
    ui->textBrowser->clear();

    if(start<0||end<0||start==end)
        return;
    Point p,q;
    int add,i,j,len;
    int wat[Max_Sta];
    int *l;
    len=pointlist.size();
    for(i=0;i<len;i++){
        pointlist.pop_back();
    }
    len=path.size();
    for(i=0;i<len;i++){
        path.pop_back();
    }
    l=city->least_transfer(city->id2str(start),city->id2str(end));
    for(i=0;i<Max_Sta&&l[i]!=Max_Sta;i++)
        wat[i]=l[i],path.push_back(wat[i]);
    wat[i]=Max_Sta;
    path.push_back(wat[i]);
    cout<<wat[0]<<" "<<wat[1]<<" "<<i<<endl;
    add=-1;
    repaint();
    if(wat[0]==Max_Sta)
        return ;
    for(i=j=1;wat[i]!=Max_Sta;i++){
        //QThread::msleep(500);
        if(wat[i]<=0){
            j++;
            continue;
        }
        cout<<wat[i]<<endl;
        p=city->getloc(wat[i-j]);
        if(j>1)
            j--;
        q=city->getloc(wat[i]);
        if(pointlist.size()==0)
            pointlist.push_back(wat[i-j]);
        pointlist.push_back(wat[i]);
        area->paintpath(zoom*q.x+positionx,zoom*q.y+positiony,zoom*p.x+positionx,zoom*p.y+positiony,zoom,add);
    }
    {
        //std::thread na(printPath);
        //na.join();
        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread,(LPVOID)this,0,&ThreadID);
    }

}
void MainWindow::printPath(){
    int i;
    this->nowpos=0;
    emit this->send();

    for(i=0;i<=pointlist.size();i++){
        QThread::msleep(500);
        this->nowpos++;
        emit this->send();
        cout<<"hereA"<<endl;

      //  this->repaint();

    }
}
void MainWindow::got(){
    if(nowpos==lastpos)
        return;
    lastpos=nowpos;
    cout<<"gotA"<<endl;

    repaint();
}

void MainWindow::Import_clicked(){
    //qDebug() << "HIIII!";


    QString file = QFileDialog::getOpenFileName(this,tr("Import map"),"/home/foxman", tr("Text Files (*.txt)"));
    //QString file = "../beijing-subway.txt";

    char a[100];
    int len,i,j,k;
   // int positionx,positiony;
    positionx=positiony=0;
    zoom=1;
    len=file.length();
    if(len>100){
        handle=new ErrorHandle(this);
        handle->SetText("ERROR!Your file's name is too long!");
        handle->show();
        return;
    }
    file.toStdString().copy(a,len,0);
    a[len]='\0';
    if(file==NULL){
        handle=new ErrorHandle(this);
        handle->SetText("ERROR!File open failed.No file input.");
        handle->show();
        return;
    }
    //cout<<a<<endl;
    city=new City(a);

    i=0;
    int sublinenum=city->getlinenum();
    QPushButton * button;

    buttonimage=buttonimage.scaled(zoom*ButtonWidth,zoom*ButtonWidth);
   // buttonimage.setAttribute(Qt::WA_TranslucentBackground,false);
    Point p,q;
    QString name;
 //   QLabel *label;
    int *sta;
    int add;
    int length=list.size();
    for(i=0;i<length;i++){
        button=list.back();
        button->deleteLater();
        list.pop_back();
        nolist.pop_back();
    }
    area->clear();


    for(i=0;i<sublinenum;i++){
        sta=city->getline(i);
        for(j=0,add=1;sta[j]!=-1;j++){
            p=city->getloc(sta[j]);
            button=new QPushButton("",area);
            button->setGeometry(zoom*p.x-ButtonWidth*zoom/2,zoom*p.y-ButtonWidth*zoom/2,ButtonWidth*zoom,ButtonWidth*zoom);
            name=QString::fromLocal8Bit(city->id2str(sta[j]));
            button->clearMask();
            //button->setFixedSize(buttonimage.size());
            button->setBackgroundRole( QPalette::Base);
            button->setMask(buttonimage.createHeuristicMask());
            button->setIcon(buttonimage);
            button->setIconSize(buttonimage.size());
            button->show();
            connect(button,SIGNAL(released()),this,SLOT(Button_clicked()));
            list.push_back(button);
            nolist.push_back(sta[j]);
            for(k=0;sta[k]!=-1;k++){
                if(city->connected(sta[k],sta[j])==1){
                    q=city->getloc(sta[k]);
                    area->paintpath(zoom*q.x+positionx,zoom*q.y+positiony,zoom*p.x+positionx,zoom*p.y+positiony,zoom,add);
                    add=0;
                }
            }
        }
    }
    xylen=i;
    //增加线路选择：
        //linesName
        ui->LineName->clear();
        ui->LineName->addItem("请选择线路（缺省）");
        for(int i=0;i<city->getlinenum();i++){
            ui->LineName->addItem(QString::fromLocal8Bit(city->id2line(i)));
        }

}//把按钮发出的信号连接到确切的槽
void MainWindow::Zoom_in(){
    if(zoom + ZOOMSTANDARD <ZOOMMAX){
        zoom_last=zoom;
        zoom+=ZOOMSTANDARD;
        QSize p=this->size();
        positionx=p.width()/2-((double)zoom/(double)(zoom-ZOOMSTANDARD))*(p.width()/2-positionx);
        positiony=p.height()/2-((double)zoom/(double)(zoom-ZOOMSTANDARD))*(p.height()/2-positiony);
        repaint();
    }
}
void MainWindow::Zoom_out(){
    if(zoom - ZOOMSTANDARD > ZOOMMIN){
        zoom_last=zoom;
        zoom-=ZOOMSTANDARD;
        QSize p=this->size();
        positionx=p.width()/2-((double)zoom/((zoom+ZOOMSTANDARD)))*(p.width()/2-positionx);
        positiony=p.height()/2-((double)zoom/(zoom+ZOOMSTANDARD))*(p.height()/2-positiony);
        repaint();
    }
}
void MainWindow::resizeEvent(QResizeEvent *size){
    //this->close();
    ui->verticalLayoutWidget->resize(size->size());
    repaint();
}
void MainWindow::Button_clicked(){
    cout<<"in clicked";
    int i,no,len;
    QObject *p;
    p=sender();

    len=list.size();
    for(i=0;i<len;i++){
        if(p==list.at(i))
            break;
    }
    no=nolist.at(i);
    if(start == -1){
        start = no;
    }else if(no == start || no == end){
        start = end = -1;
    }else if(end == -1){
        end = no;
    }else{
        start = end;
        end = no;
    }
    if(start != -1){
        ui->stationOrigin->setText(QString::fromLocal8Bit(city->id2str(start)));
    }else{
        ui->stationOrigin->setText("");
    }
    if(end != -1){
        ui->stationDestination->setText(QString::fromLocal8Bit(city->id2str(end)));
    }else{
        ui->stationDestination->setText("");
    }
    int length;
        length=pointlist.size();
        for(i=0;i<length;i++)
            pointlist.pop_back();;
        length=path.size();
        for(i=0;i<length;i++)
             path.pop_back();
        if(nowbutton!=NULL)
            nowbutton->deleteLater();
        nowbutton=NULL;
    repaint();

}



void MainWindow::on_butZoomIn_clicked()
{
    Zoom_in();
}

void MainWindow::on_butZoomOut_clicked()
{
    Zoom_out();
}

void MainWindow::on_stationOrigin_textChanged(const QString &arg1)
{
    QByteArray ba = ui->stationOrigin->text().toLocal8Bit();
    char * msg = ba.data();
    int id = city->str2id(msg);
    if(id != -1){
        ui->originNote->setText("√");
    }else{
        ui->originNote->setText("×");
    }
    if(id!=start){
        start = id;
        repaint();
    }
}

void MainWindow::on_stationDestination_textChanged(const QString &arg1)
{
    QByteArray ba = ui->stationDestination->text().toLocal8Bit();
    char * msg = ba.data();
    int id = city->str2id(msg);
    if(id != -1){
        ui->destinationNote->setText("√");
    }else{
        ui->destinationNote->setText("×");
    }
    if(id!=end){
        end = id;
        repaint();
    }
}

void MainWindow::on_radioButton_clicked()
{
    LenthOrTransfer = true;
}

void MainWindow::on_radioButton_2_clicked()
{
    LenthOrTransfer = false;
}


void MainWindow::on_butFindWay_clicked()
{
    if(LenthOrTransfer){
        ShortestPath();
    }else{
        ShortestChange();
    }
}

void MainWindow::on_LineName_activated(const QString &arg1)
{
    QByteArray ba = arg1.toLocal8Bit();
    char * msg = ba.data();
    for(int i=0;i<city->getlinenum();i++){
        if(strcmp(msg, city->id2line(i)) == 0){
            stressLine = i;
            repaint();
            return;
        }
    }
    stressLine = -1;
    repaint();
}
