// database.cpp
#include "database.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QDebug>

Database::Database()
{
}

bool Database::loadDatabase(const QString &jsonFilePath)
{
    QMutexLocker locker(&mutex);
    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open JSON file:" << file.errorString();
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull())
    {
        qDebug() << "Failed to parse JSON data.";
        return false;
    }
    bankData = jsonDoc.object();
    return true;
}

QString Database::createUser(const QString &username, const QString &password, double balance, const QString &fullname, const int age)
{
    QMutexLocker locker(&mutex);

    // Check if the user already exists
    if (!findUser(username).isEmpty())
    {
        qDebug() << "User already exists.";
        return "User already exists.";
    }

    // Generate a unique account number
    QString accountNumber = QString("ACCT-%1").arg(QDateTime::currentMSecsSinceEpoch());

    // Add the new user to the "users" array
    QJsonObject newUser;
    newUser["username"] = username;
    newUser["password"] = password;
    newUser["balance"] = balance;
    newUser["fullname"] = fullname;
    newUser["age"] = age;
    newUser["account_number"] = accountNumber;
    newUser["transaction_history"] = QJsonArray();

    QJsonArray usersArray = bankData["users"].toArray();
    usersArray.append(newUser);
    bankData["users"] = usersArray;

    // Save the updated database to the JSON file
    saveDatabaseToFile();

    return accountNumber;
}

bool Database::authenticateUser(const QString &username, const QString &password)
{
    QMutexLocker locker(&mutex);

    QJsonObject user = findUser(username);
    if (user.isEmpty())
    {
        qDebug() << "User not found.";
        return false;
    }

    return user["password"].toString() == password;
}

bool Database::authenticateAdmin(const QString &adminname, const QString &password)
{
    QMutexLocker locker(&mutex);

    QJsonObject user = findAdmin(adminname);
    if (user.isEmpty())
    {
        qDebug() << "User not found.";
        return false;
    }

    return user["password"].toString() == password;
}

QString Database::getAccountNumber(const QString &username) const
{
    QMutexLocker locker(&mutex);

    QJsonObject user = findUser(username);
    if (user.isEmpty())
    {
        qDebug() << "User not found.";
        return "User not found.";
    }

    return user["account_number"].toString();
}

double Database::getAccountBalance(const QString &accountNumber) const
{
    QMutexLocker locker(&mutex);
    QJsonArray usersArray = bankData["users"].toArray();
    for (const auto &user : usersArray)
    {
        QJsonObject userData = user.toObject();
        if (userData["account_number"].toString() == accountNumber)
        {
            return userData["balance"].toDouble();
        }
    }

    qDebug() << "User with account number" << accountNumber << "not found.";
    return -1.0;
}


QJsonArray Database::getTransactionHistory(const QString &accountNumber, int count) const
{
    QMutexLocker locker(&mutex);

    QJsonArray usersArray = bankData["users"].toArray();
    for (const auto &user : usersArray)
    {
        QJsonObject userData = user.toObject();
        if (userData["account_number"].toString() == accountNumber)
        {
            QJsonArray transactionHistory = userData["transaction_history"].toArray();
            int transactionsCount = qMin(count, transactionHistory.size());
            QJsonArray resultHistory;

            for (int i = 0; i < transactionsCount; ++i)
            {
                resultHistory.append(transactionHistory[i]);
            }
            return resultHistory;
        }
    }
    qDebug() << "User with account number" << accountNumber << "not found.";
    return QJsonArray();
}
/*
 *  This method shows admins and users
QJsonObject Database::getBankDatabase() const
{
    QMutexLocker locker(&mutex);
    return bankData;
}
*/
QJsonArray Database::getBankDatabase() const
{
    QMutexLocker locker(&mutex);
    return bankData["users"].toArray();
}

