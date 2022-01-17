#ifndef ACTIVELABEL_H
#define ACTIVELABEL_H

#include <QLabel>


class ActiveLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ActiveLabel(QWidget *parent = nullptr);

    QString id;

public slots:
    void mousePressEvent(QMouseEvent*);


signals:
    void onPress(ActiveLabel*,QMouseEvent*);


};

#endif // ACTIVELABEL_H
