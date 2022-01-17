#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setAcceptDrops(true);
    connect(ui->widget,SIGNAL(onDrop(QDropEvent*)),this,SLOT(onDrop(QDropEvent*)));
    connect(ui->widget,SIGNAL(onPress(QMouseEvent*)),this,SLOT(onPress(QMouseEvent*)));
    connect(ui->widget,SIGNAL(onPaint(QPaintEvent*)),this,SLOT(onPaint(QPaintEvent*)));
    connect(ui->label_create_start,SIGNAL(onPress(ActiveLabel*,QMouseEvent*)),this,SLOT(onCreateNode(ActiveLabel*,QMouseEvent*)));
    ui->label_create_start->id = "create_start";
    connect(ui->label_create_proc,SIGNAL(onPress(ActiveLabel*,QMouseEvent*)),this,SLOT(onCreateNode(ActiveLabel*,QMouseEvent*)));
    ui->label_create_proc->id = "create_proc";
    connect(ui->label_create_div,SIGNAL(onPress(ActiveLabel*,QMouseEvent*)),this,SLOT(onCreateNode(ActiveLabel*,QMouseEvent*)));
    ui->label_create_div->id = "create_div";
    selected = nullptr;
    current = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCreateNode(ActiveLabel *label,QMouseEvent* event)
{
    QDrag *drag;
    if(label->id=="create_start")
    {
        drag = new QDrag(ui->widget);
        QMimeData *mimeData = new QMimeData();
        mimeData->setText("start");
        drag->setMimeData(mimeData);
        drag->exec();
    }else if(label->id=="create_proc")
    {
        drag = new QDrag(ui->widget);
        QMimeData *mimeData = new QMimeData();
        mimeData->setText("proc");
        drag->setMimeData(mimeData);
        drag->exec();
    }else if(label->id=="create_div")
    {
        drag = new QDrag(ui->widget);
        QMimeData *mimeData = new QMimeData();
        mimeData->setText("div");
        drag->setMimeData(mimeData);
        drag->exec();
    }
}

void MainWindow::addCodeBlock(CodeBlock *bl)
{
    int count=0;
    QString id;
    do{
        id = QString("Block%1").arg(count);
        ++count;
    }
    while(codeblocks.contains(id));
    bl->id = id;
    connect(bl,SIGNAL(onDrop(QString,QDropEvent*)),this,SLOT(onDropOnNode(QString,QDropEvent*)));
    connect(bl,SIGNAL(onPress(QString,QMouseEvent*)),this,SLOT(onPressOnNode(QString,QMouseEvent*)));
    codeblocks.insert(id,bl);
}

void MainWindow::onDrop(QDropEvent* event)
{
    QString s = event->mimeData()->text();
    if(s=="start")
    {
        CodeBlock *q = new CodeBlock(ui->widget);
        q->type = "start";
        q->move(event->position().toPoint()-QPoint(q->width()/2,q->height()/2));
        q->show();
        q->resize(QSize(q->width()-10,q->height()));
        addCodeBlock(q);
        q->setText(q->id);
    }else{
        QStringList sl = s.split(":");
        if(sl.length()==2&&sl.at(0)=="move")
        {
            CodeBlock *t = codeblocks.find(sl.at(1)).value();
            if(t!=nullptr&&t->type=="start")
            {
                t->move(event->position().toPoint()-QPoint(t->width()/2,t->height()/2));
                t=t->next;
                while(t!=nullptr)
                {
                    t->move(t->prev->pos()+QPoint(0,t->prev->height()*7/6));
                    t = t->next;
                }
            }
        }
    }
    ui->widget->repaint();
}

