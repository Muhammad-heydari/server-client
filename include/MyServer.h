#ifndef UNTITLED59_INCLUDE_MYSERVER_H
#define UNTITLED59_INCLUDE_MYSERVER_H

#include "include/MyClient.h"
#include <QObject>
#include <QTcpServer>
#include <QByteArray>
#include <QDir>
#include <include/MyRunnable.h>
#include <QThreadPool>
#include <iostream>
#include <QTimer>
namespace Server
{
class MyServer: public QTcpServer
{
  Q_OBJECT

public:
  ///
  /// \brief MyServer default constructor
  /// \param parent
  ///
  MyServer(QObject *parent = 0);

  void  doTask(int, int);

  ///
  /// \brief startServer
  ///
  void  startServer(int, QString);

  ///
  /// \brief startClient start as client
  ///
  void  startClient(QString, int);
  void  startTask(QString);

  ///
  /// \brief setSocket for get ID
  /// \param Descriptor ID frim incoming connection
  ///
  void  setSocket(int Descriptor);

  ///
  /// \brief disconnected connection
  ///
  void  disconnected();

  ///
  /// \brief connected connection
  ///
  void  connected();

protected:
  ///
  /// \brief incomingConnection virtual
  /// \param handle
  ///
  void  incomingConnection(qintptr handle) override;

  void  waitforReadReady();

private:
  int                      _socketDiscriptor;
  QTcpSocket              *_socketserver;
  int                      _countClient;
  QVector<int>             _descriptor;
  int                      _countLine;
  QString                  _file;
  QVector<QString>         _myvec;
  QVector<int>             _myClientResult;
  std::string              _fullpth;
  QTimer                  *_timer;
  int                      _port;
  QString                  _myFile;
  bool                     _connectionFlag       = true;
  bool                     _FlagForAppendChecker = true;
  QVector<int>             _checker;
  QMap<int, MyRunnable *>  allRunnable;
  QThread                 *thread;
  QVector<int>             countDescriptor;
  bool                     _badHappend = false;

public slots:
///
/// \brief readReady connection
///
  void  readReady();

  void  DCFromThread(int Descriptor);

  void  getFromThread(QString handle, int Descriptor);

  void  endTimer();
};
}
#endif // UNTITLED59_INCLUDE_MYCLIENT_H
