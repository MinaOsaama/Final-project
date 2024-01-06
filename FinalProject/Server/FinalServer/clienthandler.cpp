// clienthandler.cpp
#include "clienthandler.h"
#include "qjsonarray.h"
#include <QJsonDocument>
#include <QDebug>

int ClientHandler::m_clientsCount = 0;

ClientHandler::ClientHandler(qintptr socketDescriptor , Database &database,QMutex &mutex, QObject *parent)
    : QObject(parent), socketDescriptor(socketDescriptor), socket(nullptr), database(database),mutex(&mutex)
{
    if (!database.loadDatabase("../../DB.json"))
    {
        qDebug() << "Error loading the database. Exiting.";
    }
    else
    {
        qInfo() << "Data Base loaded successfully.";
        m_clientsCount++;
        qInfo() << "There are "<<m_clientsCount<<" connected clients";
    }
}

ClientHandler::~ClientHandler()
{
    qInfo() << "in Destructor";
    m_clientsCount--;
    qInfo() << "There are "<<m_clientsCount<<" connected clients";
    emit finished();
}

void ClientHandler::setupConnections()
{
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}


void ClientHandler::start()
{
    QThread *currentThread = QThread::currentThread();
    qInfo() << "ClientHandler is running in thread:" << currentThread;

    setupConnections();

    if (!socket->setSocketDescriptor(socketDescriptor))
    {
        qDebug() << "Error setting socket descriptor.";
        emit finished();
        return;
    }
    checkUserType();
}

void ClientHandler::checkUserType()
{
    int attempts = 0;

    while (attempts < 3)
    {
        if (m_restart == true)
        {
            sendResponse("Please select user or admin");
            socket->waitForReadyRead(1000000);
            QByteArray userType = socket->readAll().trimmed();

            if (userType == "admin")
            {
                m_restart = false;
                qDebug() << "User type is admin";
                getAdminRequests();
                return;
            }
            else if (userType == "user")
            {
                m_restart = false;
                getUserRequests();
                return;
            }
            else
            {
                qDebug() << "Invalid user type. Attempts remaining: " << 2 - attempts;
                ++attempts;
            }
        }
        else
        {
            socket->waitForReadyRead(1000000);
            QByteArray userType = socket->readAll().trimmed();

            if (userType == "admin")
            {
                qDebug() << "User type is admin";
                getAdminRequests();
                return;
            }
            else if (userType == "user")
            {
                getUserRequests();
                return;
            }
            else
            {
                qDebug() << "Invalid user type. Attempts remaining: " << 2 - attempts;
                ++attempts;
            }
        }
    }

    // Max attempts reached, closing connection
    sendResponse("Max attempts reached. Closing connection.");
    emit finished();
}


void ClientHandler::getAdminRequests()
{
    int loginAttempts = 0;
    sendResponse("Please log in as admin. Enter your username and password separated by a space.");

    // Allow three login attempts
    while (loginAttempts < 3)
    {
        // Prompt the client to log in as an admin

        if (!socket->waitForReadyRead(1000000))
        {
            qDebug() << "Client disconnected. Closing connection.";
            emit finished();
            return;
        }

        QByteArray loginRequest = socket->readAll().trimmed();
        QStringList loginParts = QString(loginRequest).split(" ");

        if (loginParts.size() != 2)
        {
            sendResponse("Invalid login format. Please enter your username and password separated by a space.");
            ++loginAttempts;
            continue;
        }
        QString adminname = loginParts[0];
        QString password = loginParts[1];

        // Check admin credentials
        if (database.authenticateAdmin(adminname, password))
        {
            sendResponse("Login successful. Welcome, " + adminname + "!");
            // Admin authenticated, break out of the login loop
            break;
        }
        else
        {
            sendResponse("Authentication failed, Please try again.");
            ++loginAttempts;
        }
    }

    if (loginAttempts == 3)
    {
        // Max login attempts reached, disconnect the socket
        sendResponse("Max login attempts reached. Closing connection.");
        emit finished();
        socket ->disconnect();
        return;
    }

    while (socket->state() == QAbstractSocket::ConnectedState)
    {
        if (!socket->waitForReadyRead(1000000))
        {
            if (socket->state() != QAbstractSocket::ConnectedState)
            {
                qDebug() << "Client disconnected. Closing connection.";
                emit finished();
                return;
            }
        }

        QByteArray request = socket->readAll();
        if (request == "exit")
        {
            emit finished();
            return;
        }
        else if(request == "logout")
        {
            m_restart =true;
            break;
        }
        processAdminRequest(request);
    }
    if(m_restart == false)
    {
        qDebug() << "Client disconnected. Closing connection.";
        emit finished();
    }
    else if(m_restart == true)
    {
        checkUserType();
    }
}

