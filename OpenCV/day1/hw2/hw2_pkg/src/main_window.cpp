/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/
#include "hw2_pkg/main_window.hpp"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QHostAddress>
#include <QDebug>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //윈도우 이름
    setWindowTitle("Hw2");

    ip = new QLineEdit(this);
    tx_port = new QLineEdit(this);
    rx_port = new QLineEdit(this);
    path = new QLineEdit(this);
    image = new QLabel(this);

    image = ui->image;
    send = ui->send;
    ip = ui->ip;
    tx_port = ui->tx_port;
    rx_port = ui->rx_port;
    path = ui->path;

    //UDP 생성 및 수신 포트 바인딩 (rx_port 값 사용)
    udp = new QUdpSocket(this);
    udp->bind(QHostAddress::Any, rx_port->text().toUShort(),
              QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    connect(send, &QPushButton::clicked, this, &MainWindow::on_send_clicked);
    connect(udp, &QUdpSocket::readyRead, this, &MainWindow::receive_img);

    // rx_port 입력 시 바인딩 갱신
    connect(rx_port, &QLineEdit::editingFinished, this, [=](){
        quint16 port = rx_port->text().toUShort();
        if (port > 0) {
            udp->close();
            udp->bind(QHostAddress::Any, port,
                      QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_send_clicked()
{
    QString img_path = path->text();

    //OpenCV 이미지 읽기
    Mat img = imread(img_path.toStdString(), IMREAD_COLOR);
    
    if (img.empty()) {
        // 함수 종료, 프로그램 종료되지 않음
        return;
    }

    //OpenCV의 mat을 jpg(JPEG)으로 인코딩 -> 벡터
    std::vector<uchar> buf;
    //사진 절반으로 압축
    std::vector<int> compression_params = {cv::IMWRITE_JPEG_QUALITY, 50};
    imencode(".jpg", img, buf, compression_params);

    //QByteArray로 변환
    QByteArray byteArray(reinterpret_cast<const char*>(buf.data()), buf.size());

    //전송할 IP와 포트 (tx_port 사용)
    QHostAddress addr(ip->text());
    quint16 port = tx_port->text().toUShort();
}

void MainWindow::receive_img()
{
    while (udp->hasPendingDatagrams()) {
        //데이터 저장 버퍼 준비
        QByteArray datagram;
        datagram.resize(int(udp->pendingDatagramSize()));

        //데이터 읽기
        udp->readDatagram(datagram.data(), datagram.size());

        //벡터 -> OpenCV 디코딩
        std::vector<uchar> buf(datagram.begin(), datagram.end());

        //JPEG -> OpenCV Mat 복원
        Mat img = imdecode(buf, IMREAD_COLOR);

        //Mat을 QLabel로 출력
        if (!img.empty()) {
            cvtColor(img, img, COLOR_BGR2RGB);
            QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
  	    //
            image->setPixmap(QPixmap::fromImage(qimg)); 
            image->setScaledContents(true);
         } else {
            return;
        }
    }
}

