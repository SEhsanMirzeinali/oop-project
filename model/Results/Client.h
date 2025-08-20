#pragma once
#include <QObject>
#include <QTcpSocket>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    bool sendFile(const QString &filePath, const QString &host, quint16 port);

    private slots:
        void onConnected();
    void onBytesWritten(qint64 bytes);
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* socket;
    QByteArray fileData;
    qint64 bytesSent;
};
