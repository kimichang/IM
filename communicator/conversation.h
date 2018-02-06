#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QDialog>
#include "user.h"

namespace Ui {
class conversation;
}

class conversation : public QDialog
{
    Q_OBJECT

public:
    Ui::conversation *ui;
    explicit conversation(QWidget *parent = 0);
    ~conversation();
    void Setup();
    bool TryConnect(User*);
private:


};

#endif // CONVERSATION_H
