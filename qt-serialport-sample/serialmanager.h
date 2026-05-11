//
// Created by arefu on 2026/5/11.
//
#ifndef CPPLEARNING_SERIALMANAGER_H
#define CPPLEARNING_SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QMap>

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    // 打开串口（支持多串口）
    bool openSerial(const QString &portName,
                    int baud,
                    QSerialPort::FlowControl flow);

private slots:
    void onDataReceived(); // 接收数据
    void onErrorOccurred(QSerialPort::SerialPortError error); // 错误处理

private:
    // 多串口管理：key=端口名，value=串口对象
    QMap<QString, QSerialPort*> m_serials;
};

#endif //CPPLEARNING_SERIALMANAGER_H