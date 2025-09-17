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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QSlider *slider1;
    QSlider *slider2;
    QSlider *slider3;
    QPushButton *save;
    QPushButton *load;
    QPushButton *rotate;
    QMenuBar *menubar;
    QMenu *menuROARMSIM;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 561, 421));
        slider1 = new QSlider(centralwidget);
        slider1->setObjectName("slider1");
        slider1->setGeometry(QRect(0, 430, 181, 16));
        slider1->setOrientation(Qt::Orientation::Horizontal);
        slider2 = new QSlider(centralwidget);
        slider2->setObjectName("slider2");
        slider2->setGeometry(QRect(190, 430, 181, 16));
        slider2->setOrientation(Qt::Orientation::Horizontal);
        slider3 = new QSlider(centralwidget);
        slider3->setObjectName("slider3");
        slider3->setGeometry(QRect(380, 430, 181, 16));
        slider3->setOrientation(Qt::Orientation::Horizontal);
        save = new QPushButton(centralwidget);
        save->setObjectName("save");
        save->setGeometry(QRect(570, 430, 51, 25));
        load = new QPushButton(centralwidget);
        load->setObjectName("load");
        load->setGeometry(QRect(630, 430, 51, 25));
        rotate = new QPushButton(centralwidget);
        rotate->setObjectName("rotate");
        rotate->setGeometry(QRect(570, 460, 111, 25));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuROARMSIM = new QMenu(menubar);
        menuROARMSIM->setObjectName("menuROARMSIM");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuROARMSIM->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        save->setText(QCoreApplication::translate("MainWindow", "SAVE", nullptr));
        load->setText(QCoreApplication::translate("MainWindow", "LOAD", nullptr));
        rotate->setText(QCoreApplication::translate("MainWindow", "Clock Rotate", nullptr));
        menuROARMSIM->setTitle(QCoreApplication::translate("MainWindow", "ROARMSIM", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
