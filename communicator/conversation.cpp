#include "conversation.h"
#include "ui_conversation.h"

conversation::conversation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conversation)
{
    ui->setupUi(this);
}

conversation::~conversation()
{
    delete ui;
}

void conversation::Setup()
{
//    auto co = new conversation();

//    this->peer->addItem(new QListWidgetItem(tr("you")));
//    co->peer->addItem(item);
//    co->show();
    ui->input->setFocus();


}

