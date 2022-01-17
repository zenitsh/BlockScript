#ifndef CODEBLOCK_H
#define CODEBLOCK_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPushButton>

namespace Ui {
class CodeBlock;
}

class CodeBlock : public QWidget
{
    Q_OBJECT

public:
    explicit CodeBlock(QWidget *parent = nullptr);
    ~CodeBlock();

    QString id;
    QString type;
    QString code;

    int lineExpand;

    void setText(QString);
    void showButton(bool b);
    CodeBlock *prev,*next,*extra,*intend;

public slots:

    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);
    void mousePressEvent(QMouseEvent*);
    void onButtonPressed();

signals:
    void onDrop(QString,QDropEvent *);
    void onPress(QString,QMouseEvent *);
    void onAction(QString);

private:
    Ui::CodeBlock *ui;


};

#endif // CODEBLOCK_H
