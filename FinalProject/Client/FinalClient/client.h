// client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer();
public slots:
    void sendRequest(const QString &request);
private slots:
    void onReadyRead();
    void onDisconnect();
private:
    void startReadingInput();
    void setUserType();
    void handleUserInput();
private:
    QTcpSocket *socket;
    QString m_userType;
    qint8 m_state;
    QString m_data;


};

#endif // CLIENT_H
