#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <codeblock.h>
#include <ui/activelabel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMap<QString,CodeBlock*> codeblocks;
    QMap<QString,int> codeints;
    CodeBlock *selected;
    CodeBlock *current;

    void addCodeBlock(CodeBlock*);


public slots:
    void onCreateNode(ActiveLabel*,QMouseEvent*);
    void onDrop(QDropEvent*);
    void onDropOnNode(QString,QDropEvent*);
    void onPress(QMouseEvent*);
    void onPressOnNode(QString,QMouseEvent*);
    void onPaint(QPaintEvent*);
    void onNodeAction(QString);

private slots:
    void on_lineEdit_returnPressed();
    void on_action_run_triggered();
    void on_action_step_triggered();
};
#endif // MAINWINDOW_H
