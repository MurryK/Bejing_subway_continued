#include "paintwidget.h"
#include <QPainter>
#include <QPen>
PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent)
{
    isDrawing = false;
    image = QPixmap(2000 ,2000);
    backColor = qRgb(200,200,200);
    image.fill(backColor);
    colornum=0;

    {
        color[0]=qRgb(204,0,0);
        color[1]=qRgb(0,102,204);
        color[2]=qRgb(0,204,204);
        color[3]=qRgb(153,0,102);
        color[4]=qRgb(234,153,20);
        color[5]=qRgb(231,96,14);
        color[6]=qRgb(0,102,51);
        color[7]=qRgb(173,203,19);
        color[8]=qRgb(0,135,255);
        color[9]=qRgb(245,207,92);
        color[10]=qRgb(211,145,136);
        color[11]=qRgb(92,42,105);
        color[12]=qRgb(204,0,0);
        color[13]=qRgb(196,0,130);
        color[14]=qRgb(255,55,3);
        color[15]=qRgb(213,53,63);
        color[16]=qRgb(182,153,189);
        color[17]=qRgb(0x45,0x8b,0x00);
        color[18]=qRgb(0x41,0x69,0xe1);
        color[19]=qRgb(0xa5,0x2a,0x2a);
    }
}

void PaintWidget :: paintEvent(QPaintEvent *){


    QPainter painter(this);


    painter.drawPixmap(0,0,image);
}
void PaintWidget::paintcircle(int x1,int y1,int zoom, bool startOrEnd){
    QPainter painter(&image);
    QPainterPath path;
    path.moveTo(x1+10*zoom,y1);
    path.arcTo(x1-10*zoom,y1-10*zoom,20*zoom,20*zoom,0,360);
    QPen pen=painter.pen();
    pen.setWidth(5*zoom);
    if(startOrEnd)
        pen.setColor(qRgb(0xca,0xff,0x70));
    else
        pen.setColor(qRgb(0xee,0,0));
    painter.setPen(pen);
    painter.drawPath(path);
}
/*
QPoint PaintWidget::getPos(){
    image.physicalDpiX()
}
*/
void PaintWidget::paintpath(int x1,int y1,int x2,int y2,int zoom,int add){
    QPainter painter(&image);
    QPainterPath path;
    path.moveTo(x1,y1);
    path.lineTo(x2,y2);
    painter.setBrush(Qt::darkGray);
    QPen pen=painter.pen();
    pen.setWidth(5*zoom);
    if(colornum<PRECOLOR){
        pen.setColor(color[colornum]);
        if(add==-1)
            pen.setColor(qRgb(0xee,0,0));
        painter.setPen(pen);
    }else{
        int r,g,b,i;
        int rr,gg,bb;
        r=g=b=0;
        srand((unsigned int )time(NULL));
        while(true){
            r=rand()%256;
            g=rand()%256;
            b=rand()%256;
            for(i=0;i<colornum;i++){
                color[i].getRgb(&rr,&gg,&bb);
                if((rr)==r&&(gg)==g&&(bb)==b)
                    break;
            }
            if(i==PRECOLOR)
                break;
        }
        color[colornum]=qRgb(r,g,b);
        pen.setColor(qRgb(r,g,b));

        if(add==-1)
            pen.setColor(qRgb(0xee,0,0));
        painter.setPen(pen);
    }
    painter.drawPath(path);
    if(add>=0)
        colornum+=add;
}
void PaintWidget::clear(){
    image.fill(backColor);
    update();
    colornum=0;
}

/*
void PaintWidget::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        lastPoint=event->pos();
        endPoint=event->pos();
        isDrawing=true;
    }
}
*/
void PaintWidget::resize(QSize size){
    image=QPixmap(size.width(),size.height());
    backColor = qRgb(BACKGROUNDCOLOR);
    image.fill(backColor);
}