void MainWindow::onDropOnNode(QString id,QDropEvent* event)
{
    QString s = event->mimeData()->text();
    if(s=="proc")
    {
        if(codeblocks.contains(id)){
            CodeBlock *q = new CodeBlock(ui->widget);
            q->type = "proc";
            CodeBlock *p = codeblocks.find(id).value();
            q->move(p->pos()+QPoint(0,p->height()*7/6));
            q->code = "nop";

            CodeBlock *t = nullptr;

            if(p->next!=nullptr){
                t = p->next;
                q->next = t;
                t->prev = q;
                while(t!=nullptr)
                {
                    t->move(t->prev->pos()+QPoint(0,t->prev->height()*7/6));
                    t = t->next;
                }
            }

            q->prev = p;
            p->next = q;

            q->show();

            addCodeBlock(q);

            q->setText(q->code);
        }
    }else if(s=="div"){
        if(codeblocks.contains(id)){
            CodeBlock *q = new CodeBlock(ui->widget);
            q->type = "div";
            q->code = "true";
            CodeBlock *p = codeblocks.find(id).value();
            q->move(p->pos()+QPoint(0,p->height()*7/6));
            q->showButton(true);
            connect(q,SIGNAL(onAction(QString)),this,SLOT(onNodeAction(QString)));

            CodeBlock *t = nullptr;

            if(p->next!=nullptr){
                t = p->next;
                q->next = t;
                t->prev = q;
                while(t!=nullptr)
                {
                    t->move(t->prev->pos()+QPoint(0,t->prev->height()*7/6));
                    t = t->next;
                }
            }

            q->prev = p;
            p->next = q;

            q->show();

            addCodeBlock(q);

            q->setText(q->id);
        }
    }else{
        QStringList sl = s.split(":");
        if(sl.length()==2&&sl.at(0)=="move")
        {
            if(sl.at(1)==id)
            {
                if(codeblocks.find(sl.at(1)).value()->type=="start")
                {
                    ui->lineEdit->setText(codeblocks.find(sl.at(1)).value()->id);
                }else{
                    ui->lineEdit->setText(codeblocks.find(sl.at(1)).value()->code);
                }
                selected = codeblocks.find(sl.at(1)).value();
                ui->widget->repaint();
                return;
            }
            CodeBlock *t = codeblocks.find(sl.at(1)).value();
            CodeBlock *t2;
            if(t!=nullptr&&t->id!=id&&(t->type=="proc"||t->type=="div"))
            {
                t->prev->next = nullptr;
                t->prev = codeblocks.find(id).value();
                t2 = t->prev->next;
                t->prev->next = t;
                t->move(t->prev->pos()+QPoint(0,t->prev->height()*7/6));

                while(t->next!=nullptr)
                {
                    t = t->next;
                    t->move(t->prev->pos()+QPoint(0,t->prev->height()*7/6));
                }

                t->next = t2;
                if(t2)
                {
                    t2->prev = t;
                    while(t->next!=nullptr)
                    {
                        t = t->next;
                        t->move(t->prev->pos()+QPoint(0,t->prev->height()*7/6));
                    }
                }

            }
        }else if(sl.length()==2&&sl.at(0)=="connect")
        {
            if(codeblocks.find(id).value()->type=="proc")
                codeblocks.find(sl.at(1)).value()->extra = codeblocks.find(id).value();
            if(id==sl.at(1))
                codeblocks.find(id).value()->lineExpand = (codeblocks.find(id).value()->lineExpand)%6+1;
        }
    }
    ui->widget->repaint();
}

void MainWindow::onPressOnNode(QString id,QMouseEvent *event)
{
    QDrag *drag = new QDrag(ui->widget);
    QMimeData *mimeData = new QMimeData();
    mimeData->setText(QString("move:%1").arg(id));
    drag->setMimeData(mimeData);
    drag->exec();
}

void MainWindow::onPaint(QPaintEvent *event)
{
    QPainter painter(ui->widget);
    CodeBlock *block;
    QPen pen(qRgb(234,54,13));
    pen.setWidth(4);
    painter.setPen(pen);
    for(auto iter = codeblocks.begin();iter!=codeblocks.end();++iter)
    {
        block = iter.value();
        if(block->next)
            painter.drawLine(block->pos()+QPoint(block->width()/2,block->height()/2),block->next->pos()+QPoint(block->width()/2,block->next->height()/2));
        if(block->extra)
        {
            int x50 = block->lineExpand*50;
            painter.drawLine(block->pos()+QPoint(block->width(),block->height()/2),block->pos()+QPoint(block->width()+x50,block->height()/2));
            painter.drawLine(block->pos()+QPoint(block->width()+x50,block->height()/2),QPoint(block->x()+block->width()+x50,block->extra->y()+block->extra->height()/2));
            painter.drawLine(QPoint(block->x()+block->width()+x50,block->extra->y()+block->extra->height()/2),block->extra->pos()+QPoint(block->width()/2,block->extra->height()/2));
        }
        if(block==selected)
        {
            pen.setColor(qRgb(214,45,231));
            painter.setPen(pen);
            painter.drawRect(block->x()-3,block->y()-3,block->width()+6,block->height()+6);
        }
        if(block==current)
        {
            pen.setColor(qRgb(45,45,231));
            painter.setPen(pen);
            painter.drawRect(block->x()-2,block->y()-2,block->width()+4,block->height()+4);
        }
        pen.setColor(qRgb(234,54,13));
        painter.setPen(pen);
    }
}

