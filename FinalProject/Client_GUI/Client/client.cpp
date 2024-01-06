#include "client.h"
#include <QDebug>
#include <QCoreApplication>

Client::Client(QObject *parent) : QObject(parent), socket(nullptr)
{
    qDebug()<<"in constructor";
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnect);
}

void Client::connectToServer()
{
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected(5000))
    {
        emit serverResponseReceived("Server response>> Connected to server");
        emit serverResponseReceived("Server response>> Please, select admin or user");
    }
    else
    {
        emit serverResponseReceived("Server response>> Failed to connect to the server.");
        emit socket->disconnected();
    }
}

void Client::onReadyRead()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
    QString data = socket->readAll();
    m_data = data;
    emit serverResponseReceived(data);  // Emit signal when response is received
    }
    else
    {
        emit socket->disconnected();
    }
}
void Client::sendRequest(const QString &request)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
    QByteArray requestData = request.toUtf8();
    socket->write(requestData);
    socket->flush();
    }
    else
    {
        emit socket->disconnected();
    }
}
void Client::onDisconnect()
{
    emit serverResponseReceived("Server response>> Disconnected from the server.");
    socket->disconnect();
    socket->deleteLater();
    QCoreApplication::quit();
}
