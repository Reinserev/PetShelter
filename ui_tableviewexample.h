/********************************************************************************
** Form generated from reading UI file 'tableviewexample.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLEVIEWEXAMPLE_H
#define UI_TABLEVIEWEXAMPLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableViewExample
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *genesTableView;

    void setupUi(QWidget *TableViewExample)
    {
        if (TableViewExample->objectName().isEmpty())
            TableViewExample->setObjectName(QStringLiteral("TableViewExample"));
        TableViewExample->resize(758, 433);
        verticalLayout = new QVBoxLayout(TableViewExample);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        genesTableView = new QTableView(TableViewExample);
        genesTableView->setObjectName(QStringLiteral("genesTableView"));

        verticalLayout->addWidget(genesTableView);


        retranslateUi(TableViewExample);

        QMetaObject::connectSlotsByName(TableViewExample);
    } // setupUi

    void retranslateUi(QWidget *TableViewExample)
    {
        TableViewExample->setWindowTitle(QApplication::translate("TableViewExample", "TableViewExample", 0));
    } // retranslateUi

};

namespace Ui {
    class TableViewExample: public Ui_TableViewExample {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLEVIEWEXAMPLE_H
