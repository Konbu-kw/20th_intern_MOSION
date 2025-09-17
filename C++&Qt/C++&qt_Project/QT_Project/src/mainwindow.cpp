#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>

#include <QGraphicsRectItem>
//그림모듈
#include <QBrush>
#include <QPen>
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>
#include <QNetworkInterface>
#include <QMessageBox>
#include <algorithm>

//열거 데이터로 지도 갱신 및 스킬과 메세지 로그 갱신
enum MsgType : quint8 {
    //지도 정보
    MSG_REQ_MAP = 1,
    MSG_MAP = 2,
    MSG_HOST_POS = 3,
    MSG_GUEST_POS = 4,
    MSG_READY = 5,
    MSG_START = 6,

    //스킬 사용 여부
    MSG_GUEST_STUN = 10,
    MSG_HOST_STUN = 11,
    MSG_GUEST_BLIND = 12,
    MSG_HOST_BLIND = 13,
    MSG_GUEST_CTRL_REV = 14,
    MSG_HOST_CTRL_REV = 15,
    MSG_ITEM_PICKED = 20,

    //공격 및 다른 기믹들
    MSG_ATTACK = 21,
    MSG_LOG = 22,
    MSG_ITEM_USED = 23,
    MSG_WALL_HP = 24
};

//준비 상태 확인용(불리언)
bool localReady = false;
bool remoteReady = false;

//전장 구현
Arena::Arena(int n, double obsRatio) : size(n) {
    map.resize(size);
    for (int r = 0; r < size; ++r) {
        map[r].resize(size);
        for (int c = 0; c < size; ++c) {
            map[r][c].x = c;
            map[r][c].y = r;
            map[r][c].obs = false;
            map[r][c].hp = 0;
            map[r][c].isItem = false;
            map[r][c].itemType = 0;
        }
    }
    mapRandom(obsRatio);
}

Arena::~Arena() {
    //소멸자
}

void Arena::mapRandom(double obsRatio) {
    QRandomGenerator *g = QRandomGenerator::global();
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            //스폰 지점은 장애물 없음
            if ((r == 0 && c == 0) || (r == size - 1 && c == size - 1)) continue;

            if (g->bounded(100) < int(obsRatio * 100.0)) {
                int pick = g->bounded(100);
                if (pick < 50) map[r][c].hp = 3;
                else if (pick < 80) map[r][c].hp = 5;
                else if (pick < 90) map[r][c].hp = 10;

                else map[r][c].hp = -1; //무적 벽
                map[r][c].obs = true;   //설치
            }
        }
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    udpSocket(nullptr),
    arena(nullptr),
    scene(new QGraphicsScene(this)),
    pixel(24), //가장 가시가 좋음
    sendPort(0),
    recvPort(0),
    hostPos(0, 0),
    guestPos(size - 1, size - 1)
{
    //상속
    ui->setupUi(this);

    hostButton = ui->hostButton;
    joinButton = ui->joinButton;
    readyButton = ui->readyButton;
    item = ui->item;
    health = ui->health;
    log = ui->log;
    size_input = ui->size_input;
    resultTextEdit = ui->resultTextEdit;

    ui->graphicsView->setScene(scene);
    log->clear();

    connect(hostButton, &QPushButton::clicked, this, &MainWindow::on_hostButton_clicked);
    connect(joinButton, &QPushButton::clicked, this, &MainWindow::on_joinButton_clicked);
    connect(readyButton, &QPushButton::clicked, this, &MainWindow::on_readyButton_clicked);

    skillTimer = new QTimer(this);
    connect(skillTimer, &QTimer::timeout, this, [this]() {
        if (hostControlsReversedTime > 0) {
            hostControlsReversedTime -= 100;
            if (hostControlsReversedTime <= 0) {
                hostControlsReversed = false;
                sendLog("Host 조작 반전 해제");
            }
        }
        if (hostBlindedTime > 0) {
            hostBlindedTime -= 100;
            if (hostBlindedTime <= 0) {
                hostBlinded = false;
                sendLog("Host 연막 해제");
                updateScene();
            }
        }
        if (hostStunTime > 0) {
            hostStunTime -= 100;
            if (hostStunTime <= 0) {
                hostStun = false;
                sendLog("Host 기절 해제");
            }
        }

        if (guestControlsReversedTime > 0) {
            guestControlsReversedTime -= 100;
            if (guestControlsReversedTime <= 0) {
                guestControlsReversed = false;
                sendLog("Guest 조작 반전 해제");
            }
        }
        if (guestBlindedTime > 0) {
            guestBlindedTime -= 100;
            if (guestBlindedTime <= 0) {
                guestBlinded = false;
                sendLog("Guest 연막 해제");
                updateScene();
            }
        }
        if (guestStunTime > 0) {
            guestStunTime -= 100;
            if (guestStunTime <= 0) {
                guestStun = false;
                sendLog("Guest 기절 해제");
            }
        }
    });
    skillTimer->start(100);

    wallTimer = new QTimer(this);
    connect(wallTimer, &QTimer::timeout, this, &MainWindow::increaseWallHp);

    fieldTimer = new QTimer(this);
    connect(fieldTimer, &QTimer::timeout, this, &MainWindow::applyFieldEffect);
}