QString Database::deleteUser(const QString &accountNumber)
{
    QMutexLocker locker(&mutex);

    QJsonArray usersArray = bankData["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i)
    {
        QJsonObject userData = usersArray[i].toObject();
        if (userData["account_number"].toString() == accountNumber)
        {
            usersArray.removeAt(i);
            bankData["users"] = usersArray;

            // Save the updated database to the JSON file
            saveDatabaseToFile();

            return "User deleted successfully.";
        }
    }
    qDebug() << "User not found.";
    return "User not found.";
}

QString Database::updateUser(const QString &accountNumber, const QJsonObject &newData)
{
    QMutexLocker locker(&mutex);

    QJsonArray usersArray = bankData["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i)
    {
        QJsonObject userData = usersArray[i].toObject();
        if (userData["account_number"].toString() == accountNumber)
        {
            // Update user data
            for (auto it = newData.constBegin(); it != newData.constEnd(); ++it)
            {
                userData[it.key()] = it.value();
            }
            usersArray[i] = userData;

            // Update the bankData with the modified usersArray
            bankData["users"] = usersArray;
            // Save the updated database to the JSON file
            saveDatabaseToFile();

            return "User updated successfully.";
        }
    }
    qDebug() << "User not found.";
    return "User not found.";
}

bool Database::findAccNo(const QString &AccountNumber)
{
    QJsonArray usersArray = bankData["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i)
    {
        QJsonObject userData = usersArray[i].toObject();
        if (userData["account_number"].toString() == AccountNumber)
        {
            return true;
        }
    }
    return false;
}

QJsonObject Database::findUser(const QString &username) const
{
    QJsonArray usersArray = bankData["users"].toArray();
    for (const auto &user : usersArray)
    {
        QJsonObject userData = user.toObject();
        if (userData["username"].toString() == username)
        {
            return userData;
        }
    }

    return QJsonObject(); // User not found
}

QJsonObject Database::findAdmin(const QString &adminname) const
{
    QJsonArray usersArray = bankData["admins"].toArray();
    for (const auto &user : usersArray)
    {
        QJsonObject userData = user.toObject();
        if (userData["username"].toString() == adminname)
        {
            return userData;
        }
    }

    return QJsonObject(); // User not found
}

QString Database::makeTransaction(const QString &username, double amount, const QString &type)
{
    QMutexLocker locker(&mutex);

    QJsonObject user = findUser(username);
    if (user.isEmpty())
    {
        return "User not found.";
    }

    // Update balance
    double currentBalance = user["balance"].toDouble();
    if (type == "deposit")
    {
        currentBalance += amount;
    }
    else if (type == "withdrawal")
    {
        if (amount > currentBalance)
        {
            return "Insufficient funds.";
        }
        currentBalance -= amount;
    }
    else
    {
        return "Invalid transaction type.";
    }

    // Update transaction history
    QJsonArray transactionHistory = user["transaction_history"].toArray();
    QJsonObject transaction;
    transaction["amount"] = amount;
    transaction["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    transaction["type"] = type;
    transactionHistory.append(transaction);

    // Update user data
    user["balance"] = currentBalance;
    user["transaction_history"] = transactionHistory;

    // Update user in the database
    QJsonArray usersArray = bankData["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i)
    {
        QJsonObject userData = usersArray[i].toObject();
        if (userData["username"].toString() == username)
        {
            usersArray[i] = user;
            break;
        }
    }

    // Update bankData with the modified usersArray
    bankData["users"] = usersArray;

    // Save the updated database to the JSON file
    saveDatabaseToFile();

    return "Transaction successful.";
}


QString Database::transferAmount(const QString &fromUsername, const QString &toUsername, double amount)
{
    QMutexLocker locker(&mutex);

    QJsonObject fromUser = findUser(fromUsername);
    QJsonObject toUser = findUser(toUsername);

    if (fromUser.isEmpty() || toUser.isEmpty())
    {
        return "One or both users not found.";
    }

    // Check if the fromUser has sufficient funds
    double fromBalance = fromUser["balance"].toDouble();
    if (amount > fromBalance)
    {
        return "Insufficient funds.";
    }

    // Update balances and transaction history for both users
    double toBalance = toUser["balance"].toDouble();

    // Update source user
    fromBalance -= amount;
    QJsonArray fromTransactionHistory = fromUser["transaction_history"].toArray();
    QJsonObject fromTransaction;
    fromTransaction["amount"] = -amount;  // negative amount for withdrawal
    fromTransaction["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    fromTransaction["type"] = "transfer";
    fromTransaction["to_user"] = toUsername;
    fromTransactionHistory.append(fromTransaction);
    fromUser["balance"] = fromBalance;
    fromUser["transaction_history"] = fromTransactionHistory;

    // Update destination user
    toBalance += amount;
    QJsonArray toTransactionHistory = toUser["transaction_history"].toArray();
    QJsonObject toTransaction;
    toTransaction["amount"] = amount;
    toTransaction["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    toTransaction["type"] = "transfer";
    toTransaction["from_user"] = fromUsername;
    toTransactionHistory.append(toTransaction);
    toUser["balance"] = toBalance;
    toUser["transaction_history"] = toTransactionHistory;

    // Update users in the database
    QJsonArray usersArray = bankData["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i)
    {
        QJsonObject userData = usersArray[i].toObject();
        if (userData["username"].toString() == fromUsername)
        {
            usersArray[i] = fromUser;
        }
        else if (userData["username"].toString() == toUsername)
        {
            usersArray[i] = toUser;
        }
    }

    // Update bankData with the modified usersArray
    bankData["users"] = usersArray;

    // Save the updated database to the JSON file
    saveDatabaseToFile();

    return "Transfer successful.";
}



bool Database::saveDatabaseToFile() const
{
    QFile file("../../DB.json");
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open JSON file for writing:" << file.errorString();
        return false;
    }
    QJsonDocument jsonDoc(bankData);
    file.write(jsonDoc.toJson());
    file.close();
    return true;
}


