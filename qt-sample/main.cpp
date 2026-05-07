//
// Created by Administrator on 2026/5/7.
//
#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMainWindow w;
    w.setWindowTitle("MSYS2 Qt6 最小示例");
    w.resize(400, 300);

    QLabel *label = new QLabel("Hello Qt6 + MSYS2", &w);
    label->setAlignment(Qt::AlignCenter);
    w.setCentralWidget(label);

    w.show();
    return a.exec();
}
