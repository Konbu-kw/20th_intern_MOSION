#include "mainwindow.h"
#include "ui_mainwindow.h"
//메모장 파일용 모듈 호출
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //GUI제작
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(400,400);

    //링크 사용
    //Paint Event 참고
    //각 링크 순서대로 RGB 색깔 부여
    link1 = scene->addRect(0,0,100,10,QPen(Qt::red),QBrush(Qt::red));
    link1->setTransformOriginPoint(0,5);

    link2 = new QGraphicsRectItem(0,0,100,10,link1);
    link2->setBrush(Qt::green);
    link2->setTransformOriginPoint(0,5);
    link2->setPos(100,0);

    link3 = new QGraphicsRectItem(0,0,100,10,link2);
    link3->setBrush(Qt::blue);
    link3->setTransformOriginPoint(0,5);
    link3->setPos(100,0);

    //관점 조정용
    ui->graphicsView->centerOn(link1->boundingRect().center());

    //각도 초기화
    angle1 = angle2 = angle3 = 0;

    //타이머 초기화
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::autoRotate);

    //각 링크 회전각 360도로 지정
    ui->slider1->setRange(0, 360);
    ui->slider2->setRange(0, 360);
    ui->slider3->setRange(0, 360);


    //UI 연동
    connect(ui->slider1, &QSlider::valueChanged, this, &MainWindow::on_slider1_actionTriggered);
    connect(ui->slider2, &QSlider::valueChanged, this, &MainWindow::on_slider2_actionTriggered);
    connect(ui->slider3, &QSlider::valueChanged, this, &MainWindow::on_slider3_actionTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_slider1_actionTriggered(int value) {
    angle1 = value;
    link1->setRotation(angle1);
}
void MainWindow::on_slider2_actionTriggered(int value) {
    angle2 = value;
    link2->setRotation(angle2);
}
void MainWindow::on_slider3_actionTriggered(int value) {
    angle3 = value;
    link3->setRotation(angle3);
}

void MainWindow::on_save_clicked() {
    QFile file("angles.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out << angle1;
        out << " ";
        out << angle2;
        out << " ";
        out << angle3;
        out << "\n";
    }
}

void MainWindow::on_load_clicked() {
    QFile file("angles.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        in >> angle1 >> angle2 >> angle3;
        ui->slider1->setValue(angle1);
        ui->slider2->setValue(angle2);
        ui->slider3->setValue(angle3);
    }
}

void MainWindow::on_rotate_clicked() {
    if(timer->isActive())
        timer->stop();
    else
        timer->start(20);
}

void MainWindow::autoRotate() {
    angle1 = (angle1 + 1) % 360;
    angle2 = (angle2 + 2) % 360;
    angle3 = (angle3 + 3) % 360;

    ui->slider1->setValue(angle1);
    ui->slider2->setValue(angle2);
    ui->slider3->setValue(angle3);

    link1->setRotation(angle1);
    link2->setRotation(angle2);
    link3->setRotation(angle3);

    //관점 이동 방지용
    ui->graphicsView->centerOn(link1->boundingRect().center());
}
