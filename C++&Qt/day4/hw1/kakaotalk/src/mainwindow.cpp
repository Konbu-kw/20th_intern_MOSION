#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //창이름
    setWindowTitle("Kakaotalk");

    //이름 위치
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    //헤더파일 내의 멤버함수 생성 초기화
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    //QLineEdit UI 없이 코드 구현
    //서로의 이식을 쉽게 하기 위하여 UI 디자이너 없이 코드 구현하였습니다.
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    //추가 UI(사용자 편의 고려)
    //닉네임, 송수신 포트, 상대방 IP 입력란
    target_address = new QLineEdit(this);
    target_address->setPlaceholderText("Target IP");

    rx_port = new QLineEdit(this);
    rx_port->setPlaceholderText("Receive Port");
    rx_port->setValidator(new QIntValidator(1, 65535, this));

    tx_port = new QLineEdit(this);
    tx_port->setPlaceholderText("Transmit Port");
    tx_port->setValidator(new QIntValidator(1, 65535, this));

    nickname = new QLineEdit(this);
    nickname->setPlaceholderText("Nickname");

    //위젯 생성
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    inputLayout->addWidget(target_address);
    inputLayout->addWidget(rx_port);
    inputLayout->addWidget(tx_port);
    inputLayout->addWidget(nickname);

    layout->addWidget(new QLabel("Chat:"));
    layout->addWidget(chatDisplay);
    layout->addLayout(inputLayout);

    //통신 활성화를 위한 생성자
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 8081, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    //UDP 통신을 위한 각각의 버튼+기능 할당
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::receiveMessage);

    //수신 포트 변경 시 -> 추가 UI로 변경된 사항
    connect(rx_port, &QLineEdit::editingFinished, this, [=](){
        quint16 port = rx_port->text().toUShort();
        if(port > 0) {
            udpSocket->close();
            udpSocket->bind(QHostAddress::Any, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
            chatDisplay->append("Receive port changed to: " + QString::number(port));
        }
    });
}

//소멸자
MainWindow::~MainWindow() {}

void MainWindow::sendMessage()
{
    //QLineEdit으로 추가한 위젯 문자열을 text에 저장하여 송신
    QString text = messageInput->text();
    if (text.isEmpty()) return;

    QString ip = target_address->text();
    quint16 port = tx_port->text().toUShort();
    QString name = nickname->text();
    //닉네임 작성 안할 시 예외
    if(name.isEmpty()) name = "User";

    //포트 입력이 없을 시 에러 메세지 출력
    //바로 반환하여 오류 예방
    if(ip.isEmpty() || port == 0) {
        chatDisplay->append("Error: Set target IP and port");
        return;
    }

    //닉네임과 메시지를 문자열 하나로 결합
    //닉네임을 통한 서로의 대화 분류 가능
    QString fullMessage = name + ": " + text;

    QByteArray data = fullMessage.toUtf8();
    udpSocket->writeDatagram(data, QHostAddress(ip), port);

    //화면 출력
    chatDisplay->append(fullMessage);
    messageInput->clear();
}


void MainWindow::receiveMessage()
{
    //수신 포트 설정에 따라 문자열 수신
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString msg = QString::fromUtf8(datagram);
        chatDisplay->append(msg);
    }
}
