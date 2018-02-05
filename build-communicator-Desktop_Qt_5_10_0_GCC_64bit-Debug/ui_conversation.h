/********************************************************************************
** Form generated from reading UI file 'conversation.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERSATION_H
#define UI_CONVERSATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_conversation
{
public:
    QDialogButtonBox *buttonBox;
    QListWidget *peer;
    QListWidget *dialog;
    QListWidget *input;

    void setupUi(QDialog *conversation)
    {
        if (conversation->objectName().isEmpty())
            conversation->setObjectName(QStringLiteral("conversation"));
        conversation->resize(379, 501);
        buttonBox = new QDialogButtonBox(conversation);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(20, 470, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        peer = new QListWidget(conversation);
        peer->setObjectName(QStringLiteral("peer"));
        peer->setGeometry(QRect(10, 10, 361, 71));
        dialog = new QListWidget(conversation);
        dialog->setObjectName(QStringLiteral("dialog"));
        dialog->setGeometry(QRect(10, 90, 361, 221));
        input = new QListWidget(conversation);
        input->setObjectName(QStringLiteral("input"));
        input->setGeometry(QRect(10, 320, 361, 151));

        retranslateUi(conversation);
        QObject::connect(buttonBox, SIGNAL(accepted()), conversation, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), conversation, SLOT(reject()));

        QMetaObject::connectSlotsByName(conversation);
    } // setupUi

    void retranslateUi(QDialog *conversation)
    {
        conversation->setWindowTitle(QApplication::translate("conversation", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class conversation: public Ui_conversation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERSATION_H
