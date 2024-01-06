// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSend_clicked(); // Slot for handling send button click
    void handleServerResponse(const QString &response); // Slot for handling server response
    void on_pushButtonExit_clicked(); // Slot for handling exit button click
    void on_pushButtonClear_clicked(); // Slot for handling exit button click

private:
    Ui::MainWindow *ui;
    Client client;
    qint8 m_state;
    QString m_response;

};

#endif // MAINWINDOW_H
