#include "Server.h"
#include <QFile>
#include <QDebug>
#include <QDateTime>

Server::Server(QObject *parent) : QObject(parent) {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::startListening(quint16 port) {
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server failed to start:" << tcpServer->errorString();
        return false;
    }
    qDebug() << "Server listening on port" << port;
    return true;
}

void Server::onNewConnection() {
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    qDebug() << "Client connected.";
}

void Server::onReadyRead() {
    receivedData.append(clientSocket->readAll());
}

void Server::onDisconnected() {
    qDebug() << "Client disconnected. Saving file...";

    QString fileName = QString("received_%1.json")
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(receivedData);
        file.close();
        qDebug() << "File saved as" << fileName;
    } else {
        qDebug() << "Error saving file:" << file.errorString();
    }

    receivedData.clear();
    clientSocket->deleteLater();
}