//소멸자
MainWindow::~MainWindow() {
    if (udpSocket) {
        udpSocket->close();
        delete udpSocket;
    }
    if (arena) delete arena;
    delete scene;
    delete ui;
}

void MainWindow::generateMap() {
    //초기화, 생성
    if (arena) { delete arena; arena = nullptr; }
    arena = new Arena(size, obsRatio);

    //유저 배치
    hostPos = QPoint(0,0);
    guestPos = QPoint(size - 1, size - 1);
    placeItems();
    updateScene();
}

void MainWindow::placeItems() {
    if (!arena) return;

    QVector<QPoint> candidates;
    for (int r = 0; r < arena->size; ++r) {
        for (int c = 0; c < arena->size; ++c) {
            int hp = arena->map[r][c].hp;
            //벽 인식
            //배열의 벽 체력 인식 -> 해당 위치 아이템 배치
            if (hp == 3 || hp == 5 || hp == 10) {
                candidates.append(QPoint(c, r));
            }
        }
    }
    //아이템 자리 갯수 확인
    int total = candidates.size();
    if (total == 0) return;

    //아이템 갯수 지정(너무 적으면 적어도 1개 설치)
    int numItems = std::max(1, total * 30 / 100);

    //위치 섞기 난수 사용
    std::shuffle(candidates.begin(), candidates.end(), *QRandomGenerator::global());
    QVector<QPoint> itemPositions = candidates.mid(0, numItems);

    //일일이 노드 아이템 삽입
    for (int i = 0; i < itemPositions.size(); ++i) {
        QPoint p = itemPositions[i];
        Node &node = arena->map[p.y()][p.x()];
        node.isItem = true;

        //아이템 랜덤 지정(1~4)
        int pick = QRandomGenerator::global()->bounded(1, 4);
        node.itemType = pick;
    }
}

