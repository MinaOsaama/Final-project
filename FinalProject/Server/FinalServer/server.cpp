// server.cpp
#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

bool Server::startServer()
{
    if (!listen(QHostAddress::Any, 12345))
    {
        qDebug() << "Server could not start!";
        return 1;
    }
    else
    {
        qDebug() << "Server started on port 12345.";
        return 0;
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QThread* thread = new QThread;
    ClientHandler *clientHandler = new ClientHandler(socketDescriptor,database, sharedMutex);

    clientHandler->moveToThread(thread);

    connect(thread, &QThread::started, clientHandler, &ClientHandler::start);
    connect(clientHandler, &ClientHandler::finished, thread, &QThread::quit);
    connect(clientHandler, &ClientHandler::finished, clientHandler, &ClientHandler::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}
