#include "activeview.h"

ActiveView::ActiveView(QWidget *parent)
    : QWidget{parent}
{

}

void ActiveView::mouseMoveEvent(QMouseEvent* event)
{
    emit onMove(event);
}

void ActiveView::mousePressEvent(QMouseEvent* event)
{
    emit onPress(event);
}

void ActiveView::dropEvent(QDropEvent* event)
{
    emit onDrop(event);
}

void ActiveView::dragEnterEvent(QDragEnterEvent *event)
{
    QString s = event->mimeData()->text();
    if(s=="start")
    {
        event->accept();
    }
    else
    {
        QStringList sl = s.split(":");
        if(sl.length()==2&&sl.at(0)=="move")
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void ActiveView::paintEvent(QPaintEvent *event)
{
    emit onPaint(event);
}
