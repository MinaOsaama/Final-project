#include <QCoreApplication>
#include "server.h"
//#include "database.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //Database myDatabase;
    Server myServer;
    if(myServer.startServer())
    {
    }

    return a.exec();
}

