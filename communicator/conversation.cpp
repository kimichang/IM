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
  //  conversation* co = new conversation();
  //  co->ui->setupUi();
  //  co->show();
}

