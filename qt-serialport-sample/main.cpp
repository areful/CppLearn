//
// Created by arefu on 2026/5/11.
//
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QSerialPort>
#include <QVBoxLayout>
#include <QLabel>

int main(int argc, char *argv[]) {
    // 关键1：用 QApplication 才有界面
    QApplication a(argc, argv);

    qDebug() << "程序启动";

    // ================= 串口测试 =================
    QSerialPort serial;
    serial.setPortName("COM3"); // 改成你的串口
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setFlowControl(QSerialPort::HardwareControl);

    if (serial.open(QIODevice::ReadWrite)) {
        qDebug() << "✅ 串口打开成功：" << serial.portName();
    } else {
        qDebug() << "❌ 串口打开失败：" << serial.errorString();
    }

    // ================= 创建窗口 =================
    QDialog w;
    w.setWindowTitle("串口工具 - 已成功打开串口");
    w.resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&w);
    QLabel *label = new QLabel("✅ 程序运行中\n串口已打开\nCLion + MSYS2 + Qt6 正常运行");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    w.show(); // 显示窗口

    return a.exec();
}
