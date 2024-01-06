# Client-Server Banking System

Author Mina osama

It is a simple CLI & GUI client-server banking system developed using qt framework for windows.

## Table of Contents

- [Client-Server Banking System](#client-server-banking-system)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [Installation](#installation)
    - [Dependencies](#dependencies)
    - [Configuration](#configuration)
  - [Usage](#usage)
    - [Server Application](#server-application)
    - [Client Application](#client-application)
    - [Admin Requests Examples](#admin-requests-examples)
    - [User Requests Examples](#user-requests-examples)
  - [Code Structure](#code-structure)
    - [Overview](#overview)
    - [Modules](#modules)
  - [Troubleshooting](#troubleshooting)
    - [Common Issues](#common-issues)

## Getting Started

### Installation

git clone <https://github.com/MinaOsaama/Final-project>

### Dependencies

QT creator for console application and widget application
git

### Configuration

Before running the Banking System, configure the following settings:

Server port number

## Usage

### Server Application

To use the Banking System server application:

1-Launch the server application **first**.
2-Monitor incoming client connections.
3-Handle banking operations.

### Client Application

To use the Banking System client application:

1-Launch the client application either CLi or Gui.
2-Select admin or user.
3-Login as admin or user.
4-Perform banking operations.
5-Enter **logout** to repeat process
6-Enter **exit** to terminate process

### Admin Requests Examples

1- getaccountnumber "username"
    ex: getaccountnumber Mina
    response: Account Number: ACCT-1703432804102

2- viewaccountbalance "accountNumber"
    ex: viewaccountbalance ACCT-1703432804102
    response: Account Balance: $700.00

3- viewtransactionhistory "accountNumber" "count"
    ex: viewtransactionhistory ACCT-1703432804102 3
    response: [
    {
        "amount": 200,
        "timestamp": "2023-12-28T18:11:50",
        "type": "deposit"
    },
    {
        "amount": 1000,
        "timestamp": "2023-12-28T18:17:41",
        "type": "deposit"
    },
    {
        "amount": -1000,
        "timestamp": "2023-12-28T18:17:59",
        "to_user": "use1",
        "type": "transfer"
    }
]

4- viewbankdatabase  
    ex: viewbankdatabase
    response: [
    {
        "account_number": "ACCT-1703432804202",
        "age": 20,
        "balance": 1500,
        "fullname": "user1",
        "password": "password2",
        "transaction_history": [
            {
                "amount": 1000,
                "timestamp": "2023-01-03T12:45:00",
                "type": "deposit"
            }
        ],
        "username": "user1"
    },
    {
        "account_number": "ACCT-1703432804102",
        "age": 23,
        "balance": 700,
        "fullname": "Mina Osama",
        "password": "123",
        "transaction_history": [
            {
                "amount": 200,
                "timestamp": "2023-12-28T18:11:50",
                "type": "deposit"
            },
            {
                "amount": 1000,
                "timestamp": "2023-12-28T18:17:41",
                "type": "deposit"
            },
            {
                "amount": -1000,
                "timestamp": "2023-12-28T18:17:59",
                "to_user": "use1",
                "type": "transfer"
            },
            {
                "amount": 500,
                "timestamp": "2023-12-31T15:42:46",
                "type": "deposit"
            },
            {
                "amount": -200,
                "timestamp": "2024-01-04T16:04:34",
                "to_user": "Antony",
                "type": "transfer"
            },
            {
                "amount": 200,
                "timestamp": "2024-01-05T20:31:45",
                "type": "withdrawal"
            },
            {
                "amount": 100,
                "timestamp": "2024-01-06T01:04:54",
                "type": "withdrawal"
            },
            {
                "amount": 100,
                "timestamp": "2024-01-06T01:05:14",
                "type": "deposit"
            },
            {
                "amount": -100,
                "timestamp": "2024-01-06T01:06:00",
                "to_user": "User3",
                "type": "transfer"
            }
        ],
        "username": "Mina"
    },
    {
        "account_number": "ACCT-1703961213111",
        "age": 28,
        "balance": 100200,
        "fullname": "User2",
        "password": "assword",
        "transaction_history": [
            {
                "amount": 200,
                "from_user": "mina",
                "timestamp": "2024-01-04T16:04:34",
                "type": "transfer"
            }
        ],
        "username": "User2"
    },
    {
        "account_number": "ACCT-1704377291855",
        "age": 25,
        "balance": 50100,
        "fullname": "User3",
        "password": "2345",
        "transaction_history": [
            {
                "amount": 100,
                "from_user": "Mina",
                "timestamp": "2024-01-06T01:06:00",
                "type": "transfer"
            }
        ],
        "username": "User3"
    },
    {
        "account_number": "ACCT-1704495581676",
        "age": 27,
        "balance": 70000,
        "fullname": "user 4",
        "password": "0123",
        "transaction_history": [
        ],
        "username": "user4"
    }
]

5- deleteuser "accountNumber"
    ex: deleteuser ACCT-1703432804102
    response: User deleted successfully.

6- createnewuser "username" "password" "balance" "fullname" "age"
    ex: createnewuser ehab 1234 50000 Ehab 28
    response: New user created. Account Number: ACCT-1704548009338

7- updateuser accountNumber newdata
    ex: updateuser ACCT-1703961213111 {"password": "newPassword", "balance": 30000, "age" :24, "fullname": "Mina Osama"}
    response: account updated successfully.

8- logout
    ex: logout

9- exit
    ex: exit

### User Requests Examples

1- getaccountnumber
    ex: getaccountnumber
    response: Account Number: ACCT-1703432804102

2- viewaccountbalance
    ex: viewaccountbalance
    response: Account Balance: $700.00

3- viewtransactionhistory "count"
    ex: viewtransactionhistory  3
     response: [
    {
        "amount": 200,
        "timestamp": "2023-12-28T18:11:50",
        "type": "deposit"
    },
    {
        "amount": 1000,
        "timestamp": "2023-12-28T18:17:41",
        "type": "deposit"
    },
    {
        "amount": -1000,
        "timestamp": "2023-12-28T18:17:59",
        "to_user": "use1",
        "type": "transfer"
    }
]

4- maketransaction "amount" "type"
    ex: maketransaction 1000 deposit

5- transferamount "to_username" "amount"
    ex: transferamount User2 1000
    reponse:Transaction successful.

8- logout
    ex: logout

9- exit
    ex: exit

## Code Structure

### Overview

The Banking System has the following file and folder structure
-FinalProject
    -Server
        -FinalServer
            -clienthandler.cpp
            -clienthandler.h
            -database.cpp
            -database.h
            -FinalServer.pro
            -FinalServer.pro.user
            -main.cpp
            -server.cpp
            -server.h

    -Client
        -FinalClient
            -client.cpp
            -client.h
            -FinalClient.pro
            -FinalClient.pro.user
            -main.cpp

    -Client_GUI
        -Client
            -client.cpp
            -client.h
            -FinalClient.pro
            -FinalClient.pro.user
            -main.cpp
            -mainwindow.cpp
            -mainwindow.h
            -mainwindow.ui

    -DB.json

### Modules

The major modules of the Banking System include:

-Client module: This module gets client input and send to server and shows server response.

-Server module: This module starts server and waits for new incomming connection to create new thread and new instance of CLientHandler.

-ClientHandler module: This module creates socket ,read client requests and send server responser.

-Database module: This module handles users and admin requests and save new data to DB.json file.

-MainWindow module: This module responsiple for the GUI client.

## Troubleshooting

### Common Issues

If you encounter issues, try the following solutions:

-Check the database connection.
-Check availability of server.
-check the host ip.
-Check that port in server and client is same.
-Client doesn't write neither admin nor user, he has 3 chances other way system terminates.
-Client login can't be authenticated either invalid login format or error password or user not found,he has 3 chances other way system terminates.
-Client sends invalid request or request out of his authorizations.
-Client sends invalid request format.
