#include "client.h"
#include <QDebug>
#include <QCoreApplication>

Client::Client(QObject *parent) : QObject(parent), socket(nullptr),m_state(0)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,    this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnect);
}

void Client::connectToServer()
{
    socket->connectToHost("127.0.0.1", 12345);

    if (socket->waitForConnected())
    {
        qDebug() << "Connected to server";
        startReadingInput();
    }
    else
    {
        qDebug() << "Failed to connect to the server, make sure server is active and try again.";
        emit socket -> disconnected();
        qTerminate();
    }
}

void Client::onReadyRead()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QString data = socket->readAll();
        qDebug() << "Server response>> " << data.toStdString().c_str();
        m_data = data;
        if(m_data == "Max login attempts reached. Closing connection." || m_data == "Max attempts reached. Closing connection." )
        {
            emit socket-> disconnected();
        }
        else
        {
            handleUserInput(); // Continue reading input after receiving server response
        }
    }
    else
    {
        emit socket-> disconnected();
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
        emit socket-> disconnected();
    }
}

void Client::startReadingInput()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        qint8 userTypeAttempts = 0;
        m_state++;
        if(m_state==1)
        {
            while (userTypeAttempts < 3)
            {
                qInfo() << "Server response>> Please,select admin or user";
                QTextStream qin(stdin);
                QString userInput = qin.readLine().trimmed();
                if(userInput != "user" && userInput != "admin")
                {
                    qInfo() <<"Server response>> Invalid user type";
                    ++userTypeAttempts;
                    sendRequest("Dummy");
                }
                else
                {
                    sendRequest(userInput);
                    return ;
                }
            }
        }
        else if(m_state == 2)
        {
            QTextStream qin(stdin);
            QString userInput = qin.readLine().trimmed();
            if(userInput == "exit")
            {
                sendRequest(userInput);
                emit socket->disconnected();
            }
            else
            {
                sendRequest(userInput);
            }
        }

        else
        {
            if(m_data == "Authentication failed, Please try again."
                || m_data == "Please select user or admin"
                || m_data == "Please log in as admin. Enter your username and password separated by a space."
                || m_data == "Please log in as user. Enter your username and password separated by a space.")
            {
                QTextStream qin(stdin);
                QString userInput = qin.readLine().trimmed();
                if(userInput == "exit")
                {
                    sendRequest(userInput);
                    emit socket->disconnected();
                }
                else
                {
                    sendRequest(userInput);
                }
            }

            else
            {
                qInfo() << "Server response>> Please, enter your request:";
                QTextStream qin(stdin);
                QString userInput = qin.readLine().trimmed();
                if(userInput == "exit")
                {
                    sendRequest(userInput);
                    emit socket->disconnected();
                }
                else
                {
                    sendRequest(userInput);
                }
            }
        }
    }
    else
    {
        emit socket->disconnected();
    }
}
void Client::handleUserInput()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        startReadingInput();
    }
    else
    {
        qInfo() << "Server response>> Disconnected from the server.";
        emit socket->disconnected();
    }
}


void Client::onDisconnect()
{
    qInfo() << "Server response>> Disconnected from the server.";
    socket->disconnect();
    socket->deleteLater();
    QCoreApplication::quit();
}
