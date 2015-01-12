#include <QtCore/QMimeData>
#include <QtGui/QDropEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
    // Source: https://stackoverflow.com/questions/14895302
    for (auto url: event->mimeData()->urls()) {
        QString filePath = url.toLocalFile();
        QString shownText = QString("%1 -> (%2)")
                .arg(filePath.split("/").last())
                .arg(filePath);
        ui->listWidget->addItem(shownText);
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->listWidget->clear();
}
