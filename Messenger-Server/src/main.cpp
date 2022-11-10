#include <QCoreApplication>

#include "server/server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    _Server::Start();

    return a.exec();
}
