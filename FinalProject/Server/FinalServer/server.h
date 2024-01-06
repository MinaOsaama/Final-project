#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QMutex>
#include <clienthandler.h>
#include <database.h>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    bool startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QMutex sharedMutex;
    Database database;
};

#endif // SERVER_H