void ClientHandler::getUserRequests()
{
    int loginAttempts = 0;
    sendResponse("Please log in as user. Enter your username and password separated by a space.");
    // Allow three login attempts
    while (loginAttempts < 3)
    {
        // Prompt the client to log in as an admin
        if (!socket->waitForReadyRead(1000000))
        {
            qDebug() << "Client disconnected. Closing connection .";
            emit finished();
            return;
        }

        QByteArray loginRequest = socket->readAll().trimmed();
        QStringList loginParts = QString(loginRequest).split(" ");
        if (loginParts.size() != 2)
        {
            sendResponse("Invalid login format. Please enter your username and password separated by a space.");
            ++loginAttempts;
            continue;
        }

        QString username = loginParts[0];
        QString password = loginParts[1];

        if (database.authenticateUser(username, password))
        {
            m_username = username;
            sendResponse("Login successful. Welcome, " + username + "!");
            // user authenticated, break out of the login loop
            break;
        }
        else
        {
            sendResponse("Authentication failed. Please try again.");
            ++loginAttempts;
        }
    }

    if (loginAttempts == 3)
    {
        // Max login attempts reached, disconnect the socket
        sendResponse("Max login attempts reached. Closing connection.");
        emit finished();
        return;
    }

    while (socket->state() == QAbstractSocket::ConnectedState)
    {
        if (!socket->waitForReadyRead(1000000))
        {
            if (socket->state() != QAbstractSocket::ConnectedState)
            {
                qDebug() << "Client disconnected. Closing connection.";
                emit finished();
                return;
            }
        }

        QByteArray request = socket->readAll();
        if (request == "exit")
        {
            emit finished();
            return;
        }

        else if(request == "logout")
        {
            m_restart =true;
            break;
        }
        processUserRequest(request);
    }
    if(m_restart == false)
    {
        qDebug() << "Client disconnected. Closing connection.";
        emit finished();
    }
    else if(m_restart == true)
    {
        checkUserType();
    }
}

