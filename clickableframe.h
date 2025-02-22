#ifndef CLICKABLEFRAME_H
#define CLICKABLEFRAME_H

#include <QFrame>
#include <QString>
#include <QMouseEvent>

class ClickableFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ClickableFrame(QWidget *parent = nullptr);
    QString getFilter();
    QString getType();
    void setFilter(QString filter);
    void setType(QString type);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString filter;
    QString type;
};

#endif // CLICKABLEFRAME_H
