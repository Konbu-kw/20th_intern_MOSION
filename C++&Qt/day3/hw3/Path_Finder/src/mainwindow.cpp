#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <ctime>

//PF 생성자
PF::PF(int n, double obsRatio) : size(n) {
    map.resize(size, std::vector<Node>(size));
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            map[r][c].x = c;
            map[r][c].y = r;
        }
    }
    map_random(obsRatio);
}

//장애물 배치
void PF::map_random(double obsRatio) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            map[r][c].obs = ((std::rand() % 100) < static_cast<int>(obsRatio * 100.0));
            map[r][c].visited = false;
            map[r][c].g = std::numeric_limits<double>::infinity();
            map[r][c].h = 0;
            map[r][c].parent = nullptr;
        }
    }
    start = nullptr;
    goal = nullptr;
}

void PF::searching() {
    openD.clear();
    openA.clear();
    finishedD = finishedA = false;
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            map[r][c].visited = false;
            map[r][c].g = std::numeric_limits<double>::infinity();
            map[r][c].h = 0;
            map[r][c].parent = nullptr;
        }
    }
    if (start && goal) {
        start->g = 0;
        start->h = heuristic(start, goal);
        openD.push_back(start);
        openA.push_back(start);
    }
}

std::vector<Node*> PF::sidenode(Node* node) {
    std::vector<Node*> out;
    static const int dx[4] = {0, 0, 1, -1};
    static const int dy[4] = {1, -1, 0, 0};
    for (int k = 0; k < 4; ++k) {
        int nr = node->y + dy[k];
        int nc = node->x + dx[k];
        if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
            if (!map[nr][nc].obs) out.push_back(&map[nr][nc]);
        }
    }
    return out;
}

double PF::heuristic(Node* a, Node* b) {
    int dx = a->x - b->x;
    if (dx < 0) dx = -dx;
    int dy = a->y - b->y;
    if (dy < 0) dy = -dy;

    return dx + dy;
}

bool PF::dijkstra() {
    if (finishedD) return true;
    if (openD.empty()) return true;

    auto it = std::min_element(openD.begin(), openD.end(),
                               [](Node* A, Node* B){ return A->g < B->g; });
    Node* cur = *it;
    openD.erase(it);

    if (cur->visited) return false;
    cur->visited = true;

    if (cur == goal) { finishedD = true; return true; }

    for (Node* nb : sidenode(cur)) {
        if (nb->visited) continue;
        double ng = cur->g + 1.0;
        if (ng < nb->g) {
            nb->g = ng;
            nb->parent = cur;
            openD.push_back(nb);
        }
    }
    return false;
}

bool PF::aStar() {
    if (finishedA) return true;
    if (openA.empty()) return true;

    auto it = std::min_element(openA.begin(), openA.end(),
                               [](Node* A, Node* B){ return (A->g + A->h) < (B->g + B->h); });
    Node* cur = *it;
    openA.erase(it);

    if (cur->visited) return false;
    cur->visited = true;

    if (cur == goal) { finishedA = true; return true; }

    for (Node* nb : sidenode(cur)) {
        if (nb->visited) continue;
        double ng = cur->g + 1.0;
        if (ng < nb->g) {
            nb->g = ng;
            nb->h = heuristic(nb, goal);
            nb->parent = cur;
            openA.push_back(nb);
        }
    }
    return false;
}

