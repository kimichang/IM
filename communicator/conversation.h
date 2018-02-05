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

private:
    Ui::conversation *ui;
    void Setup();
};

#endif // CONVERSATION_H
