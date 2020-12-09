
#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include<filecrud.h>
#include <QDataStream>
#include<QTimer>
#include<QUdpSocket>
#include<QNetworkInterface>
class Client
{
  //Q_OBJECT
public:
    Client();
    ~Client();
signals:
    void newMessage(QString);
public slots:

    QStringList listRemove(QStringList list,QString data);
    QStringList fileToList(QString filename);
    void listToFile(QStringList list, QString filename);
    QString listGetLine(QStringList list,QString data);

private slots:
    void readSocket();
    void discardSocket();
    void displayMessage(const QString& str);
    void connectHost();
    void commandExecuteSlot(QString command,QString username,QString password);
    void udpSocketRead();
    void timerControlSlot();
    void hostAddressMacButtonSlot();
    //QString getIpPort///Status(QString ip_,QString prt);
private:
    QTimer *timer;
    QTimer *timerControl;
    QTcpSocket* socket;
    QString myIpAddress;
    QString myMacAddress;
    QString localDir;
    QString ip;
    QString port;
    QUdpSocket *udpSocket = nullptr;

};

#endif // CLIENT_H
