#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
class myThread: public QObject
{
  Q_OBJECT

public:
  myThread(QObject *parent = 0);

  int               _index;
  int               _Descriptor;
  QVector<int>      _descriptor;
  int               _indexNumber;
  QTcpSocket       *_socketServer;
  QVector<QString>  _myvec;

public slots:
  void  startmyThread();

signals:
  void  finished();
};

#endif // MYTHREAD_H