void MainWindow::movePlayer(QPoint &pos, int dx, int dy) {
    //지도 안 만들었을때
    if (!arena) return;
    int nx = pos.x() + dx;
    int ny = pos.y() + dy;

    //지도 범위 이상 조좍 불가능 예외처리
    if (nx < 0 || nx >= size || ny < 0 || ny >= size) return;

    //유저 충돌 인식 알고리즘

    QPoint other = (&pos == &hostPos) ? guestPos : hostPos;

    //공격 판정
    if (other == QPoint(nx, ny)) {
        handleAttack(&pos == &hostPos);
        //함수 빠져나가서 아래 동작 X
        return;
    }

    //유저 충돌 아닌 경우
    Node &node = arena->map[ny][nx];

    //아이템 벽 파괴 인식 알고리즘, 아이템 획득
    if (node.isItem && node.hp == 0) {
        playerItems.push_back(node.itemType);
        sendLog(QString("아이템 획득: 타입 %1").arg(node.itemType));
        //아이템 상태 초기화
        node.isItem = false;
        node.itemType = 0;

        //아이템 정보 상태 패킷 통신 및 갱신
        QByteArray b;
        QDataStream out(&b, QIODevice::WriteOnly);//b에 작성할 out내용
        //정보 출력
        out.setVersion(QDataStream::Qt_6_0);
        out << quint8(MSG_ITEM_PICKED) << qint32(nx) << qint32(ny);
        //업데이트
        sendGameData(b);

        updateItemDisplay();
    }

    //벽 상태 판별
    if (node.hp != 0) {
        if (node.hp != -1) {
            node.hp -= 1;
            if (node.hp <= 0) {
                node.hp = 0;
                node.obs = false;
            }
        }
        updateScene();

        //이하 동일
        QByteArray b;
        QDataStream out(&b, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << quint8(MSG_WALL_HP) << qint32(nx) << qint32(ny) << qint32(node.hp);
        sendGameData(b);

        return;
    }

    //좌표 지정
    pos.setX(nx);
    pos.setY(ny);


    //이것도 동일
    QByteArray b;
    QDataStream out(&b, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    if (isHost) out << quint8(MSG_HOST_POS) << qint32(pos.x()) << qint32(pos.y());
    else out << quint8(MSG_GUEST_POS) << qint32(pos.x()) << qint32(pos.y());

    sendGameData(b);

    updateScene();
}


QByteArray MainWindow::serializeMap() {
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << quint8(MSG_MAP);
    out << quint32(arena->size);
    for (int r=0;r<arena->size;++r) {
        for (int c=0;c<arena->size;++c) {
            const Node &n = arena->map[r][c];
            out << qint32(n.hp);
            out << bool(n.isItem);
            out << qint32(n.itemType);
        }
    }
    return ba;
}

void MainWindow::deserializeMap(const QByteArray &data) {
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_6_0);
    quint8 type;
    in >> type;
    if (type != MSG_MAP) return;
    quint32 s;
    in >> s;
    if (arena) { delete arena; arena = nullptr; }
    size = int(s);
    arena = new Arena(0, 0.0);
    arena->size = size;
    arena->map.resize(size);
    for (int r=0;r<size;++r) {
        arena->map[r].resize(size);
        for (int c=0;c<size;++c) {
            qint32 hp;
            bool isItem;
            qint32 itemType;
            in >> hp >> isItem >> itemType;
            Node &n = arena->map[r][c];
            n.x = c;
            n.y = r;
            n.hp = hp;
            n.obs = (hp != 0);
            n.isItem = isItem;
            n.itemType = itemType;
        }
    }
    if (!gameStarted) {
        hostPos = QPoint(0,0);
        guestPos = QPoint(size-1, size-1);
    }
    updateScene();
}

//체력에 따른 벽 색깔
QColor hpToColor(int hp) {
    if (hp == -1) return QColor(0,160,0);
    if (hp >= 12) return QColor(0,255,0);
    if (hp >= 10) return QColor(0,200,0);
    if (hp >= 8) return QColor(160,200,0);
    if (hp >= 5) return QColor(255,200,0);
    if (hp >= 3) return QColor(255,160,80);
    if (hp >= 1) return QColor(255,120,80);
    return QColor(0,0,0,0);
}

//화면 업데이트
void MainWindow::updateScene() {
    if (!arena) return;
    scene->clear();
    scene->setSceneRect(0, 0, arena->size * pixel, arena->size * pixel);

    //모든 위치 벡터 확인
    for (int r=0; r<arena->size; ++r) {
        for (int c=0; c<arena->size; ++c) {
            const Node &n = arena->map[r][c];
            QColor color;
            QPen pen(Qt::gray);

            //벽  체력에 따른 색깔 지정
            if (n.hp != 0) {
                color = hpToColor(n.hp);
                //아이템 속성 벽이라면 끝에 검정으로 시각화
            } else if (n.isItem) {
                color = Qt::black;
            } else {
                //배경색 맞추기
                color = Qt::transparent;
            }

            //자기장 벡터와 겹치면 색깔 민트색
            if (isInsideField(c, r)) {
                color = QColor(173, 216, 230, 100);
            }

            //연막 스킬 처리용
            if (isHost) {
                if (hostBlinded) {
                    color = Qt::black;
                }
            } else {
                if (guestBlinded) {
                    color = Qt::black;
                }
            }

            //픽셀 그리기(사각형Rectangle)
            scene->addRect(c*pixel, r*pixel, pixel, pixel, pen, QBrush(color));

            if (n.hp != 0) {
                //디버깅용 코드
                //벽 체력 표시 및 무적 판별 벽 표시
                QGraphicsTextItem* hpText = new QGraphicsTextItem(QString::number(n.hp));
                hpText->setPos(c * pixel, r * pixel);
                hpText->setDefaultTextColor(Qt::red);
                hpText->setFont(QFont("Arial", 8));
                scene->addItem(hpText);
            }
        }
    }

    QBrush hBrush(Qt::red);
    QBrush gBrush(QColor(139,69,19));

    QRectF hr(hostPos.x()*pixel, hostPos.y()*pixel, pixel, pixel);
    scene->addRect(hr, QPen(Qt::gray), hBrush);

    QRectF gr(guestPos.x()*pixel, guestPos.y()*pixel, pixel, pixel);
    scene->addRect(gr, QPen(Qt::gray), gBrush);

    if (ui->graphicsView->scene())
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    health->setText(QString("Host HP: %1\nGuest HP: %2").arg(hostHp).arg(guestHp));
}

void MainWindow::sendGameData(const QByteArray &data) {
    if (peerAddress.isNull()) {
        return;
    }
    if (!udpSocket) return;
    //데이터그램 전송
    udpSocket->writeDatagram(data, peerAddress, quint16(sendPort));
}

void MainWindow::on_hostButton_clicked() {
    isHost = true;
    //**포트는 미리 지정**
    sendPort = 45454;
    recvPort = 8081;

    //Qt 모듈로 저장되는 size 정수화해서 저장
    size = size_input->text().toInt();

    //지도 크기 예외처리
    if (size <= 0) {

        QMessageBox::warning(this, "경고", "지도 크기가 올바르지 않음");

        return;

    }else if(size >0 && size <=15){
        size = 15;
    }
    else if(size >= 30){
        size = 30;
    }
    //여기까지 지도 크기 예외처리

    if (!udpSocket) {
        //통신 생성
        udpSocket = new QUdpSocket(this);
        connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::onUdpReadyRead);
    } else {
        udpSocket->close();
    }

    if (!udpSocket->bind(QHostAddress::Any, recvPort)) {
        sendLog("바인드 실패(호스트)");
        return;
    }

    //로그 출력 - 주소만
    sendLog("바인드 성공(호스트). 로컬 IP 목록:");
    auto addrs = QNetworkInterface::allAddresses();
    for (QHostAddress &a : addrs) {
        //IPv4 주소 기준
        if (a.protocol() == QAbstractSocket::IPv4Protocol && !a.isLoopback()) {
            sendLog(QString(" - %1").arg(a.toString()));
        }
    }

    generateMap();
    updateScene();

    switchToPlayScreen();
    initGameScreen();
}

