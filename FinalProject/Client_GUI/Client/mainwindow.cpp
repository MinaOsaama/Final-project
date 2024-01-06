// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) , m_state(0)
{
    ui->setupUi(this);
    connect(ui->pb_SendRequest, &QPushButton::clicked, this, &MainWindow::on_pushButtonSend_clicked);
    connect(ui->pb_Exit, &QPushButton::clicked, this, &MainWindow::on_pushButtonExit_clicked);
    connect(ui->pb_Clear, &QPushButton::clicked, this, &MainWindow::on_pushButtonClear_clicked);

    connect(&client, &Client::serverResponseReceived, this, &MainWindow::handleServerResponse);
    client.connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSend_clicked()
{
    if (client.socket->state() == QAbstractSocket::ConnectedState)
    {
        QString userInput = ui->le_UserInput->text().trimmed();
        m_state++;

        if(m_response == "Server response>> Failed to connect to the server.")
        {
            QMessageBox::critical(this, "Error", "connection failed");
            QCoreApplication::quit();
        }

        if(m_state == 1 || m_state == 2 || m_state == 3)
        {
            if(userInput != "user" && userInput != "admin" )
            {
                handleServerResponse("Server response>> Invalid user type");
                handleServerResponse("Server response>> Please,select admin or user");
                if( m_state == 3)
                {
                    QMessageBox::critical(this, "Error", "Max attempts reached. Closing connection.");
                    QApplication::quit();
                }
            }
            else
            {
                client.sendRequest(userInput);
                m_state =3;
            }
        }
        else if (m_state == 4)
        {
            if (userInput == "exit")
            {
                client.sendRequest(userInput);
                QMessageBox::critical(this, "Exit", "closing");
                QCoreApplication::quit();
            }
            else
            {
                client.sendRequest(userInput);
            }
        }
        else
        {
            if (   m_response == "Authentication failed, Please try again."
                || m_response == "Please select user or admin"
                || m_response == "Please log in as admin. Enter your username and password separated by a space."
                || m_response == "Please log in as user. Enter your username and password separated by a space.")
            {
                if (userInput == "exit")
                {
                    client.sendRequest(userInput);
                    QCoreApplication::quit();
                }
                else
                {
                    client.sendRequest(userInput);
                }
            }
            else
            {
                if (userInput == "exit")
                {
                    client.sendRequest(userInput);
                    QMessageBox::critical(this, "Exit", "closing");
                    QCoreApplication::quit();
                }
                else
                {
                    client.sendRequest(userInput);
                }
            }
        }
    }
    else
    {
        QMessageBox::critical(this, "Exit", "closing");
        QCoreApplication::quit();
    }
}


void MainWindow::handleServerResponse(const QString &response)
{
    m_response = response;
    if(    response == "Max login attempts reached. Closing connection."
        || response == "Max attempts reached. Closing connection." )
    {
        ui->tb_ServerResponse->append(response);
        QMessageBox::critical(this, "Error", "Closing connection");
        QCoreApplication::quit();
    }
    else
    {
        ui->tb_ServerResponse->append(response);
    }
}

void MainWindow::on_pushButtonExit_clicked()
{
    QMessageBox::critical(this, "Exit", "closing");
    QCoreApplication::quit();
}

void MainWindow::on_pushButtonClear_clicked()
{
    QMessageBox::warning(this, "clear", "clearing window");

    ui->tb_ServerResponse->clear();
}