void MainWindow::onNodeAction(QString id)
{
    QDrag *drag = new QDrag(ui->widget);
    QMimeData *mimeData = new QMimeData();
    mimeData->setText(QString("connect:%1").arg(id));
    drag->setMimeData(mimeData);
    drag->exec();
}

void MainWindow::onPress(QMouseEvent*)
{

}

void MainWindow::on_lineEdit_returnPressed()
{
    qDebug()<<"OK";
    if(selected&&selected->type!="start")
    {
        selected->code = ui->lineEdit->text();
        selected->setText(selected->code);
    }else{
        if(selected&&selected->type=="start"&&!codeblocks.contains(ui->lineEdit->text()))
        {
            codeblocks.remove(selected->id);
            selected->id = ui->lineEdit->text();
            codeblocks.insert(selected->id,selected);
            selected->setText(selected->id);
        }
    }
}


void MainWindow::on_action_run_triggered()
{
    if(codeblocks.contains("main"))
    {
        current = codeblocks.find("main").value();
    }
    ui->widget->repaint();
}


void MainWindow::on_action_step_triggered()
{
    if(current)
    {
        if(current->type=="start")
            current->intend = current->next;
        current = current->intend;
        if(current)
        {
            if(current->type=="proc")
            {
                QStringList args = current->code.split(" ");
                if(args.length()==3)
                {
                    if(args.at(0)=="set")
                    {
                        codeints.insert(args.at(1),args.at(2).toInt());
                    }
                    else if(args.at(0)=="add")
                    {
                        if(codeints.contains(args[1]))
                        {
                            codeints.insert(args.at(1),codeints.find(args.at(1)).value()+args.at(2).toInt());
                        }
                    }
                    else if(args.at(0)=="sub")
                    {
                        if(codeints.contains(args[1]))
                        {
                            codeints.insert(args.at(1),codeints.find(args.at(1)).value()-args.at(2).toInt());
                        }
                    }
                }
                current->intend = current->next;
            }
            else if(current->type=="div")
            {
                QStringList args;
                bool jumped = false;
                if(current->code.contains("<="))
                {
                    args = current->code.split("<=");
                    if(codeints.contains(args.at(0)))
                    {
                        if(codeints.find(args.at(0)).value()<=args.at(1).toInt())
                        {
                            current->intend = current->extra;
                            jumped = true;
                        }
                    }
                }else if(current->code.contains("<"))
                {
                    args = current->code.split("<");
                    if(codeints.contains(args.at(0)))
                    {
                        if(codeints.find(args.at(0)).value()<args.at(1).toInt())
                        {
                            current->intend = current->extra;
                            jumped = true;
                        }
                    }
                }else if(current->code.contains(">="))
                {
                    args = current->code.split(">=");
                    if(codeints.contains(args.at(0)))
                    {
                        if(codeints.find(args.at(0)).value()>=args.at(1).toInt())
                        {
                            current->intend = current->extra;
                            jumped = true;
                        }
                    }
                }else if(current->code.contains(">"))
                {
                    args = current->code.split(">");
                    if(codeints.contains(args.at(0)))
                    {
                        if(codeints.find(args.at(0)).value()>args.at(1).toInt())
                        {
                            current->intend = current->extra;
                            jumped = true;
                        }
                    }
                }else if(current->code.contains("="))
                {
                    args = current->code.split("=");
                    if(codeints.contains(args.at(0)))
                    {
                        if(codeints.find(args.at(0)).value()==args.at(1).toInt())
                        {
                            current->intend = current->extra;
                            jumped = true;
                        }
                    }
                }
                if(!jumped)
                {
                    current->intend = current->next;
                }
            }
        }
    }
    ui->listWidget->clear();
    for(auto iter = codeints.begin();iter!=codeints.end();++iter)
    {
        ui->listWidget->addItem(QString("%1=%2").arg(iter.key()).arg(iter.value()));
    }
    ui->widget->repaint();
}

