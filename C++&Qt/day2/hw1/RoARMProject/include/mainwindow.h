#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//UI 슬롯들
private slots:
    void on_save_clicked();
    void on_load_clicked();
    void on_rotate_clicked();

    void on_slider1_actionTriggered(int value);
    void on_slider2_actionTriggered(int value);
    void on_slider3_actionTriggered(int value);
    //타이머 제어 -> 시계 방향!
    void autoRotate();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsRectItem *link1;
    QGraphicsRectItem *link2;
    QGraphicsRectItem *link3;

    int angle1, angle2, angle3;

    QTimer *timer;
};

#endif // MAINWINDOW_H
