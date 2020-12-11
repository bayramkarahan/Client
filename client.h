/*****************************************************************************
 *   Copyright (C) 2020 by Bayram KARAHAN                                    *
 *   <bayramk@gmail.com>                                                     *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/

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
