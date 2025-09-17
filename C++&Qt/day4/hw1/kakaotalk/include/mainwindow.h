#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QHostAddress>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMessage();
    void receiveMessage();

private:
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QLineEdit *nickname;
    QLineEdit *target_address;
    QLineEdit *rx_port;
    QLineEdit *tx_port;
    QPushButton *sendButton;
    QUdpSocket *udpSocket;
};

#endif // MAINWINDOW_H
