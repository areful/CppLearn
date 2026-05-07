//
// Created by Administrator on 2026/5/7.
//

#ifndef CPPLEARNING_MAINWINDOW_H
#define CPPLEARNING_MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    // 朗读按钮
    void on_speakBtn_clicked();

    // 停止按钮
    void on_stopBtn_clicked();

private:
    Ui::MainWindow *ui;
    QTextToSpeech *speech; // 语音引擎对象
};

#endif //CPPLEARNING_MAINWINDOW_H