void ClientHandler::processAdminRequest(const QByteArray &request)
{
    qDebug()<<"in process admin";
    QString requestString(request);
    QStringList parts = requestString.split(" ");

    if (parts.isEmpty())
    {
        sendResponse("Invalid request");
        return;
    }

    QString adminCommand = parts[0].toLower();

    if (adminCommand == "getaccountnumber")
    {
        // Command to get the account number for a user: getaccountnumber <username>
        if (parts.size() == 2)
        {
            QString username = parts[1];
            QString accountNumber = database.getAccountNumber(username);

            if (!accountNumber.isEmpty())
            {
                sendResponse("Account Number: " + accountNumber);
            }
            else
            {
                sendResponse("User not found.");
            }
        }
        else
        {
            sendResponse("Invalid command format. Usage: getaccountnumber <username>");
        }
    }
    else if (adminCommand == "viewaccountbalance")
    {
        // Command to view account balance for a user: viewaccountbalance <accountNumber>
        if (parts.size() == 2)
        {
            QString accountNumber = parts[1];
            double balance = database.getAccountBalance(accountNumber);
            if(balance != -1.0)
            {
                sendResponse("Account Balance: $" + QString::number(balance, 'f', 2));
            }
            else
            {
                sendResponse("User not found. You can try again");
            }
        }
        else
        {
            sendResponse("Invalid command format. Usage: viewaccountbalance <accountNumber>");
        }
    }
    else if (adminCommand == "viewtransactionhistory")
    {
        // Command to view transaction history for a user: viewtransactionhistory <accountNumber> <count>
        if (parts.size() == 3)
        {
            QString accountNumber = parts[1];
            int count = parts[2].toInt();
            QJsonArray transactionHistory = database.getTransactionHistory(accountNumber, count);
            if(! transactionHistory.isEmpty())
            {
                sendResponse(QString(QJsonDocument(transactionHistory).toJson()));

            }
            else
            {
                if(database.findAccNo(accountNumber) == false )
                {
                    sendResponse("User not found, You can try again.");
                }
                else
                {
                    sendResponse("No transactions made on this account.");
                }
            }
        }
        else
        {
            sendResponse("Invalid command format. Usage: viewtransactionhistory <accountNumber> <count>");
        }
    }
    else if (adminCommand == "viewbankdatabase")
    {
        qDebug()<<"in view data base";

        QJsonArray bankDatabase = database.getBankDatabase();
        // Convert the QJsonDocument to a formatted JSON string
        QJsonDocument jsonDoc(bankDatabase);
        QString formattedResponse = jsonDoc.toJson(QJsonDocument::Indented);
        sendResponse(formattedResponse.toStdString().c_str());
    }
    else if (adminCommand == "createnewuser")
    {
        // Command to create a new user: createnewuser <username> <password> <balance> <fullname> <age>
        if (parts.size() == 6)
        {
            QString username = parts[1];
            QString password = parts[2];
            double balance = parts[3].toDouble();
            QString fullname = parts[4];
            int age = parts[5].toInt();

            if(age >= 21 && balance >=10000)
            {
                QString accountNumber = database.createUser(username, password, balance,fullname,age);
                if (!accountNumber.isEmpty())
                {
                    sendResponse("New user created. Account Number: " + accountNumber);
                }
                else
                {
                    sendResponse("Failed to create user. User already exists.");
                }
            }
            else
            {
                sendResponse("Sorry,you can't create account in our bank. You should be at least 21 years old and have at least 10000 EGP .");
            }
        }
        else
        {
            sendResponse("Invalid command format. Usage: createnewuser <username> <password> <balance> <fullname> <age>");
        }
    }
    else if (adminCommand == "deleteuser")
    {
        // Command to delete a user: deleteuser <accountNumber>
        if (parts.size() == 2)
        {
            QString accountNumber = parts[1];
            QString result = database.deleteUser(accountNumber);
            sendResponse(result);
        }
        else
        {
            sendResponse("Invalid command format. Usage: deleteuser <accountNumber>");
        }
    }
    else if (adminCommand == "updateuser")
    {
        // Command to update a user: updateuser <accountNumber> {"password": "newPassword", "balance": 1000,...}
        // Ex: updateuser ACCT-1703961213111 {"password": "newPassword", "balance": 1000, "age" :28, "fullname": "Mina Osama"}
        if (parts.size() >= 3)
        {
            QString accountNumber = parts[1];
            QString newDataString = parts.mid(2).join(" ");

            QJsonDocument newDataDoc = QJsonDocument::fromJson(newDataString.toUtf8());
            if (!newDataDoc.isNull() && newDataDoc.isObject())
            {
                QJsonObject newData = newDataDoc.object();
                QString result = database.updateUser(accountNumber, newData);
                sendResponse(result);
                return;
            }
        }
        sendResponse("Invalid command format. Usage: updateuser <accountNumber> <newdata>");
    }
    else if(adminCommand == "help" )
    {
        sendResponse("\n 1- getaccountnumber <username> \n 2- viewaccountbalance <accountNumber> \n"
                     " 3- viewtransactionhistory <accountNumber> <count> \n 4- viewbankdatabase  \n"
                     " 5- deleteuser <accountNumber> \n 6- createnewuser <username> <password> <balance> <fullname> <age> \n"
                     " 7- updateuser <accountNumber> <newdata> \n 8- logout \n 9- exit");
    }
    else
    {
        sendResponse("Unknown admin command. Try help to view admin valid requests");
    }
}


