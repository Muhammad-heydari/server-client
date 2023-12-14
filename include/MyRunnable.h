#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H
#include <QTcpSocket>
#include <QRunnable>
#include <QThread>
class MyRunnable: public QObject
{
  Q_OBJECT

public:
  MyRunnable(QObject *parent = 0);

  void  run();


  int         _indexNumber;
  QTcpSocket *_socketServer;
  int         Descriptor;
  bool        checkIsFailed = false;
  QString     result;

  void  readReady();

  void  finished();

  void  closeConnection(bool badHappend = false);

signals:
  void  FromThreadToServer(QString handle, int Descriptor);

  void  DCGoFromThread(int Descriptor);

  void  closeConnectionFromMain(bool badHappend = false);
};

#endif // MYRUNNABLE_H
