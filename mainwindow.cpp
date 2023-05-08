#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myvideocapture.h"
#include "myvideocapture.cpp"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mOpenCV_videoCapture = new MyVideoCapture(this);
    /*connect(mOpenCV_videoCapture, &MyVideoCapture::nowaklatka, this, [&]()
    {
        ui->label_2->setPixmap(mOpenCV_videoCapture->zdjecie().scaled(500, 500));
    });*/
    connect(mOpenCV_videoCapture, &MyVideoCapture::newPixmapCaptured, this, [&]()
    {
        ui->label->setPixmap(mOpenCV_videoCapture->pixmap().scaled(500, 500));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    mOpenCV_videoCapture->terminate();
}

void MainWindow::on_pushButton_clicked()
{
    mOpenCV_videoCapture->start(QThread::HighestPriority);
}

void MainWindow::on_pushButton_2_clicked()
{
    QPixmap frame4 = mOpenCV_videoCapture->klatka();
    ui->label_2->setPixmap(frame4.scaled(500, 500));
}

void MainWindow::on_pushButton_4_clicked()
{
    /*QMessageBox::StandardButton reply = QMessageBox::information(this, "Ostrzeżenie", "Czy napewno chcesz zamknąć program? Twoja praca może zostać niezapisana.", QMessageBox::Cancel, QMessageBox::Ok);
    if (reply == QMessageBox::Ok)
    {
        QApplication::quit();
    }*/
    QApplication::quit();
}


void MainWindow::on_pushButton_3_clicked()
{
    QPixmap q;
    ui->label_2->setPixmap(q);
    mOpenCV_videoCapture->terminate();
    ui->label->setPixmap(q);
}