void MainWindow::on_joinButton_clicked() {
    isHost = false;
    sendPort = 8081;
    recvPort = 45454;

    //Qt 모듈로 입력되는 ip_input값으로 대상 주소 지정
    QString ip = ui->ip_input->text().trimmed();

    //입력된 IP 주소가 유효한지 확인
    peerAddress = QHostAddress(ip);
    if (peerAddress.isNull() || ip.isEmpty()) {
        QMessageBox::warning(this, "경고", "올바른 IP 주소를 입력하세요.");
        return;
    }

    peerAddress = QHostAddress(ip);

    if (!udpSocket) {
        udpSocket = new QUdpSocket(this);
        connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::onUdpReadyRead);
    } else {
        udpSocket->close();
    }

    if (!udpSocket->bind(QHostAddress::Any, recvPort)) {
        sendLog("바인드 실패(JOIN)");
        return;
    }

    sendLog(QString("바인드 성공(JOIN). %1, REQ_MAP 송신").arg(sendPort));

    //이하 동일
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << quint8(MSG_REQ_MAP);

    udpSocket->writeDatagram(ba, peerAddress, quint16(sendPort));

    switchToPlayScreen();
    initGameScreen();
}


void MainWindow::onUdpReadyRead() {
    while (udpSocket && udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort = 0;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if (peerAddress.isNull()) {
            peerAddress = sender;
        }

        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_6_0);

        quint8 type;
        in >> type;

        qint32 t;

        switch(type) {
        case MSG_GUEST_STUN: in >> t; guestStun=true; guestStunTime=t; updateScene(); break;
        case MSG_GUEST_BLIND: in >> t; guestBlinded=true; guestBlindedTime=t; updateScene(); break;
        case MSG_GUEST_CTRL_REV: in >> t; guestControlsReversed=true; guestControlsReversedTime=t; updateScene(); break;

        case MSG_HOST_STUN: in >> t; hostStun=true; hostStunTime=t; updateScene(); break;
        case MSG_HOST_BLIND: in >> t; hostBlinded=true; hostBlindedTime=t; updateScene(); break;
        case MSG_HOST_CTRL_REV: in >> t; hostControlsReversed=true; hostControlsReversedTime=t; updateScene(); break;

        case MSG_REQ_MAP:
            if (isHost && arena) {
                QByteArray mapBa = serializeMap();
                sendGameData(mapBa, peerAddress, quint16(sendPort));
            }
            break;

        case MSG_MAP:
            deserializeMap(datagram);
            updateScene();
            break;

        case MSG_HOST_POS: {
            qint32 x,y; in >> x >> y; hostPos = QPoint(x,y); updateScene(); break;
        }
        case MSG_GUEST_POS: {
            qint32 x,y; in >> x >> y; guestPos = QPoint(x,y); updateScene(); break;
        }
        case MSG_READY:
            remoteReady = true;
            sendLog("준비 완료1/2");
            if (localReady && remoteReady) {
                onStartGame();
            }
            break;
        case MSG_START:
            gameStarted = true;
            sendLog("게임 시작");
            wallTimer->start(wallCycle);
            fieldTimer->start(fieldCycle);
            break;

        case MSG_ITEM_PICKED: {
            qint32 x, y;
            in >> x >> y;
            if (arena) {
                Node &node = arena->map[y][x];
                node.isItem = false;
                node.itemType = 0;
                updateScene();
            }
            break;
        }

        case MSG_ITEM_USED: {
            qint32 itemType;
            in >> itemType;
            if (isHost) {
                switch(itemType) {
                case 1: hostControlsReversed = true; hostControlsReversedTime = 3000; sendLog("Guest, 조작 반전 사용!!"); break;
                case 2: hostStun = true; hostStunTime = 3000; sendLog("Guest, 기절 사용!!"); break;
                case 3: hostBlinded = true; hostBlindedTime = 5000; sendLog("Guest, 블라인드 사용!!"); break;
                }
            } else {
                switch(itemType) {
                case 1: guestControlsReversed = true; guestControlsReversedTime = 3000; sendLog("Host, 조작 반전 사용!!"); break;
                case 2: guestStun = true; guestStunTime = 3000; sendLog("Host, 기절 사용!!"); break;
                case 3: guestBlinded = true; guestBlindedTime = 5000; sendLog("Host, 연막 사용!!"); break;
                }
            }
            break;
        }

        case MSG_ATTACK: {
            qint32 newHp;
            quint8 target;
            in >> newHp >> target;
            if (target == 0) {
                guestHp = newHp;
            }
            else {
                hostHp = newHp;
            }

            sendLog(QString("%1 피격! HP: %2")
                        .arg(target == 0 ? "Guest" : "Host").arg(newHp));
            updateScene();
            break;
        }

        case MSG_WALL_HP: {
            qint32 x, y, hp;
            in >> x >> y >> hp;
            if (arena) {
                arena->map[y][x].hp = hp;
                if(hp == 0) arena->map[y][x].obs = false;
                updateScene();
            }
            break;
        }

        case MSG_LOG: {
            if (sender != QHostAddress(QHostAddress::LocalHost)) {
                QString message;
                in >> message;
                log->append(message);
            }
            break;
        }

        default:
            break;
        }
    }
}
void MainWindow::on_readyButton_clicked() {
    if (!udpSocket) {
        QMessageBox::warning(this, "오류", "UDP 통신이 준비되지 않음");
        return;
    }
    readyButton->hide();

    localReady = true;
    sendLog("준비완료1/2");
    sendReadyState();

    if (localReady && remoteReady) {
        onStartGame();
    }
}
//준비 및 메세지 전송용 함수
void MainWindow::sendReadyState() {
    QByteArray b;
    QDataStream out(&b, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << quint8(MSG_READY);
    sendGameData(b);
}

void MainWindow::onStartGame() {
    gameStarted = true;

    QByteArray s;
    QDataStream o(&s, QIODevice::WriteOnly);
    o.setVersion(QDataStream::Qt_6_0);
    o << quint8(MSG_START);
    sendGameData(s);

    sendLog("준비 완료\n게임 시작");

    wallTimer->start(wallCycle);
    fieldTimer->start(fieldCycle);
}

void MainWindow::handleAttack(bool isHostAttacking) {
    QPoint attacker = isHostAttacking ? hostPos : guestPos;
    QPoint target = isHostAttacking ? guestPos : hostPos;
    int dx = qAbs(attacker.x() - target.x());
    int dy = qAbs(attacker.y() - target.y());

    if ((dx + dy) == 1) {
        if (isHostAttacking) {
            guestHp--;
            sendLog(QString("Host공격: Guest체력 -1(%1)").arg(guestHp));

            QByteArray b;
            QDataStream out(&b, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            out << quint8(MSG_ATTACK) << qint32(guestHp) << quint8(0);
            sendGameData(b);

        } else {
            hostHp--;
            sendLog(QString("Guest공격: Host체력 -1(%1)").arg(hostHp));

            QByteArray b;
            QDataStream out(&b, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            out << quint8(MSG_ATTACK) << qint32(hostHp) << quint8(1);
            sendGameData(b);
        }
    }

    updateScene();

    if (hostHp <= 0) {
        endGame("Guest 승");
    } else if (guestHp <= 0) {
        endGame("HOST 승");
    }
}

//아이템 상태 표시
void MainWindow::updateItemDisplay() {
    QStringList names;
    for (int i = 0; i < playerItems.size(); ++i) {
        QString itemName;
        switch (playerItems[i]) {
        case 1: itemName = "조작 반전"; break;
        case 2: itemName = "기절"; break;
        case 3: itemName = "연막"; break;
        default: itemName = "오류"; break;
        }
        names << QString("%1: %2").arg(i+1).arg(itemName);
    }

    if (names.isEmpty())
        item->setText("비어있음");
    else
        item->setText(QString("목록:\n%1").arg(names.join("\n")));
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (!gameStarted) {
        return;
    }

    if (event->isAutoRepeat()) {
        return;
    }

    if (!gameStarted) {
        return;
    }

    if (!arena) return;

    if (isHost) {
        if (hostStun) return;
    } else {
        if (guestStun) return;
    }
    //기절이면 위 과정 생략

    //왼쪽 Alt가 스킬 사용
    if (event->key() == Qt::Key_Alt) {
        useItem();
        return;
    }

    //Q는 아이템 목록 정렬용, 디큐
    if (event->key() == Qt::Key_Q) {
        if (playerItems.size() > 1) {
            std::rotate(playerItems.rbegin(), playerItems.rbegin() + 1, playerItems.rend());
            sendLog("아이템 순서 변경 (Q)");
            updateItemDisplay();
        }
    }


    //조작 역전 스킬 사용 시
    int dx=0, dy=0;

    //불리언 변수 1
    bool isHostReversed = isHost && hostControlsReversed;
    bool isGuestReversed = !isHost && guestControlsReversed;

    //입력키에 따른 조작 반대 적용
    if (event->key() == (isHostReversed || isGuestReversed ? Qt::Key_S : Qt::Key_W)) dy = -1;
    else if (event->key() == (isHostReversed || isGuestReversed ? Qt::Key_W : Qt::Key_S)) dy = 1;
    else if (event->key() == (isHostReversed || isGuestReversed ? Qt::Key_D : Qt::Key_A)) dx = -1;
    else if (event->key() == (isHostReversed || isGuestReversed ? Qt::Key_A : Qt::Key_D)) dx = 1;
    else if (event->key() == Qt::Key_Space) {
        handleAttack(isHost);
        return;
    } else {
        QMainWindow::keyPressEvent(event);
        return;
    }

    //스킬 적용 대상자 식별
    if (isHost) {
        movePlayer(hostPos, dx, dy);
    }
    else {
        movePlayer(guestPos, dx, dy);
    }
}

//기타 데이터 송수신용 함수들
void MainWindow::sendGameData(const QByteArray &data, QHostAddress addr, quint16 port) {
    if (!udpSocket) return;
    udpSocket->writeDatagram(data, addr, port);
}

void MainWindow::processReceivedData(const QByteArray &data) {
    Q_UNUSED(data);
}
//스택 위젯 사용, 타이틀 화면 -> 플레이 화면 -> 가능하면 마지막 화면
//각각의 윈도우
void MainWindow::switchToPlayScreen() {
    ui->stackedWidget->setCurrentWidget(ui->play);
}
//시작화면
void MainWindow::initGameScreen() {
    if (!ui->graphicsView->scene())
        ui->graphicsView->setScene(scene);
    if (arena)
        ui->graphicsView->scene()->setSceneRect(0,0,arena->size*pixel,arena->size*pixel);
}

//아이템 사용 함수
void MainWindow::useItem() {
    if (playerItems.empty() || !udpSocket || peerAddress.isNull()) return;

    //아이템 번호(1~4)
    int itemType = playerItems.front();
    playerItems.pop_front();

    //출력
    QByteArray b;
    QDataStream out(&b, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    out << quint8(MSG_ITEM_USED) << qint32(itemType);

    //대상자 및 적용 시간
    if (isHost) {
        switch (itemType) {
        case 1: guestControlsReversed = true; guestControlsReversedTime = 3000; sendLog("Host, 조작 반전 사용!!"); break;
        case 2: guestStun = true; guestStunTime = 3000; sendLog("Host, 기절 사용!!"); break;
        case 3: guestBlinded = true; guestBlindedTime = 5000; sendLog("Host, 연막 사용!!"); break;
        }
    } else {
        switch (itemType) {
        case 1: hostControlsReversed = true; hostControlsReversedTime = 3000; sendLog("Guest, 조작 반전 사용!!"); break;
        case 2: hostStun = true; hostStunTime = 3000; sendLog("Guest, 기절 사용!!"); break;
        case 3: hostBlinded = true; hostBlindedTime = 5000; sendLog("Guest, 연막 사용!!"); break;
        }
    }

    sendGameData(b);

    updateItemDisplay();
}

//지도 공유
void MainWindow::sendMapData() {
    if (isHost && arena) {
        QByteArray mapBa = serializeMap();
        sendGameData(mapBa, peerAddress, quint16(sendPort));
    }
}

//로그 공유
void MainWindow::sendLog(const QString &message) {
    log->append(message);
    QByteArray b;
    QDataStream out(&b, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << quint8(MSG_LOG) << message;
    sendGameData(b);
}

//
bool MainWindow::isInsideField(int x, int y) {
    int currentSize = size - fieldStep;
    int offset = fieldStep / 2;
    return x < offset || x >= offset + currentSize || y < offset || y >= offset + currentSize;
}

void MainWindow::applyFieldEffect() {
    //한칸씩 확장
    fieldStep++;

    if (isHost) {
        if (isInsideField(hostPos.x(), hostPos.y())) {
            hostHp -= 6;
            sendLog(QString("Host, 구역 피해, HP: %1").arg(hostHp));
            QByteArray b;
            QDataStream out(&b, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            out << quint8(MSG_ATTACK) << qint32(hostHp) << quint8(1);
            sendGameData(b);
        }
    } else {
        if (isInsideField(guestPos.x(), guestPos.y())) {
            guestHp -= 6;
            sendLog(QString("Guest, 구역 피해, HP: %1").arg(guestHp));
            QByteArray b;
            QDataStream out(&b, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            out << quint8(MSG_ATTACK) << qint32(guestHp) << quint8(0);
            sendGameData(b);
        }
    }

    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            //벽돌 좌표 = 자기장 좌표?
            int currentSize = size - fieldStep;
            int offset = fieldStep / 2;
            if (c< offset || c >= offset + currentSize || r < offset || r >= offset + currentSize) {
                Node &node = arena->map[r][c];
                //벽이 존재-> HP를 0으로 설정
                if (node.hp != 0) {
                    node.hp = 0;
                    node.obs = false;
                }
            }
        }
    }

    updateScene();

    //유저 사망 판별 -> 게임 종료
    if (hostHp <= 0 || guestHp <= 0) {
        endGame(hostHp <= 0 ? "Guest 승" : "Host 승");
        fieldTimer->stop();
    }
}

//추가 함수 - 시간 지나면 벽 체력 증가
void MainWindow::increaseWallHp() {
    if (!arena || !isHost) return;

    int currentTotalWallHp = 0;

    for (int r = 0; r < arena->size; ++r) {
        for (int c = 0; c < arena->size; ++c) {
            Node &n = arena->map[r][c];
            if (n.hp > 0 && n.hp < wallHpMax) {
                n.hp += wallHpIncrement;
            }
            if (n.hp > 0) {
                currentTotalWallHp += n.hp;
            }
        }
    }

    QByteArray b = serializeMap();
    b[0] = MSG_MAP;
    sendGameData(b);

    updateScene();
}
void MainWindow::endGame(const QString &resultMessage) {
    //맵 업데이트 타이머 정지
    if (wallTimer->isActive()) {
        wallTimer->stop();
    }
    //자기장 타이머 정지
    if (fieldTimer->isActive()) {
        fieldTimer->stop();
    }
    // 스킬 타이머 정지
    if (skillTimer->isActive()) {
        skillTimer->stop();
    }

    //결과 메시지 설정
    if (resultTextEdit) {
        resultTextEdit->setPlainText(resultMessage);
    }
    //윈도우 변경
    ui->stackedWidget->setCurrentWidget(ui->end);
}
