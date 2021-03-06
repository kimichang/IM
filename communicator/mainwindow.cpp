#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPushButton>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QListWidget>
#include <QListView>
#include <qdebug.h>
#include "conversation.h"
#include "ui_conversation.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->status->setItemIcon(0,QIcon(":/images/timg.jpg"));
    ui->status->setItemIcon(1,QIcon(":/images/timg.jpg"));
    ui->userlist->addItem(new QListWidgetItem(tr("name 1")));
    ui->userlist->addItem(new QListWidgetItem(tr("name 2")));
    ui->userlist->addItem(new QListWidgetItem(tr("third man")));
    ui->userlist->addItem(new QListWidgetItem(QIcon(":/images/timg.jpg"),tr("lync")));
    ui->userlist->addItem(new QListWidgetItem(QIcon(":/images/window.ico"),tr("icon")));

 //   Conversation* cs = new Conversation();
 //   void (*fn)(void)  = &Conversation::Setup;
    connect(ui->userlist,&QListWidget::itemDoubleClicked,this,&MainWindow::CreateConversation);

    statusBar();
}

void MainWindow::onItemClicked(QListWidgetItem *item){
    qDebug() << "got" << item->text();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Show()
{
    cout<<"show in mainwindow\n";
}
void MainWindow::open()
{
    QMessageBox::information(this,tr("Information"),tr("Open"));
}
void MainWindow::CreateConversation(QListWidgetItem* item){

//    co->show();
    //to find user_info by item text

    auto co = new conversation();
    co->ui->peer->addItem(new QListWidgetItem(tr("you")));
    co->ui->peer->addItem(new QListWidgetItem(QIcon(item->icon()),item->text()));
    co->Setup();
    co->show();


}


