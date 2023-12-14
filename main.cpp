#include <QCoreApplication>
#include "include/MyServer.h"
#include <QCommandLineParser>
#include <QTcpSocket>

int  main(int argc, char *argv[])
{
// qInfo()<<QThread::currentThread();
  QCoreApplication    a(argc, argv);
  QCommandLineParser  parser;

  parser.setApplicationDescription("Test helper");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
  parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));
  QCommandLineOption  option2(QStringList() << "p" << "port",
                             QCoreApplication::translate("main", "Copy all source files into <directory>."),
                             QCoreApplication::translate("main", "directory"));
  QCommandLineOption  option1(QStringList() << "i" << "ip",
                             QCoreApplication::translate("main", "Copy all source files into <directory>."),
                             QCoreApplication::translate("main", "directory"));
  QCommandLineOption  option3(QStringList() << "f" << "file",
                             QCoreApplication::translate("main", "Copy all source files into <directory>."),
                             QCoreApplication::translate("main", "directory"));
  parser.addOption(option1);
  parser.addOption(option2);
  parser.addOption(option3);
  parser.process(a);
  QString           item    = parser.positionalArguments().at(0);
  Server::MyServer *mServer = new Server::MyServer;
  Client::MyClient *mClient = new Client::MyClient;

  if (item == QString("server"))
  {
    // start server
    qDebug() << "start server";
    auto     port = parser.value(option2).toInt();
    QString  file = parser.value(option3);

    mServer->startServer(port, file);
  }
  else if (item == "client")
  {
    // start client
    qDebug() << "start client";
    auto  ip   = parser.value(option1);
    auto  port = parser.value(option2).toInt();
    mClient->startClient(ip, port);
  }

  return a.exec();
}
