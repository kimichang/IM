#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <conversation.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Show();
    void onItemClicked(QListWidgetItem*);
    void CreateConversation(QListWidgetItem*);

private:
    Ui::MainWindow *ui;
    QAction *openaction;
    Ui::conversation* co;
    void open();

};

#endif // MAINWINDOW_H
