/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *A;
    QPushButton *B_Apply;
    QPushButton *B_Play;
    QLineEdit *mapsize;
    QLineEdit *Obs;
    QLabel *label;
    QLabel *label_2;
    QGraphicsView *D;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        A = new QGraphicsView(centralwidget);
        A->setObjectName("A");
        A->setGeometry(QRect(0, 0, 301, 301));
        B_Apply = new QPushButton(centralwidget);
        B_Apply->setObjectName("B_Apply");
        B_Apply->setGeometry(QRect(290, 380, 95, 25));
        B_Play = new QPushButton(centralwidget);
        B_Play->setObjectName("B_Play");
        B_Play->setGeometry(QRect(400, 380, 95, 25));
        mapsize = new QLineEdit(centralwidget);
        mapsize->setObjectName("mapsize");
        mapsize->setGeometry(QRect(10, 380, 113, 25));
        Obs = new QLineEdit(centralwidget);
        Obs->setObjectName("Obs");
        Obs->setGeometry(QRect(150, 380, 113, 25));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 350, 67, 17));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 350, 91, 17));
        D = new QGraphicsView(centralwidget);
        D->setObjectName("D");
        D->setGeometry(QRect(310, 0, 301, 301));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(150, 310, 67, 17));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(440, 310, 67, 17));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(270, 380, 16, 17));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        B_Apply->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        B_Play->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Size(n*n)", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Obstacles(%)", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "A*", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Dijkstra", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
