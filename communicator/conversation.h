#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QDialog>

namespace Ui {
class conversation;
}

class conversation : public QDialog
{
    Q_OBJECT

public:
    explicit conversation(QWidget *parent = 0);
    ~conversation();
    void Setup();

private:
    Ui::conversation *ui;

};

#endif // CONVERSATION_H
