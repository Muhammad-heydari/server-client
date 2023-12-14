#include "include/MyServer.h"

#include <QDebug>
#include <iostream>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QDataStream>

namespace Server
{
MyServer::MyServer(QObject *parent)
{
  _countClient          = 0;
  _timer                = new QTimer(this);
  _FlagForAppendChecker = true;
}

void  MyServer::startServer(int port, QString file)
{
  _myFile = file;
  _port   = port;

  if (!this->listen(QHostAddress::Any, port))
  {
    qDebug() << "dont want listen";

    connect(_timer, &QTimer::timeout,
            this, &MyServer::endTimer);

    _timer->start(5000);
    qInfo() << "wait 5 second to try again";

    return;
  }

  disconnect(_timer, &QTimer::timeout,
             this, &MyServer::endTimer);
  _timer->stop();
  qDebug() << "listening";
  _file = file;
}

void  MyServer::setSocket(int Descriptor)
{
  _socketDiscriptor = Descriptor;
  qInfo() << "ID:" << _socketDiscriptor << "connected";

  ++_countClient;
  _socketserver = new QTcpSocket;
  connect(_socketserver, &QTcpSocket::readyRead, this, &Server::MyServer::readReady);
  connect(_socketserver, &QTcpSocket::disconnected, this, &MyServer::MyServer::disconnected);
  connect(_socketserver, &QTcpSocket::connected, this, &Server::MyServer::connected);

// _socketserver->setSocketDescriptor(Descriptor);
  _descriptor.push_back(Descriptor);

  allRunnable.clear();
  std::cout << "start Task or not?(Y/N) " << std::endl;
  std::string  myAnwser;
  std::cin >> myAnwser;

  if ((myAnwser == "Y") || (myAnwser == "y"))
  {
    std::string  pth = _file.toStdString();

    if (!_file.isEmpty())
    {
      QFile    file(QString::fromStdString(pth));
      QString  result;

      if (file.open(QIODevice::Text | QIODevice::ReadOnly))
      {
        QTextStream  textstream(&file);
        result = textstream.readAll();
        file.close();
      }

      QString      arrName;
      QStringList  mylist = result.split(QString("\n"));     /// all line

      for (QString item : mylist)
      {
        if (!item.isEmpty())
        {
          _myvec.append(item);
        }
      }

      qInfo() << "all item in file: " << _myvec;

      int          index = pth.find_last_of("/");
      std::string  resultpth;

      for (index; index < pth.size(); ++index)
      {
        resultpth += pth[index];
      }

      resultpth += "\n";
      arrName.append(QString::fromStdString(resultpth));/////name
      _connectionFlag = false;
      startTask(arrName);
    }
  }
  else if ((myAnwser == "N") || (myAnwser == "n"))
  {
    _connectionFlag = true;
  }
  else
  {
    qInfo() << "wrong anwser";
    _connectionFlag = true;

    return;
  }
}

void  MyServer::disconnected()
{
  qDebug() << "ID:" << _socketDiscriptor << "disconnected because server do something";
// --_countClient;
// _descriptor.removeOne(_socketDiscriptor);
}

void  MyServer::connected()
{
}

void  MyServer::readReady()
{
}

void  MyServer::DCFromThread(int Descriptor)
{
  qInfo() << Descriptor << "was disconnected";
  --_countClient;
  _descriptor.removeOne(Descriptor);
}

void  MyServer::startTask(QString pth)
{
  std::string  stdPth = pth.toStdString();
  std::string  resultPth(stdPth.begin(), stdPth.end() - 1);

  _fullpth = "/home/m-heydari/Downloads/T1" + resultPth;
  int  _countLine;

  _countLine   = _myvec.size();
  _countClient = _descriptor.size();
  qInfo() << _countLine << "  " << _countClient;
  int  numberInClient     = _countLine / _countClient;
  int  numberINLastClient = (_countLine % _countClient) + numberInClient;

  if (!QDir("/home/m-heydari/Downloads/T1").exists())
  {
    QDir().mkdir("/home/m-heydari/Downloads/T1");
  }

  int      count = 0;
  QString  myresultLastClient;

  for (int h = 1; h <= numberINLastClient; ++h)////every number in last client
  {
    myresultLastClient += _myvec.at(count) + "\n";
    ++count;
  }

  thread = new QThread;
  MyRunnable *task = new MyRunnable;

  task->_indexNumber = count;
  task->result       = myresultLastClient;
  task->Descriptor   = _descriptor.at(_descriptor.size() - 1);
  task->moveToThread(thread);
  QObject::connect(thread, &QThread::started, task, &MyRunnable::run);
  QObject::connect(thread, &QThread::finished, task, &MyRunnable::deleteLater);
  QObject::connect(task, &MyRunnable::FromThreadToServer, this, &MyServer::getFromThread);
  QObject::connect(task, &MyRunnable::DCGoFromThread, this, &MyServer::DCFromThread);
  thread->start();
  allRunnable.insert(_descriptor.at(_descriptor.size() - 1), task);

  for (int i = 0; i < _descriptor.size() - 1; ++i)
  {
    QString  myresultOthreClient;

    for (int h = 1; h <= numberInClient; ++h)     // every number in client
    {
      myresultOthreClient += _myvec.at(count) + "\n";
      ++count;
    }

    thread = new QThread;
    MyRunnable *task = new MyRunnable;

    task->_indexNumber = count;
    task->Descriptor   = _descriptor.at(i);
    task->result       = myresultOthreClient;
    task->moveToThread(thread);
    QObject::connect(thread, &QThread::started, task, &MyRunnable::run);
    QObject::connect(thread, &QThread::finished, task, &MyRunnable::deleteLater);
    QObject::connect(task, &MyRunnable::FromThreadToServer, this, &MyServer::getFromThread);
    QObject::connect(task, &MyRunnable::DCGoFromThread, this, &MyServer::DCFromThread);
    thread->start();
    allRunnable.insert(_descriptor.at(i), task);
  }
}

void  MyServer::getFromThread(QString handle, int Descriptor)
{
  if (handle == "wrong")
  {
    qInfo() << "bad happent(number of exist client is not ok)!!";
    _badHappend = true;
  }

  countDescriptor.append(Descriptor);
  _myClientResult.append(handle.toInt());

  if (_FlagForAppendChecker == true)
  {
    for (int item : _descriptor)
    {
      _checker.append(item);
    }

    _FlagForAppendChecker = false;
  }

  qInfo() << "i got something from " << _myClientResult.size() << "clients !! total client: " << allRunnable.size();

  if (_myClientResult.size() == allRunnable.size())
  {
    int  finalResult = 0;

    for (int item : _myClientResult)
    {
      finalResult += item;
    }

    qInfo() << "final Result" << finalResult;

    QFile  file(QString::fromStdString(_fullpth));

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      QTextStream  text(&file);
      text << finalResult;
      qInfo() << "write final result in " << QString::fromStdString(_fullpth);
      file.close();
    }

    _myvec.clear();
    _descriptor.clear();

    _FlagForAppendChecker = true;

    _countLine   = 0;
    _countClient = 0;
  }

// _socketDiscriptor = Descriptor;
// _socketserver->setSocketDescriptor(Descriptor);
// _socketserver->disconnectFromHost();
// qInfo() << allRunnable[Descriptor]->Descriptor;
// qInfo() << "idk " << Descriptor;

  if (_myClientResult.size() == allRunnable.size())
  {
    for (int item : countDescriptor)
    {
      if (_badHappend == true)
      {
        allRunnable[item]->closeConnectionFromMain(true);
      }
      else
      {
        allRunnable[item]->closeConnectionFromMain();
      }
    }

    _myClientResult.clear();
    _checker.clear();
    _connectionFlag = true;
    countDescriptor.clear();
    _badHappend = false;
  }
}

void  MyServer::endTimer()
{
  startServer(_port, _myFile);
}

void  MyServer::doTask(int Descriptor, int indexNumber)
{
}

void  MyServer::incomingConnection(qintptr handle)
{
  if (_connectionFlag == false)
  {
    _socketserver->setSocketDescriptor(handle);
    _socketserver->close();
  }
  else
  {
    setSocket(handle);
  }
}

void  MyServer::waitforReadReady()
{
  connect(_socketserver, &QTcpSocket::readyRead, this, &Server::MyServer::readReady);
}
}
