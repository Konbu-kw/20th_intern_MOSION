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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *main;
    QPushButton *hostButton;
    QPushButton *joinButton;
    QLabel *Title;
    QLineEdit *ip_input;
    QLabel *label;
    QLineEdit *size_input;
    QLabel *label_2;
    QWidget *play;
    QGraphicsView *graphicsView;
    QLabel *label_item;
    QTextEdit *item;
    QLabel *label_status;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QTextEdit *health;
    QLabel *label_6;
    QPushButton *readyButton;
    QTextEdit *log;
    QLabel *label_9;
    QLabel *label_10;
    QWidget *end;
    QTextEdit *resultTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(20, 10, 761, 491));
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color:rgb(220, 220, 220)"));
        main = new QWidget();
        main->setObjectName("main");
        hostButton = new QPushButton(main);
        hostButton->setObjectName("hostButton");
        hostButton->setGeometry(QRect(270, 210, 95, 25));
        hostButton->setStyleSheet(QString::fromUtf8("color:rgb(0, 0, 0)"));
        joinButton = new QPushButton(main);
        joinButton->setObjectName("joinButton");
        joinButton->setGeometry(QRect(270, 360, 95, 25));
        joinButton->setStyleSheet(QString::fromUtf8("color:black"));
        Title = new QLabel(main);
        Title->setObjectName("Title");
        Title->setGeometry(QRect(210, 140, 311, 51));
        Title->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font: 600 25pt \"URW Bookman\";\n"
"    color: black;\n"
"    border-radius: 8px;\n"
"}"));
        ip_input = new QLineEdit(main);
        ip_input->setObjectName("ip_input");
        ip_input->setGeometry(QRect(260, 390, 113, 25));
        label = new QLabel(main);
        label->setObjectName("label");
        label->setGeometry(QRect(230, 390, 21, 17));
        label->setStyleSheet(QString::fromUtf8("color:black"));
        size_input = new QLineEdit(main);
        size_input->setObjectName("size_input");
        size_input->setGeometry(QRect(320, 240, 41, 25));
        label_2 = new QLabel(main);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(280, 245, 31, 17));
        label_2->setStyleSheet(QString::fromUtf8("color:black"));
        stackedWidget->addWidget(main);
        play = new QWidget();
        play->setObjectName("play");
        graphicsView = new QGraphicsView(play);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(10, 10, 471, 391));
        label_item = new QLabel(play);
        label_item->setObjectName("label_item");
        label_item->setGeometry(QRect(490, 200, 67, 17));
        label_item->setStyleSheet(QString::fromUtf8("color:black"));
        item = new QTextEdit(play);
        item->setObjectName("item");
        item->setGeometry(QRect(490, 220, 104, 111));
        item->setStyleSheet(QString::fromUtf8("color:black"));
        label_status = new QLabel(play);
        label_status->setObjectName("label_status");
        label_status->setGeometry(QRect(490, 10, 67, 17));
        label_status->setStyleSheet(QString::fromUtf8("color:black"));
        label_3 = new QLabel(play);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 410, 171, 17));
        label_3->setStyleSheet(QString::fromUtf8("color:black"));
        label_4 = new QLabel(play);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 430, 171, 17));
        label_4->setStyleSheet(QString::fromUtf8("color:black"));
        label_5 = new QLabel(play);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 450, 171, 17));
        label_5->setStyleSheet(QString::fromUtf8("color:black"));
        health = new QTextEdit(play);
        health->setObjectName("health");
        health->setGeometry(QRect(490, 60, 101, 131));
        health->setStyleSheet(QString::fromUtf8("color:black"));
        label_6 = new QLabel(play);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(495, 35, 31, 17));
        label_6->setStyleSheet(QString::fromUtf8("color:black"));
        readyButton = new QPushButton(play);
        readyButton->setObjectName("readyButton");
        readyButton->setGeometry(QRect(490, 370, 95, 25));
        readyButton->setStyleSheet(QString::fromUtf8("color:black"));
        log = new QTextEdit(play);
        log->setObjectName("log");
        log->setGeometry(QRect(630, 30, 121, 451));
        log->setStyleSheet(QString::fromUtf8("color:black"));
        label_9 = new QLabel(play);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(490, 350, 67, 17));
        label_9->setStyleSheet(QString::fromUtf8("color:black"));
        label_10 = new QLabel(play);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(10, 470, 171, 17));
        label_10->setStyleSheet(QString::fromUtf8("color:black"));
        stackedWidget->addWidget(play);
        item->raise();
        graphicsView->raise();
        label_item->raise();
        label_status->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        health->raise();
        label_6->raise();
        readyButton->raise();
        log->raise();
        label_9->raise();
        label_10->raise();
        end = new QWidget();
        end->setObjectName("end");
        resultTextEdit = new QTextEdit(end);
        resultTextEdit->setObjectName("resultTextEdit");
        resultTextEdit->setGeometry(QRect(340, 130, 104, 70));
        stackedWidget->addWidget(end);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        hostButton->setText(QCoreApplication::translate("MainWindow", "HOST", nullptr));
        joinButton->setText(QCoreApplication::translate("MainWindow", "JOIN", nullptr));
        Title->setText(QCoreApplication::translate("MainWindow", "Alcatraz Arena", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "IP", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Size", nullptr));
        label_item->setText(QCoreApplication::translate("MainWindow", "Item", nullptr));
        label_status->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "WASD -> \354\241\260\354\242\205 / \352\271\250\352\270\260(\352\263\265\352\262\251)", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "C -> \355\214\250\353\247\201", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "L_Alt   -> \354\225\204\354\235\264\355\205\234 \354\202\254\354\232\251", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\354\262\264\353\240\245", nullptr));
        readyButton->setText(QCoreApplication::translate("MainWindow", "Ready", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\354\244\200\353\271\204 \353\262\204\355\212\274", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Q   -> \354\225\204\354\235\264\355\205\234 \353\263\200\352\262\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
