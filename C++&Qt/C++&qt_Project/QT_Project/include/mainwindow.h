
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
//UDP 통신
#include <QUdpSocket>
#include <QHostAddress>
//키 인식
#include <QKeyEvent>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
//지도 동적 할당
#include <QVector>
#include <QPoint>
//스킬 사용 큐
#include <deque>

//노드 구조체 -> 픽셀
struct Node {
    int x, y;
    int hp;
    int itemType; //순서대로 조작 반전, 기절, 연막
    bool obs;
    bool isItem;
};

//Arena
class Arena {
public:
    //size에 따른 동적할당 size^2
    int size;
    QVector<QVector<Node>> map;

    Arena(int n, double obsRatio = 0.0);
    ~Arena();

    void mapRandom(double obsRatio);
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_hostButton_clicked();
    void on_joinButton_clicked();
    void onUdpReadyRead();
    void onStartGame();
    //준비
    void on_readyButton_clicked();


private:
    Ui::MainWindow *ui;
    QHostAddress peerAddress;
    QUdpSocket* udpSocket;
    QGraphicsScene* scene;
    QTextEdit *time_pass;
    QTextEdit *wall_plus;
    QTextEdit *resultTextEdit;
    QPushButton* hostButton;
    QPushButton* joinButton;
    QPushButton* readyButton;
    QLineEdit *size_input;
    QTextEdit *item;
    QTextEdit *health;
    QTextEdit *log;
    Arena* arena;

    //기본 맵 크기 15^2
    //장애물 비율 30%
    int pixel;
    int size = 15;
    double obsRatio = 0.8;

    //호스트 및 게임 시작 판별
    bool isHost = false;
    bool gameStarted = false;

    //사전 지정 포트
    int sendPort;
    int recvPort;
    int wall_hard = 0;

    //이벤트 처리
    //각각 호스트, 게스트
    QPoint hostPos;
    QPoint guestPos;

    //Host 상태
    bool hostControlsReversed = false;
    bool hostBlinded = false;
    bool hostStun = false;
    int hostControlsReversedTime = 0;
    int hostBlindedTime = 0;
    int hostStunTime = 0;
    int hostHp= 20;
    int hostPt= 0;


    //Guest 상태
    bool guestControlsReversed = false;
    bool guestBlinded = false;
    bool guestStun = false;
    int guestControlsReversedTime = 0;
    int guestBlindedTime = 0;
    int guestStunTime = 0;
    int guestHp=20;
    int guestPt= 0;


    //스킬 타이머
    QTimer *skillTimer;
    // 플레이어 아이템 인벤토리
    std::deque<int> playerItems;

    //자기장 요소
    int fieldCycle = 10000;
    int fieldStep = 0;
    QTimer* fieldTimer = nullptr;

    //자기장 함수(하늘색 픽셀)
    bool isInsideField(int x, int y);
    void applyFieldEffect();

    //시간 경과 -> 벽 체력 가중치
    QTimer* wallTimer = nullptr;
    int wallCycle = 5000;
    int wallHpIncrement = 1;
    int wallHpMax = 16;
    //그에 대한 함수
    void increaseWallHp();

    //지도 생성 및 화면 갱신
    void placeItems();
    void generateMap();
    void updateScene();
    void switchToPlayScreen();
    void initGameScreen();
    void sendGameData(const QByteArray &data, QHostAddress addr, quint16 port);
    void updateItemDisplay();

    //이동 및 공격 동작
    void movePlayer(QPoint& pos, int dx, int dy);
    void handleAttack(bool isHostAttacking);

    //네트워크 통신 UDP
    void sendGameData(const QByteArray &data);
    void processReceivedData(const QByteArray &data);

    //직렬화 / 역직렬화
    QByteArray serializeMap();
    void deserializeMap(const QByteArray &data);

    void useItem();
    //추가 함수
    void sendMapData();

    //통신 보정용 함수
    //(유저2의 데이터 송신 오류 보정)
    void sendReadyState();
    void sendLog(const QString&);
    void endGame(const QString &resultMessage);
};

#endif // MAINWINDOW_H

