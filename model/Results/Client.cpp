#include "Client.h"
#include <QFile>
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    bytesSent = 0;

    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::bytesWritten, this, &Client::onBytesWritten);
    connect(socket, &QTcpSocket::errorOccurred, this, &Client::onErrorOccurred);
}

bool Client::sendFile(const QString &filePath, const QString &host, quint16 port) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for sending:" << file.errorString();
        return false;
    }
    fileData = file.readAll();
    file.close();

    bytesSent = 0;
    socket->connectToHost(host, port);
    return socket->waitForConnected(3000);
}

void Client::onConnected() {
    qDebug() << "Connected to server. Sending file...";
    qint64 sent = socket->write(fileData);
    if (sent > 0) bytesSent += sent;
}

void Client::onBytesWritten(qint64 bytes) {
    bytesSent += bytes;
    if (bytesSent >= fileData.size()) {
        qDebug() << "File sent successfully.";
        socket->disconnectFromHost();
    }
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError << socket->errorString();
}
