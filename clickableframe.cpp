#include "ClickableFrame.h"


ClickableFrame::ClickableFrame(QWidget *parent)
    : QFrame(parent)
{

}

QString ClickableFrame::getFilter()
{
    return filter;
}


QString ClickableFrame::getType()
{
    return type;
}

void ClickableFrame::setFilter(QString filter)
{
    this->filter = filter;
}

void ClickableFrame::setType(QString type)
{
    this->type = type;
}

void ClickableFrame::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);
    emit clicked();
}
