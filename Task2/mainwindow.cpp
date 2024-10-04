#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the send button to the sendMessage function
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    // Load configuration file
    if (!loadConfig()) {
        ui->outputText->append("Failed to open config file.");
        return;
    }

    // Try to open the serial port
    if (serial.open(QIODevice::ReadWrite)) {
        ui->outputText->append("Connected to Arduino.");
        connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readData);  // Використовуємо &
    } else {
        ui->outputText->append("Failed to open port.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage()
{
    QString message = ui->messageInput->text();
    if (!message.isEmpty() && serial.isOpen()) {  // Використовуємо крапку
        serial.write(message.toUtf8() + '\n');
        ui->outputText->append("Message sent: " + message);
    }
}

void MainWindow::readData()
{
    QByteArray data = serial.readAll();  // Використовуємо крапку
    ui->outputText->append("Received from Arduino: " + QString(data).trimmed());
}

bool MainWindow::loadConfig()
{
    QFile file("/Users/olegvozniuk/Desktop/sketch_sep26a/untitled5/config.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;  // Failed to open the file
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (xml.name().toString() == QString("PortName")) {  // Перетворюємо в QString для порівняння
                serial.setPortName(xml.readElementText());
            } else if (xml.name().toString() == QString("BaudRate")) {
                serial.setBaudRate(xml.readElementText().toInt());
            }
        }
    }

    file.close();
    return !xml.hasError();
}
