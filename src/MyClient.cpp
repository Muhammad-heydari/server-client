#include "include/MyClient.h"

#include "iostream"
#include <QFile>
#include <QDataStream>
namespace Client
{
MyClient::MyClient(QObject *parent):
  QObject(parent)
{
  _timer = new QTimer;

  _clientFlag = false;
}

void  MyClient::startClient(QString ip, int port)
{
  _socket = new QTcpSocket;
  _ip     = ip;
  _port   = port;
  _socket->connectToHost(ip, port);

  if (_socket->waitForConnected(5000))
  {
    qDebug() << "connected";

    connect(_socket, &QTcpSocket::readyRead, this, &MyClient::readReady);
    connect(_socket, &QTcpSocket::disconnected, this, &MyClient::disconnected);
    _timer->stop();
    _timer      = new QTimer;
    _clientFlag = false;
  }
  else
  {
    if (_clientFlag == false)
    {
      connect(_timer, &QTimer::timeout,
              this, &MyClient::startfromDC);

      _timer->start(5000);
    }

    qDebug() << "not conencted";
    qInfo() << "wait 5 second to try again";
  }
}

void  MyClient::readReady()
{
  QString      str    = _socket->readAll();
  QStringList  list   = str.split('\n');
  int          result = 0;

  qInfo() << "client read: " << list;

  for (QString item : list)
  {
    if (!item.isEmpty())
    {
      result += item.toInt();
    }
  }

  QByteArray  byte;
  byte.append(QString::number(result));
  qInfo() << "client write: " << byte;
  _socket->write(byte);
  _socket->flush();
}

void  MyClient::endTimer()
{
  startClient(_ip, _port);
}

void  MyClient::startfromDC()
{
  startClient(_ip, _port);
}

void  MyClient::disconnected()
{
  qInfo() << "disconnected";
  qInfo() << "wait 5 second to try again!!";

  _clientFlag = true;
  _timer->start(5000);
  connect(_timer, &QTimer::timeout, this, &MyClient::startfromDC);
}
}
