#include "codeblock.h"
#include "ui_codeblock.h"

CodeBlock::CodeBlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeBlock)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    prev = nullptr;
    next = nullptr;
    extra = nullptr;
    intend = nullptr;
    ui->pushButton->hide();
    lineExpand = 1;
    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(onButtonPressed()));
}

CodeBlock::~CodeBlock()
{
    delete ui;
}

void CodeBlock::dropEvent(QDropEvent* event)
{
    emit onDrop(id,event);
}

void CodeBlock::mousePressEvent(QMouseEvent* event)
{
    emit onPress(id,event);
}

void CodeBlock::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void CodeBlock::setText(QString text)
{
    ui->label->setText(text);
}

void CodeBlock::onButtonPressed()
{
    emit onAction(id);
}

void CodeBlock::showButton(bool b)
{
    if(b)
    {
        ui->pushButton->show();
    }else{
        ui->pushButton->hide();
    }
}

