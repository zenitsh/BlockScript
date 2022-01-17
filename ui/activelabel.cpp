#include "activelabel.h"

ActiveLabel::ActiveLabel(QWidget *parent)
    : QLabel{parent}
{

}


void ActiveLabel::mousePressEvent(QMouseEvent* event)
{
    emit onPress(this,event);
}