std::vector<Node*> PF::map_way() {
    std::vector<Node*> path;
    if (!goal) return path;
    Node* cur = goal;
    while (cur) {
        path.push_back(cur);
        if (cur == start) break;
        cur = cur->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    pfA(nullptr),
    pfD(nullptr),
    sceneA(new QGraphicsScene(this)),
    sceneD(new QGraphicsScene(this)),
    pixel(20),
    click(0),
    play(false)
{
    ui->setupUi(this);
    ui->A->setScene(sceneA);
    ui->D->setScene(sceneD);

    ui->A->viewport()->installEventFilter(this);
    ui->D->viewport()->installEventFilter(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::nextPixel);
}

MainWindow::~MainWindow() {
    delete ui;
    delete pfA;
    delete pfD;
}

bool MainWindow::map_center(QObject* viewObj, const QPoint& pos, int &row, int &col) {
    QWidget* viewport = qobject_cast<QWidget*>(viewObj);
    if (!viewport) return false;
    QGraphicsView* view = qobject_cast<QGraphicsView*>(viewport->parentWidget());
    if (!view) return false;
    QPointF scenePt = view->mapToScene(pos);
    int c = static_cast<int>(scenePt.x()) / pixel;
    int r = static_cast<int>(scenePt.y()) / pixel;
    if (!pfA) return false;
    if (r < 0 || c < 0 || r >= pfA->size || c >= pfA->size) return false;
    row = r; col = c;
    return true;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* ev) {
    if (ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent* me = static_cast<QMouseEvent*>(ev);
        int r, c;
        if ((obj == ui->A->viewport() || obj == ui->D->viewport()) &&
            map_center(obj, me->pos(), r, c)) {
            if (!pfA || !pfD) return true;
            if (me->button() == Qt::LeftButton) {
                if (click == 0) {
                    pfA->start = &pfA->map[r][c];
                    pfD->start = &pfD->map[r][c];
                    click = 1;
                } else if (click == 1) {
                    pfA->goal = &pfA->map[r][c];
                    pfD->goal = &pfD->map[r][c];
                    click = 2;
                }
            } else if (me->button() == Qt::RightButton) {
                pfA->map[r][c].obs = true;
                pfD->map[r][c].obs = true;
            }
            map_generating();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, ev);
}

void MainWindow::on_B_Apply_clicked() {
    bool ok1=false, ok2=false;
    int n = ui->mapsize->text().toInt(&ok1);
    int obsPercent = ui->Obs->text().toInt(&ok2);

    double obsRatio = static_cast<double>(obsPercent) / 100.0;

    delete pfA; delete pfD;
    pfA = new PF(n, obsRatio);
    pfD = new PF(n, obsRatio);

    click = 0;
    play = false;
    timer->stop();

    map_generating();
}

void MainWindow::on_B_Play_clicked() {
    if (!pfA || !pfD) return;
    if (!pfA->start || !pfA->goal) return;

    pfA->searching();
    pfD->searching();
    play = true;
    timer->start(30);
}

void MainWindow::map_generating() {
    if (!pfA || !pfD) return;
    sceneA->clear();
    sceneD->clear();
    int n = pfA->size;

    if(n > 0 && n < std::numeric_limits<int>::max()){
        n = (int)n;
    }
    else{
        return;
    }

    sceneA->setSceneRect(0,0,n*pixel,n*pixel);
    sceneD->setSceneRect(0,0,n*pixel,n*pixel);

    for (int r=0;r<n;++r) {
        for (int c=0;c<n;++c) {
            Node* na = &pfA->map[r][c];
            Node* nd = &pfD->map[r][c];

            QColor colA = Qt::white;
            QColor colD = Qt::white;

            if (na->obs) colA = Qt::black;
            else if (na == pfA->start) colA = Qt::green;
            else if (na == pfA->goal) colA = Qt::red;

            if (nd->obs) colD = Qt::black;
            else if (nd == pfD->start) colD = Qt::green;
            else if (nd == pfD->goal) colD = Qt::red;

            sceneA->addRect(c*pixel,r*pixel,pixel,pixel,QPen(Qt::gray),QBrush(colA));
            sceneD->addRect(c*pixel,r*pixel,pixel,pixel,QPen(Qt::gray),QBrush(colD));
        }
    }
    map_sight();
    map_scaling();
}

void MainWindow::map_sight() {
    if (!pfA || !pfD) return;
    int n = pfA->size;

    for (int r=0;r<n;++r) {
        for (int c=0;c<n;++c) {
            Node* na = &pfA->map[r][c];
            Node* nd = &pfD->map[r][c];

            if (na->visited && na != pfA->start && na != pfA->goal)
                sceneA->addRect(c*pixel,r*pixel,pixel,pixel,QPen(Qt::gray),QBrush(QColor(173,216,230)));
            if (nd->visited && nd != pfD->start && nd != pfD->goal)
                sceneD->addRect(c*pixel,r*pixel,pixel,pixel,QPen(Qt::gray),QBrush(QColor(255,228,181)));
        }
    }

    auto pathA = pfA->map_way();
    for (auto node : pathA) {
        if (node != pfA->start && node != pfA->goal)
            sceneA->addRect(node->x*pixel,node->y*pixel,pixel,pixel,QPen(Qt::gray),QBrush(Qt::yellow));
    }
    auto pathD = pfD->map_way();
    for (auto node : pathD) {
        if (node != pfD->start && node != pfD->goal)
            sceneD->addRect(node->x*pixel,node->y*pixel,pixel,pixel,QPen(Qt::gray),QBrush(Qt::yellow));
    }
}

void MainWindow::map_scaling() {
    if (!sceneA->sceneRect().isEmpty()){
        ui->A->fitInView(sceneA->sceneRect(), Qt::KeepAspectRatio);
    }
    if (!sceneD->sceneRect().isEmpty()){
        ui->D->fitInView(sceneD->sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::nextPixel() {
    if (!play) return;

    bool doneA = pfA->aStar();
    bool doneD = pfD->dijkstra();

    map_generating();
    map_sight();

    if (doneA && doneD) {
        timer->stop();
        play = false;
    }
}
