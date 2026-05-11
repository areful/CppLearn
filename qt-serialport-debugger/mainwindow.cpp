//
// Created by arefu on 2026/5/11.
//
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Qt6 完整版串口调试工具 - 数据位/停止位/校验位/流控");
    resize(850, 550);

    m_serial = new QSerialPort(this);

    // ===================== 顶部参数区 =====================
    QHBoxLayout *topLayout = new QHBoxLayout;

    // 串口
    cbbPort = new QComboBox;

    // 波特率
    cbbBaud = new QComboBox;
    QList<qint32> bauds = {9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
    for (auto b: bauds)
        cbbBaud->addItem(QString::number(b), b);
    cbbBaud->setCurrentText("115200");

    // 数据位 5 6 7 8
    cbbDataBits = new QComboBox;
    cbbDataBits->addItem("5", QSerialPort::Data5);
    cbbDataBits->addItem("6", QSerialPort::Data6);
    cbbDataBits->addItem("7", QSerialPort::Data7);
    cbbDataBits->addItem("8", QSerialPort::Data8);
    cbbDataBits->setCurrentIndex(3);

    // 停止位 1 / 1.5 / 2
    cbbStopBits = new QComboBox;
    cbbStopBits->addItem("1", QSerialPort::OneStop);
    cbbStopBits->addItem("1.5", QSerialPort::OneAndHalfStop);
    cbbStopBits->addItem("2", QSerialPort::TwoStop);
    cbbStopBits->setCurrentIndex(0);

    // 校验位
    cbbParity = new QComboBox;
    cbbParity->addItem("无", QSerialPort::NoParity);
    cbbParity->addItem("奇校验", QSerialPort::OddParity);
    cbbParity->addItem("偶校验", QSerialPort::EvenParity);
    cbbParity->addItem("标志", QSerialPort::MarkParity);
    cbbParity->addItem("空格", QSerialPort::SpaceParity);
    cbbParity->setCurrentIndex(0);

    // 流控
    cbbFlowCtrl = new QComboBox;
    cbbFlowCtrl->addItem("无流控", QSerialPort::NoFlowControl);
    cbbFlowCtrl->addItem("硬件 RTS/CTS", QSerialPort::HardwareControl);
    cbbFlowCtrl->addItem("软件 XON/XOFF", QSerialPort::SoftwareControl);

    // 打开/关闭按钮
    btnOpen = new QPushButton("打开串口");
    connect(btnOpen, &QPushButton::clicked, this, &MainWindow::toggleOpenSerial);

    // 把所有控件加入布局
    topLayout->addWidget(new QLabel("串口:"));
    topLayout->addWidget(cbbPort);
    topLayout->addWidget(new QLabel("波特:"));
    topLayout->addWidget(cbbBaud);
    topLayout->addWidget(new QLabel("数据位:"));
    topLayout->addWidget(cbbDataBits);
    topLayout->addWidget(new QLabel("停止位:"));
    topLayout->addWidget(cbbStopBits);
    topLayout->addWidget(new QLabel("校验:"));
    topLayout->addWidget(cbbParity);
    topLayout->addWidget(new QLabel("流控:"));
    topLayout->addWidget(cbbFlowCtrl);
    topLayout->addWidget(btnOpen);

    // ===================== 接收区 =====================
    editRecv = new QTextEdit;
    editRecv->setReadOnly(true);

    // ===================== 十六进制选项 =====================
    QHBoxLayout *hexLayout = new QHBoxLayout;
    chkHexRecv = new QCheckBox("十六进制接收");
    chkHexSend = new QCheckBox("十六进制发送");
    btnClear = new QPushButton("清空接收");
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearRecv);
    hexLayout->addWidget(chkHexRecv);
    hexLayout->addWidget(chkHexSend);
    hexLayout->addStretch();
    hexLayout->addWidget(btnClear);

    // ===================== 发送区 =====================
    QHBoxLayout *sendLayout = new QHBoxLayout;
    editSend = new QLineEdit;
    btnSend = new QPushButton("发送");
    sendLayout->addWidget(editSend);
    sendLayout->addWidget(btnSend);
    connect(btnSend, &QPushButton::clicked, this, &MainWindow::sendData);

    // ===================== 主布局 =====================
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(editRecv);
    mainLayout->addLayout(hexLayout);
    mainLayout->addLayout(sendLayout);

    QWidget *centralWid = new QWidget;
    centralWid->setLayout(mainLayout);
    setCentralWidget(centralWid);

    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
    scanSerialPorts();
}

// 扫描串口
void MainWindow::scanSerialPorts() {
    cbbPort->clear();
    auto ports = QSerialPortInfo::availablePorts();
    for (auto &info: ports) {
        cbbPort->addItem(info.portName());
    }
}

// 打开/关闭串口（已包含所有参数）
void MainWindow::toggleOpenSerial() {
    if (m_serial->isOpen()) {
        m_serial->close();
        btnOpen->setText("打开串口");
        qDebug() << "串口已关闭";
        return;
    }

    m_serial->setPortName(cbbPort->currentText());
    m_serial->setBaudRate(cbbBaud->currentData().toInt());
    m_serial->setDataBits((QSerialPort::DataBits) cbbDataBits->currentData().toInt());
    m_serial->setStopBits((QSerialPort::StopBits) cbbStopBits->currentData().toInt());
    m_serial->setParity((QSerialPort::Parity) cbbParity->currentData().toInt());
    m_serial->setFlowControl((QSerialPort::FlowControl) cbbFlowCtrl->currentData().toInt());

    if (m_serial->open(QIODevice::ReadWrite)) {
        btnOpen->setText("关闭串口");
        qDebug() << "✅ 串口打开成功";
    } else {
        qDebug() << "❌ 打开失败：" << m_serial->errorString();
    }
}

// 接收数据
void MainWindow::onReadyRead() {
    QByteArray data = m_serial->readAll();
    if (chkHexRecv->isChecked()) {
        editRecv->append(data.toHex(' '));
    } else {
        editRecv->append(QString::fromLocal8Bit(data));
    }
}

// 发送数据
void MainWindow::sendData() {
    if (!m_serial->isOpen()) return;
    QString txt = editSend->text();
    if (txt.isEmpty()) return;

    QByteArray data;
    if (chkHexSend->isChecked()) {
        data = QByteArray::fromHex(txt.toLatin1());
    } else {
        data = txt.toLocal8Bit();
    }
    m_serial->write(data);
}

// 清空接收
void MainWindow::clearRecv() {
    editRecv->clear();
}
