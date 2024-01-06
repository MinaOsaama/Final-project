/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pb_SendRequest;
    QLineEdit *le_UserInput;
    QTextEdit *tb_ServerResponse;
    QPushButton *pb_Exit;
    QPushButton *pb_Clear;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(995, 555);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pb_SendRequest = new QPushButton(centralwidget);
        pb_SendRequest->setObjectName("pb_SendRequest");
        pb_SendRequest->setGeometry(QRect(300, 350, 51, 29));
        le_UserInput = new QLineEdit(centralwidget);
        le_UserInput->setObjectName("le_UserInput");
        le_UserInput->setGeometry(QRect(300, 200, 201, 28));
        tb_ServerResponse = new QTextEdit(centralwidget);
        tb_ServerResponse->setObjectName("tb_ServerResponse");
        tb_ServerResponse->setGeometry(QRect(540, 204, 421, 181));
        pb_Exit = new QPushButton(centralwidget);
        pb_Exit->setObjectName("pb_Exit");
        pb_Exit->setGeometry(QRect(450, 350, 51, 29));
        pb_Clear = new QPushButton(centralwidget);
        pb_Clear->setObjectName("pb_Clear");
        pb_Clear->setGeometry(QRect(360, 350, 83, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 995, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pb_SendRequest->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        pb_Exit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        pb_Clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
