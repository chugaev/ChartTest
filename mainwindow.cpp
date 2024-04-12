#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chartwidget.h"
#include "filereaderthread.h"

#include <QPushButton>
#include <QFileDialog>
#include <QResizeEvent>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _fileThread = new FileReaderThread();
    connect(_fileThread, &FileReaderThread::fileRead, this, &MainWindow::handleFileRead);
    connect(_fileThread, &FileReaderThread::error, this, &MainWindow::handleFileReadError);

    ChartWidget* chart = ui->chart;
    QPushButton* browseButton = ui->browseButton;

    connect(browseButton, &QPushButton::clicked, this, [chart, this]() -> void {
        QString filePath = QFileDialog::getOpenFileName(this, "Choose file", QDir::homePath());
        if (!filePath.isEmpty()) {
            qDebug() << "Chosen file " << filePath;
            if (_fileThread->isRunning()) {
                _fileThread->requestInterruption();
                _fileThread->wait();
            }
            _fileThread->setFilePath(filePath);

            chart->setLoading(true);
            this->_fileThread->start();
        }
    });
}

void MainWindow::handleFileRead() {
    ui->chart->loadData(_fileThread->getData());
    ui->chart->setLoading(false);
}

void MainWindow::handleFileReadError(const QString& message) {
    qDebug() << message;
    ui->chart->setLoading(false);
    QMessageBox::critical(nullptr, "Error", message);
}

MainWindow::~MainWindow() {
    _fileThread->requestInterruption();
    _fileThread->wait();
    delete _fileThread;
    delete ui;
}

