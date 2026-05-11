//
// Created by arefu on 2026/5/11.
//
#include <QDebug>
#include "serialmanager.h"

SerialManager::SerialManager(QObject *parent) : QObject(parent) {
}

SerialManager::~SerialManager() {
    // 关闭所有串口
    for (auto *serial: m_serials.values()) {
        serial->close();
        serial->deleteLater();
    }
}

bool SerialManager::openSerial(const QString &portName,
                               int baud,
                               QSerialPort::FlowControl flow) {
    if (m_serials.contains(portName)) {
        qDebug() << portName << "已打开";
        return false;
    }

    auto *serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(baud);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(flow); // 流控：硬件/软件/无

    // 打开
    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "打开失败：" << serial->errorString();
        delete serial;
        return false;
    }

    // 信号绑定
    connect(serial, &QSerialPort::readyRead, this, &SerialManager::onDataReceived);
    connect(serial, &QSerialPort::errorOccurred, this, &SerialManager::onErrorOccurred);

    m_serials.insert(portName, serial);
    qDebug() << "成功打开：" << portName;
    return true;
}

void SerialManager::onDataReceived() {
    auto *serial = qobject_cast<QSerialPort *>(sender());
    if (!serial) return;

    QByteArray data = serial->readAll();
    qDebug() << "[" << serial->portName() << "] 接收：" << data.toHex(' ');
}

void SerialManager::onErrorOccurred(QSerialPort::SerialPortError error) {
    auto *serial = qobject_cast<QSerialPort *>(sender());
    if (error != QSerialPort::NoError) {
        qDebug() << "串口错误：" << serial->portName() << serial->errorString();
    }
}
