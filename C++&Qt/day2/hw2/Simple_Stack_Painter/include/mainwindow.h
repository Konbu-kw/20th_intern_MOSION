#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//벡터 사용
#include <vector>
//마우스 사용, 이미지 사용 모듈
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QImage>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mouse_condition(QMouseEvent *event);
    void mouse_move(QMouseEvent *event);
    void mouse_let(QMouseEvent *event);
    bool mouse_event(QObject *object, QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_sliderMoved(int value);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QImage currentImage;
    std::vector<QImage> undo;
    std::vector<QImage> redo;

    QPoint lastPoint;
    bool draw1 = 0;
    bool draw2 = 0;
    void UNDO();
    void update();

    double size;
};
#endif // MAINWINDOW_H
