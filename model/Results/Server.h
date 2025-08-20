#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startListening(quint16 port);

    private slots:
        void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer* tcpServer;
    QTcpSocket* clientSocket;
    QByteArray receivedData;
};
