#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include <vector>
#include <limits>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//픽셀 노드
struct Node {
    int x, y;
    //장애물 배치 X
    bool obs = false;
    bool visited = false;
    //다익스트라용
    double g = std::numeric_limits<double>::infinity();
    double h = 0;
    Node* parent = nullptr;
};

//경로 탐색 클랙스
class PF {
public:
    PF(int n, double obsRatio);
    void map_random(double obsRatio);
    void searching();
    std::vector<Node*> sidenode(Node* node);
    double heuristic(Node* a, Node* b);
    bool dijkstra();
    bool aStar();
    std::vector<Node*> map_way();

    int size;
    std::vector<std::vector<Node>> map;
    Node* start;
    Node* goal;

    std::vector<Node*> openD;
    std::vector<Node*> openA;
    bool finishedD, finishedA;
};


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//%%마우스 클릭은 protected 영역
protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private slots:
    void on_B_Apply_clicked();
    void on_B_Play_clicked();
    void nextPixel();

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    PF* pfA;
    PF* pfD;
    QGraphicsScene* sceneA;
    QGraphicsScene* sceneD;
    int pixel;
    int click;
    bool play;

    void map_generating();
    void map_sight();
    void map_scaling();
    bool map_center(QObject* viewObj, const QPoint& pos, int &row, int &col);
};

#endif // MAINWINDOW_H
