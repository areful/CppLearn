//
// Created by arefu on 2026/5/11.
//

#ifndef CPPLEARNING_MAINWINDOW_H
#define CPPLEARNING_MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void scanSerialPorts();

    void toggleOpenSerial();

    void onReadyRead();

    void sendData();

    void clearRecv();

private:
    QSerialPort *m_serial;

    QComboBox *cbbPort;
    QComboBox *cbbBaud;
    QComboBox *cbbDataBits; // 数据位
    QComboBox *cbbStopBits; // 停止位
    QComboBox *cbbParity; // 校验位
    QComboBox *cbbFlowCtrl; // 流控

    QCheckBox *chkHexRecv;
    QCheckBox *chkHexSend;
    QTextEdit *editRecv;
    QLineEdit *editSend;
    QPushButton *btnOpen;
    QPushButton *btnSend;
    QPushButton *btnClear;
};

#endif //CPPLEARNING_MAINWINDOW_H
