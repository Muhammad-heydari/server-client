#include "include/myThread.h"

myThread::myThread(QObject *parent):
  QObject(parent)
{
}

void  myThread::startmyThread()
{
  qInfo() << _Descriptor << "-----" << QThread::currentThread();

  QByteArray  byte;

  byte.append(_myvec.at(_indexNumber) + QString("\n"));

  qInfo() << byte;
  _socketServer->write(byte);

  _socketServer->flush();
  emit  finished();
}