void ClientHandler::processUserRequest(const QByteArray &request)
{
    QString requestString(request);
    QStringList parts = requestString.split(" ");

    if (parts.isEmpty())
    {
        sendResponse("Invalid request");
        return;
    }

    QString userCommand = parts[0].toLower();

    if (userCommand == "getaccountnumber")
    {
        // Command to get the account number for the logged-in user: getaccountnumber
        if (parts.size() == 1)
        {
            QString accountNumber = database.getAccountNumber(m_username);
            sendResponse("Account Number: " + accountNumber);
        }
        else
        {
            sendResponse("Invalid command format. Usage: getaccountnumber");
        }
    }
    else if (userCommand == "viewaccountbalance")
    {
        // Command to view account balance for the logged-in user: viewaccountbalance
        if (parts.size() == 1)
        {
            double balance = database.getAccountBalance(database.getAccountNumber(m_username));
            sendResponse("Account Balance: $" + QString::number(balance, 'f', 2));

        }
        else
        {
            sendResponse("Invalid command format. Usage: viewaccountbalance");
        }
    }
    else if (userCommand == "viewtransactionhistory")
    {
        // Command to view transaction history for the logged-in user: viewtransactionhistory <count>
        if (parts.size() == 2)
        {
            int count = parts[1].toInt();
            QJsonArray transactionHistory = database.getTransactionHistory(database.getAccountNumber(m_username), count);
            sendResponse(QString(QJsonDocument(transactionHistory).toJson()));

            if(! transactionHistory.isEmpty())
            {
                sendResponse(QString(QJsonDocument(transactionHistory).toJson()));
            }
            else
            {
                sendResponse("No transactions made on this account.");
            }
        }
        else
        {
            sendResponse("Invalid command format. Usage: viewtransactionhistory <count>");
        }
    }
    else if (userCommand == "maketransaction")
    {
        // Command to make a transaction: maketransaction <amount> <type>
        if (parts.size() == 3)
        {
            double amount = parts[1].toDouble();
            QString type = parts[2].toLower();
            QString result = database.makeTransaction(m_username, amount, type);  // Use logged-in username
            sendResponse(result);
        }
        else
        {
            sendResponse("Invalid command format. Usage: maketransaction <amount> <type>");
        }
    }
    else if (userCommand == "transferamount")
    {
        // Command to transfer amount: transferamount <to_username> <amount>
        if (parts.size() == 3)
        {
            QString toUsername = parts[1];
            double amount = parts[2].toDouble();
            QString result = database.transferAmount(m_username, toUsername, amount);
            sendResponse(result);
        }
        else
        {
            sendResponse("Invalid command format. Usage: transferamount <to_username> <amount>");
        }
    }
    else if(userCommand == "help" )
    {
        sendResponse("\n 1- getaccountnumber  \n 2- viewaccountbalance  \n"
                     " 3- viewtransactionhistory <count> \n 4- maketransaction <amount> <type>  \n"
                     " 5- transferamount <to_username> <amount> \n 6- logout \n 7- exit");
    }
    else
    {
        sendResponse("Unknown user request. Try help to view user valid requests");
    }
}

void ClientHandler::sendResponse(const QString &response)
{
    QByteArray responseData = response.toUtf8();
    socket->write(responseData);
    socket->flush();
    qDebug() << "sendResponse completed";
}

void ClientHandler::onDisconnected()
{
    qDebug() << "Socket disconnected";
    socket->disconnectFromHost();
    socket ->deleteLater();
}
