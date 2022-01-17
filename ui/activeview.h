#ifndef ACTIVEVIEW_H
#define ACTIVEVIEW_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>

class ActiveView : public QWidget
{
    Q_OBJECT
public:
    explicit ActiveView(QWidget *parent = nullptr);

public slots:
    void mouseMoveEvent(QMouseEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
signals:
    void onMove(QMouseEvent *);
    void onDrop(QDropEvent *);
    void onPaint(QPaintEvent *);
    void onPress(QMouseEvent*);

};

#endif // ACTIVEVIEW_H
