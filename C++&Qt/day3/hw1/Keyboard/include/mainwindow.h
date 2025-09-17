#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_B1_clicked();
    void on_B2_clicked();
    void on_B3_clicked();
    void on_B4_clicked();
    void on_B5_clicked();
    void on_B6_clicked();
    void on_B7_clicked();
    void on_B8_clicked();
    void on_B9_clicked();
    void on_B10_clicked();

    void on_Shift_clicked();
    void on_Space_clicked();
    void on_Comma_clicked();
    void on_Enter_clicked();
    void on_Backspace_clicked();

    //천지인 키보드 문자 확정용
    void commit();

private:
    Ui::MainWindow *ui;

    QVector<QString> button_assign;
    QTimer *commit_Time;
    //1초 기준

    //문자열 제작
    int button_final;
    //버튼 그룹 내 인덱스
    int button_index;
    bool shift;

    QString commit_Text;


    void generate_char(int buttonIndex);
    void display_string();
    void shifting();
    void particular_symbols(const QChar &c); //Space/Comma/Enter
};

#endif
