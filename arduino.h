#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QByteArray>

class Arduino {
public:
    // Constructor
    Arduino();

    // Methods to manage the Arduino connection
    int connect_arduino();             // Establish a connection to the Arduino
    int close_arduino();               // Close the Arduino connection
    QByteArray read_from_arduino();    // Read data from Arduino
    int write_to_arduino(QByteArray d); // Write data to Arduino

    // Getters
    QSerialPort* getserial();          // Return the QSerialPort object
    QString getarduino_port_name();    // Return the Arduino port name

private:
    QSerialPort *serial;               // Serial communication object
    QString arduino_port_name;         // Name of the Arduino port
    bool arduino_is_available;         // Arduino availability flag
    QByteArray data;                   // Data received from Arduino

    // Replace these IDs with your Arduino's Vendor ID and Product ID
    static constexpr int arduino_uno_vendor_id = 0x2341; // Example Vendor ID for Arduino Uno
    static constexpr int arduino_uno_product_id = 0x0043; // Example Product ID for Arduino Uno
};

#endif // ARDUINO_H

