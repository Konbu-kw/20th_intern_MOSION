#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QGraphicsScene 초기화
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //이미지 초기화
    currentImage = QImage(750, 500, QImage::Format_ARGB32);
    currentImage.fill(Qt::white);

    //초기 Undo 상태 저장
    UNDO();
    update();

    //graphicsView의 viewport에서 마우스 이벤트 감지
    ui->graphicsView->viewport()->installEventFilter(this);
}


//헤더에 선언된 마우스 처리 함수 구현
void MainWindow::mouse_condition(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        draw1 = 1;
        lastPoint = event->pos() - QPoint(15,25);
    }
    else if(event->button() == Qt::RightButton){
        draw2 = 1;
        lastPoint = event->pos() - QPoint(15,25);
    }
}

void MainWindow::mouse_move(QMouseEvent *event)
{
    if(draw1) {
        QPainter painter(&currentImage);
        painter.setPen(QPen(Qt::black, size, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine((lastPoint), event->pos() - QPoint(15,25));
        lastPoint = event->pos() - QPoint(15,25);;
        update();
    }
    else if(draw2){
        QPainter painter(&currentImage);
        painter.setPen(QPen(Qt::white, size, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint, event->pos() - QPoint(15,25));
        lastPoint = event->pos() - QPoint(15,25);
        update();
    }
}

void MainWindow::mouse_let(QMouseEvent *event)
{
    if(draw1 && event->button() == Qt::LeftButton) {
        draw1 = 0;
        UNDO();
        redo.clear();
    }
    else if(draw2 && event->button() == Qt::RightButton){
        draw2 = 0;
        UNDO();
        redo.clear();
    }
}
//undo, redo
void MainWindow::UNDO()
{
    undo.push_back(currentImage);
    if(undo.size() > 50)
        undo.erase(undo.begin());
}

void MainWindow::update()
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(currentImage));
}

void MainWindow::on_pushButton_clicked()
{
    if(undo.size() > 1) {
        redo.push_back(undo.back());
        undo.pop_back();
        currentImage = undo.back();
        update();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!redo.empty()) {
        undo.push_back(redo.back());
        currentImage = redo.back();
        redo.pop_back();
        update();
    }
}


//graphicsView viewport 사용할 때의 처리함수
bool MainWindow::mouse_event(QObject *object, QEvent *event)
{
    if(object == ui->graphicsView->viewport()) {
        if(event->type() == QEvent::MouseButtonPress) {
            mouse_condition(static_cast<QMouseEvent*>(event));
            return true;
        }
        else if(event->type() == QEvent::MouseMove) {
            mouse_move(static_cast<QMouseEvent*>(event));
            return true;
        }
        else if(event->type() == QEvent::MouseButtonRelease) {
            mouse_let(static_cast<QMouseEvent*>(event));
            return true;
        }
    }
    return false;
}

//마우스 이벤트 처리
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    return mouse_event(obj, event) || QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//브러쉬 크기
void MainWindow::on_horizontalSlider_sliderMoved(int value)
{
    size = 3;
    size = value;
}

