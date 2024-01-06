// client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QTextBrowser>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

public slots:
    void connectToServer();
    void sendRequest(const QString &request);

private slots:
    void onReadyRead();
    void onDisconnect();
signals:
    void serverResponseReceived(const QString &response);
public:
    QString m_data;
    QTcpSocket *socket;
    QString m_userType;

};

#endif // CLIENT_H
