#ifndef UNTITLED59_INCLUDE_MYCLIENT_H
#define UNTITLED59_INCLUDE_MYCLIENT_H


#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QString>
#include <QTimer>
#include <QVector>
namespace Client
{
class MyClient: public QObject
{
  Q_OBJECT

public:
  ///
  /// \brief myClient constructor
  /// \param parent
  ///
  explicit MyClient(QObject *parent = nullptr);

  ///
  /// \brief startClient start as a client
  /// \param ip address
  /// \param port
  ///
  void  startClient(QString ip, int port);

public slots:
  ///
  /// \brief readReady for connection
  ///
  void  readReady();

protected:
private:
  QByteArray  _file;
  QTcpSocket *_socket;
  QTimer     *_timer;
  QString     _ip;
  int         _port;
  bool        _clientFlag;

public slots:
  void  endTimer();

  void  startfromDC();

  void  disconnected();
};
}


#endif // UNTITLED59_INCLUDE_MYCLIENT_H
