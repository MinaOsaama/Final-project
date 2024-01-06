// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QJsonObject>
#include <QMutex>

class Database
{
public:
    Database();

    // Load the JSON database from file
    bool loadDatabase(const QString &jsonFilePath);

    // Add a new user to the database
    QString createUser(const QString &username, const QString &password, double balance, const QString &fullname, const int age);

    // Authenticate a user based on username and password
    bool authenticateUser(const QString &username, const QString &password);


    bool authenticateAdmin(const QString &adminname, const QString &password);

    // Get the account number of a user
    QString getAccountNumber(const QString &username) const;

    // Get the balance of a user
    double getAccountBalance(const QString &accountNumber) const;

    // View transaction history for a user
    QJsonArray getTransactionHistory(const QString &accountNumber, int count) const;

    // View the entire bank database
    //QJsonObject getBankDatabase() const;

    QJsonArray getBankDatabase() const;
    // Delete a user
    QString deleteUser(const QString &accountNumber);

    // Update user data
    QString updateUser(const QString &accountNumber, const QJsonObject &newData);

    QString transferAmount(const QString &fromAccountNumber, const QString &toAccountNumber, double amount);

    QString makeTransaction(const QString &username, double amount, const QString &type);

    bool findAccNo(const QString &AccountNumber);

private:
    QJsonObject bankData;
    mutable QMutex mutex;

    // Helper method
    QJsonObject findUser(const QString &username) const;

    QJsonObject findAdmin(const QString &adminname) const;

    // Save Bank data to json file
    bool saveDatabaseToFile() const;
};

#endif // DATABASE_H
