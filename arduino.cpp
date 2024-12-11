#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QSqlQuery>
#include "arduino.h"

Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

int Arduino::connect_arduino() {
    // Ensure serial is initialized
    if (!serial) {
        serial = new QSerialPort();
    }

    qDebug() << "Checking available ports...";
    arduino_port_name.clear();
    arduino_is_available = false;

    // Scan available ports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Detected port:" << info.portName() << ", Description:" << info.description();
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == arduino_uno_vendor_id &&
                info.productIdentifier() == arduino_uno_product_id) {
                arduino_port_name = info.portName();
                arduino_is_available = true;
                break;
            }
        }
    }

    // Use the specified port if found
    if (!arduino_is_available) {
        qDebug() << "Arduino not found.";
        return 2; // Arduino is not available
    }

    qDebug() << "Attempting to open port: COM3"; // Explicitly set to COM3
    serial->setPortName("COM3"); // Set the port name to COM3

    if (serial->open(QIODevice::ReadWrite)) {
        // Configure serial port settings
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        qDebug() << "Arduino connected successfully!";
        return 0; // Successfully connected
    } else {
        qDebug() << "Failed to open the port: COM3. Error:" << serial->errorString();
        return 1; // Arduino available but not connected
    }
}

int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Arduino port closed successfully.";
        return 0; // Successfully closed
    }
    qDebug() << "Failed to close Arduino port. Port was not open.";
    return 1; // Port wasn't open
}

QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll(); // Retrieve data
        qDebug() << "Data received from Arduino:" << data;
        return data;
    } else {
        qDebug() << "Serial port not readable! Error:" << serial->errorString();
    }
    return QByteArray();
}

int Arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        serial->write(d); // Send data to Arduino
        qDebug() << "Data sent to Arduino:" << d;
        return 0; // Successfully sent
    } else {
        qDebug() << "Couldn't write to serial! Error:" << serial->errorString();
        return 1; // Error writing
    }
}

QSerialPort* Arduino::getserial()
{
    return serial;
}

QString Arduino::getarduino_port_name()
{
    return this->arduino_port_name;
}
