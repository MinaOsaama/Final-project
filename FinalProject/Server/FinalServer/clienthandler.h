// clienthandler.h
#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include "database.h"
#include <QThread>
#include <QMutex>
class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(qintptr socketDescriptor, Database &database, QMutex &mutex, QObject *parent = nullptr);
    ~ClientHandler();

    void start() ;
signals:
    void finished();

private slots:
    void onDisconnected();

private:
    qintptr socketDescriptor;
    QTcpSocket *socket;
    Database &database;
    QMutex* mutex;
    QString m_username;
    bool m_restart=false;
     static int m_clientsCount;

private:
    void processAdminRequest(const QByteArray &request);
    void processUserRequest(const QByteArray &request);

    void sendResponse(const QString &response);
    void setupConnections();
    void getAdminRequests();
    void getUserRequests();
    void onReadyRead();
    void checkUserType();


};

#endif // CLIENTHANDLER_H
