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
    QString getSource();
    void setFilter(QString filter);
    void setType(QString type);
    void setSource(QString source);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString filter;
    QString type;
    QString source;
};

#endif // CLICKABLEFRAME_H
