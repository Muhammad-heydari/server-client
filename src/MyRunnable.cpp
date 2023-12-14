#include "include/MyRunnable.h"
#include <QThread>
#include <QThreadPool>

MyRunnable::MyRunnable(QObject *parent):
  QObject(parent)
{
}

void  MyRunnable::run()
{
  _socketServer = new QTcpSocket;

  _socketServer->setSocketDescriptor(Descriptor);
  connect(_socketServer, &QTcpSocket::disconnected, this, &MyRunnable::finished);
  connect(_socketServer, &QTcpSocket::readyRead, this, &MyRunnable::readReady);
  connect(this, &MyRunnable::closeConnectionFromMain, this, &MyRunnable::closeConnection);

  QByteArray  byte;

  qInfo() << QThread::currentThread();

  byte.append(result);

  qInfo() << "server sent to client (" << Descriptor << ") exist" << byte.split('\n');

  _socketServer->write(byte);
  _socketServer->flush();

  if (!_socketServer->waitForReadyRead(10000))
  {
    emit  FromThreadToServer("wrong", Descriptor);
  }
}

void  MyRunnable::readReady()
{
  QString  mynumber = _socketServer->readAll();
  int      myread   = mynumber.toInt();

  qInfo() << "server read from client: " << myread;

  emit  FromThreadToServer(mynumber, Descriptor);
}

void  MyRunnable::finished()
{
  emit  DCGoFromThread(Descriptor);
}

void  MyRunnable::closeConnection(bool badhappend)
{
  if (badhappend == true)
  {
    qInfo() << "try to close number " << Descriptor << " !!(number of client is wrong)";
  }

  _socketServer->close();
}
