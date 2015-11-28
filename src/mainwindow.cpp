#include <QtCore/QMimeData>
#include <QtCore/QRegExp>
#include <QtGui/QDropEvent>
#include <QtWidgets/QMessageBox>

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
        ui->listWidget->addItem(QString("%1 -> (%2)")
                                .arg(filePath.split("/").last())
                                .arg(filePath));
    }
}

void MainWindow::on_pushButton_rename_clicked()
{
    /*TODO
     *
     * Add Checkbox "Include Extension"
     * Add Preview function on lineEdit_output_changed
     * Check for illegal character (\, /, :, *, ?, ", <, >, |)
     * Testing
     */
    if(ui->lineEdit_input->text().isEmpty()) {
        QMessageBox noRegex(this);
        noRegex.setWindowTitle(tr("Warning!"));
        noRegex.setIcon(QMessageBox::Icon::Warning);
        noRegex.setStandardButtons(QMessageBox::StandardButton::Ok);
        noRegex.setText(tr("You didn't specify an input RegEx."));
        noRegex.exec();
        return;
    }
    QRegExp reInput(ui->lineEdit_input->text());
    // Double backslash for C++-Compiler -.-
    QRegExp reStart(".+\\(");
    QRegExp reEnd("\\)$");

    for(int row = 0; row < ui->listWidget->count(); row++) {
        QListWidgetItem *item = ui->listWidget->item(row);
        QString fullFileName = item->text();

        // Remove everything except the full path
        fullFileName.remove(reStart);
        fullFileName.remove(reEnd);

        // Create FileHandle
        QFile file(fullFileName);

        // Remove Extension?

        // Replace text
        QString fileName = file.fileName().split("/").last();
        fileName.replace(reInput, ui->lineEdit_output->text());
        fullFileName.replace(file.fileName().split("/").last(), fileName);

        // Rename File
        if(!file.rename(fullFileName)) {
            QMessageBox notRenamed(this);
            notRenamed.setWindowTitle(tr("Couldn't rename file"));
            notRenamed.setIcon(QMessageBox::Icon::Warning);
            notRenamed.setStandardButtons(QMessageBox::StandardButton::Ok);
            notRenamed.setText(tr("RegEx didn't match for file %1.").arg(file.fileName()));
            notRenamed.setText(fullFileName);
            notRenamed.exec();
            continue;
        }

        // Change list entry
        item->setText(QString("%1 -> (%2)")
                .arg(fullFileName.split("/").last())
                .arg(fullFileName));
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->listWidget->clear();
}
