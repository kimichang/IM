#include "conversation.h"
#include "ui_conversation.h"
#include "user.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <event2/event.h>


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

bool conversation::TryConnect(User* user){

    int sockfd;
    sockaddr_in *clientaddr;
    clientaddr->sin_family = AF_INET;
    clientaddr->sin_port = htons(user->getport());
    //clientaddr->sin_addr.s_addr = htons(user->getip());
}
