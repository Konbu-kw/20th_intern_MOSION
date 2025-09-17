#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    commit_Time = new QTimer(this);
    button_final = -1;
    button_index = 0;
    shift = false;

    button_assign.resize(10);
    button_assign[0] = ".,?!";
    button_assign[1] = "abc";
    button_assign[2] = "def";
    button_assign[3] = "ghi";
    button_assign[4] = "jkl";
    button_assign[5] = "mno";
    button_assign[6] = "pqrs";
    button_assign[7] = "tuv";
    button_assign[8] = "wxyz";
    button_assign[9] = "0";

    commit_Time->setSingleShot(true);
    //1초 기준 내 문자변경 간격
    commit_Time->setInterval(1000);
    connect(commit_Time,SIGNAL(timeout()),this,SLOT(commit()));

    commit_Text = "";
    shifting();
    display_string();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate_char(int idx)
{
    if(button_final!=-1 && button_final!=idx) commit();

    if(button_final==idx){
        QString t=button_assign[idx];
        if(t.length()>0) button_index=(button_index+1)%t.length();
        else button_index=0;
    }else{
        button_final=idx;
        button_index=0;
    }

    display_string();
    commit_Time->start();
}

void MainWindow::display_string()
{
    QString s=commit_Text;
    if(button_final!=-1){
        QString t=button_assign[button_final];
        if(t.length()>0){
            QChar c = t[button_index];
            if(shift && c.isLetter()) c=c.toUpper();
            s+=c;
        }
    }

    ui->textEdit->blockSignals(true);
    ui->textEdit->setPlainText(s);
    QTextCursor cur=ui->textEdit->textCursor();
    cur.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cur);
    ui->textEdit->blockSignals(false);
}

void MainWindow::particular_symbols(const QChar &c)
{
    if(button_final!=-1) commit();
    if(!c.isNull()){
        commit_Text+=c;
        display_string();
    }
}

void MainWindow::commit()
{
    if(button_final==-1) return;

    QString t=button_assign[button_final];
    if(t.length()>0){
        QChar c=t[button_index];
        if(shift && c.isLetter()) c=c.toUpper();
        commit_Text+=c;
    }

    button_final=-1;
    button_index=0;
    if(commit_Time->isActive()) commit_Time->stop();

    display_string();
}

void MainWindow::shifting()
{
    ui->B1->setText(button_assign[0]);
    for(int i=1;i<=8;i++){
        QString t = button_assign[i];
        if(shift) t=t.toUpper();
        if(i==1) ui->B2->setText(t);
        else if(i==2) ui->B3->setText(t);
        else if(i==3) ui->B4->setText(t);
        else if(i==4) ui->B5->setText(t);
        else if(i==5) ui->B6->setText(t);
        else if(i==6) ui->B7->setText(t);
        else if(i==7) ui->B8->setText(t);
        else if(i==8) ui->B9->setText(t);
    }
    ui->B10->setText(button_assign[9]);
}

void MainWindow::on_B1_clicked(){generate_char(0);}
void MainWindow::on_B2_clicked(){generate_char(1);}
void MainWindow::on_B3_clicked(){generate_char(2);}
void MainWindow::on_B4_clicked(){generate_char(3);}
void MainWindow::on_B5_clicked(){generate_char(4);}
void MainWindow::on_B6_clicked(){generate_char(5);}
void MainWindow::on_B7_clicked(){generate_char(6);}
void MainWindow::on_B8_clicked(){generate_char(7);}
void MainWindow::on_B9_clicked(){generate_char(8);}
void MainWindow::on_B10_clicked(){generate_char(9);}

void MainWindow::on_Shift_clicked(){shift=!shift; shifting(); display_string();}
void MainWindow::on_Space_clicked(){particular_symbols(' ');}
void MainWindow::on_Comma_clicked(){particular_symbols(',');}
void MainWindow::on_Enter_clicked(){
    if(button_final!=-1) commit();
    commit_Text+='\n';
    display_string();
    ui->label->setText(commit_Text);

    //파일입출력
    QFile f("output.txt");
    if(f.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text)){
        QTextStream out(&f);
        out<<commit_Text<<"\n";
        f.close();
    }
    commit_Text="";
}
//지우기 버튼
void MainWindow::on_Backspace_clicked(){
    if(button_final!=-1){
        button_final=-1; button_index=0;
        if(commit_Time->isActive()) commit_Time->stop();
    }else{
        if(commit_Text.length()>0) commit_Text.chop(1);
    }
    display_string();
}
