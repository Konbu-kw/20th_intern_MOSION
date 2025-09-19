/**
 * @file /include/hw2_pkg/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/
#ifndef HW_UDP_GUI_HPP
#define HW_UDP_GUI_HPP

/*****************************************************************************
** Includes
*****************************************************************************/
#include <QMainWindow>
#include <QPushButton>
#include <QUdpSocket>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *image;


private slots:
    void on_send_clicked();
    void receive_img();

private:
    Ui::MainWindow *ui;

    QPushButton *send;
    QUdpSocket *udp;
    QLineEdit *udSsocket;
    QLineEdit *tx_port;
    QLineEdit *rx_port;
    QLineEdit *path;
    QLineEdit *ip;
};

#endif // HW_UDP_GUI_HPP

