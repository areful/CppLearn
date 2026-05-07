//
// Created by Administrator on 2026/5/7.
//
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Qt6 语音朗读演示");

    // 初始化语音引擎
    speech = new QTextToSpeech(this);

    // 检查是否有可用语音引擎
    if (speech->availableVoices().isEmpty()) {
        QMessageBox::warning(this, "警告", "未找到可用的语音引擎！");
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

// 朗读按钮点击
void MainWindow::on_speakBtn_clicked() {
    QString text = ui->textEdit->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::information(this, "提示", "请输入要朗读的文字");
        return;
    }

    speech->say(text); // 朗读文本
}

// 停止朗读
void MainWindow::on_stopBtn_clicked() {
    speech->stop();
}
