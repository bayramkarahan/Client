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
#include "client.h"
#include<QDebug>
#include<QProcess>

Client::Client()
{
    localDir="/usr/share/E-Ag/";

    timerControl = new QTimer();
    QObject::connect(timerControl, &QTimer::timeout, [&](){ timerControlSlot();});

    timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [&](){ connectHost();});

    /***********************************/
    FileCrud *fcc=new FileCrud();
    fcc->dosya=localDir+"hostport";
    QStringList lst;
    if(!fcc->fileExists())
    {
    lst.append("|7879|Server");
    listToFile(lst,"hostport");
    }

    if(fcc->fileExists())
    {
        QString hostport=fcc->fileGetLine(1);

        if (hostport!="")
        {
             port=hostport.split("|")[1];
            /***************************************************/
            QString uport=port;
            std::reverse(uport.begin(), uport.end());

            udpSocket = new QUdpSocket();
            udpSocket->bind(uport.toInt(), QUdpSocket::ShareAddress);
            QObject::connect(udpSocket,&QUdpSocket::readyRead,[&](){udpSocketRead();});
            qDebug()<<port<<uport<<"udp bağlandı";

        }


    }

    /**************************************/
    connectHost();
}
Client::~Client()
{
    socket->close();
    socket->deleteLater();
    udpSocket->close();
    udpSocket->deleteLater();
    socket=nullptr;

}
void Client::timerControlSlot()
{
    QString sip;
    QString hip;
    QString lip;
    hostAddressMacButtonSlot();

    if (socket->peerAddress().toString().count()>15)
    {
     sip=socket->peerAddress().toString().split(":")[3];


    }
    else
    {
     sip=socket->peerAddress().toString();//.split(":")[3];
    }
    lip=socket->localAddress().toString();
/************************************************************/
    FileCrud *fcc=new FileCrud();
    fcc->dosya=localDir+"hostport";
    if(fcc->fileExists())
    {
        QString hostport=fcc->fileGetLine(1);

        if (hostport!="")
        {
            hip=hostport.split("|")[0];
           QString hport=hostport.split("|")[1];
           // socket->connectToHost(ip,port.toInt());

        }
    }
    if(sip!=hip)
    {
        qDebug()<<"Client Bağlantısı:Servser Adresi Normal Değil Yenilenecek..."<<sip<<hip;

        socket->close();
        socket->deleteLater();
        udpSocket->close();
        udpSocket->deleteLater();
        socket=nullptr;
       system("sleep 5");
       connectHost();
    }
   // qDebug()<<"Client Bağlantısı:"<<lip<<myIpAddress;

    if(lip!=myIpAddress)
    {
        qDebug()<<"Client Bağlantısı:Client Adresi Normal Değil Yenilenecek..."<<lip<<myIpAddress;
        socket->close();
        socket->deleteLater();
        udpSocket->close();
        udpSocket->deleteLater();
        socket=nullptr;
       system("sleep 5");
       connectHost();
    }
///else
   /// qDebug()<<"Client Bağlantı Test.."<<sip<<hip;


   // connectHost();
}
void Client::udpSocketRead()
{
    QByteArray datagram;
    QStringList mesaj;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString rmesaj=datagram.constData();
         mesaj=rmesaj.split("|");

       qDebug()<<"Gelen Udp Mesaj:"<<mesaj[0]<<mesaj[1]<<mesaj[2]<<mesaj[3];
         QStringList lst;
       lst.append(mesaj[1]+"|"+mesaj[3]);


       listToFile(lst,"hostport");
    }


}

void Client::readSocket()
{
    QByteArray block = socket->readAll();

    QDataStream in(&block, QIODevice::ReadOnly);
  //  in.setVersion(QDataStream::Qt_5_11);

    while (!in.atEnd())
    {
        QString receiveString;
        in >> receiveString;
        receiveString.prepend(QString("%1 :: |").arg(socket->socketDescriptor()));
        //emit newMessage(receiveString);
        displayMessage(receiveString);

    }
}

void Client::discardSocket()
{

    socket->close();
    socket->deleteLater();

   // this->ui->statusBar->showMessage("Disconnected!");
    qDebug()<<"Bağlantı Kesildi..";
  //  system("rm /usr/share/E-Ag/E-AgClientOk");

   timerControl->stop();
timer->start(5000);
   //system("sleep 5");
  // connectHost();

}

void Client::displayMessage(const QString& str)
{
 /*   if(socket->isOpen())
    {
   QByteArray block;
   QDataStream out(&block, QIODevice::WriteOnly);
   out << "bayrammm";
   socket->write(block);
}*/
    //qDebug()<<"Gelen Mesaj:"<<str;

     QStringList lst=str.split("|");
     if(lst[1]!="")
     {
     system("nohup sh /usr/bin/E-AgRunPrg.sh&");
     //    commandExecuteSlot("sh /usr/bin/E-AgRunPrg.sh",lst[3],lst[4]);
       //  system("xhost +si:localuser:root");
         qDebug()<<"Users güvenlik işlemi yapıldı..";
     }
    if(lst[1]=="ekranmesaj")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
        QStringList liste;
        liste.append(str);
        listToFile(liste,"tcpMessage");
       // qDebug()<<"ekranmesaj:"<<lst[2];
       // system("kill $(pidof 'GClient')");
       // system("sleep 2");
        system("nohup GClient&");
        //system("xmessage 'xcalc'");
    }
    if(lst[1]=="ekrankilit")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
        QStringList liste;
        liste.append(str);
        listToFile(liste,"tcpMessage");
        system("nohup GClient&");

    }
    if(lst[1]=="ekranimage")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
        QStringList liste;
        liste.append(str);
        listToFile(liste,"tcpMessage");
        system("nohup GClient&");
        //kaydetTimer->start(1000);
        //ekranImageSayac=0;
   // kaydetTimerSlot(lst[3],lst[4],lst[5]);
    }
    if(lst[1]=="ekrankilitac")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
         QStringList liste;
        liste.append(str);
        listToFile(liste,"tcpMessage");
       // system("nohup GClient&");

    }
    if(lst[1]=="ekranpanoac")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
         QStringList liste;
        liste.append(str);
        listToFile(liste,"tcpMessage");
        system("nohup GClient&");

    } if(lst[1]=="ekranpanokapat")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
         QStringList liste;
        liste.append(str);
        listToFile(liste,"tcpMessage");
       // system("nohup GClient&");

    }
    if(lst[1]=="komut")
    {
        qDebug()<<"Client-Gelen Komut Mesaj:"<<str;
        QString kmt="nohup "+lst[2]+" &";
       // qDebug()<<"komut:"<<lst[2];
       // std::string kmt=lst[2].toStdString();

        system(kmt.toStdString().c_str());
        // this->ui->textBrowser_receivedMessages->append("Gelen Komut: "+lst[2]+"Kullanıcı: "+lst[3]+"Şifre: "+lst[4]);
       // commandExecuteSlot(lst[2],lst[3],lst[4]);
    }
    if(lst[1]=="pckapat")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
       // this->ui->textBrowser_receivedMessages->append("Gelen Komut: "+lst[2]+"Kullanıcı: "+lst[3]+"Şifre: "+lst[4]);
        commandExecuteSlot(lst[2],lst[3],lst[4]);
    }
    if(lst[1]=="pcbaslat")
    {
        qDebug()<<"Client-Gelen Mesaj:"<<str;
       // this->ui->textBrowser_receivedMessages->append("Gelen Komut: "+lst[2]+"Kullanıcı: "+lst[3]+"Şifre: "+lst[4]);
        commandExecuteSlot(lst[2],lst[3],lst[4]);
    }

}

void Client::connectHost(){
    socket = new QTcpSocket();

     QObject::connect(socket,&QTcpSocket::readyRead,[&](){
        readSocket();
    });

    QObject::connect(socket,&QTcpSocket::disconnected,[&](){
        discardSocket();
    });


    /**********************host ve port okunuyor*****************************/
    FileCrud *fcc=new FileCrud();
    fcc->dosya=localDir+"hostport";
    if(fcc->fileExists())
    {
        QString hostport=fcc->fileGetLine(1);

        if (hostport!="")
        {
            ip=hostport.split("|")[0];
            port=hostport.split("|")[1];
            socket->connectToHost(ip,port.toInt());

        }
    }
    else {
        qDebug()<<"hostport Dosyası Bulunamadı..";
        QStringList lst;

        lst.append("|7879|Server");
        listToFile(lst,"hostport");
    }
    /******************************************************/

    qDebug()<<"Server Bağlantısı Yapılmaya Çalışılıyor..."<<ip<<port;

    if(socket->waitForConnected())
    {
       // this->ui->statusBar->showMessage("Server Bağlatısı Yapıldı.");
        timer->stop();
        qDebug()<<"Server Bağlatısı Yapıldı.";

        timerControl->start(5000);
      //  system("touch /usr/share/E-Ag/E-AgClientOk");
        //  QStringList lst;lst<<"abc";listToFile(lst,"E-AgClientOk");
      }
      if(!socket->waitForConnected())
      {
         // this->ui->statusBar->showMessage("Server Bağlatısı Yapılamadı!");
          qDebug()<<"Server Bağlatısı Yapılamadı.";
        //  system("rm /usr/share/E-Ag/E-AgClientOk");
          //disconnect(socket, 0, 0, 0);
          ///disconnect(this,SIGNAL(newMessage(QString)),this,SLOT(displayMessage(QString)));
          ///disconnect(socket,SIGNAL(readyRead()),this,SLOT(readSocket()));
          ///disconnect(socket,SIGNAL(disconnected()),this,SLOT(discardSocket()));

           timer->start(5000);
        //  QMessageBox::critical(this,"QTCPClient", QString("Bağlantı Kurulurken Hata Oluştu: %1.").arg(socket->errorString()));
         // exit(EXIT_FAILURE);
      }


  }

  QStringList Client::listRemove(QStringList list,QString data)
   {
      // qDebug()<<"deneme-ddd"<<data;
      // QStringList list;
       //  qDebug()<<list;
       //list << "bayram|sun" << "cloud" << "sun|elif" << "firi|rain";
       QRegularExpression re(data);
       for(int i=0;i<list.count();i++)if(list[i].contains(re)) list.removeAt(i);
      // qDebug()<<list;
       return list;
   }
  QString Client::listGetLine(QStringList list,QString data)
 {
     //QStringList list;
     //list << "bayram|sun" << "cloud" << "sun|elif" << "firi|rain";
     QRegularExpression re(data);
     for(int i=0;i<list.count();i++) if(list[i].contains(re)) return list[i];
     //qDebug()<<list;
     return "";
 }
QStringList Client::fileToList(QString filename)
 {
    FileCrud *fcc=new FileCrud();
    fcc->dosya=localDir+filename;
    QStringList list;
    for(int i=1;i<=fcc->fileCount();i++)
    {
         QString line=fcc->fileGetLine(i);
         if(line!="")
         {
             line.chop(1);
             QStringList lst=line.split("|");
             QString ln="";
             if(lst.count()>0)ln.append(lst[0]);
             if(lst.count()>1)ln.append("|").append(lst[1]);
             if(lst.count()>2)ln.append("|").append(lst[2]);
             if(lst.count()>3)ln.append("|").append(lst[3]);
             if(lst.count()>4)ln.append("|").append(lst[4]);
             if(lst.count()>5)ln.append("|").append(lst[5]);

             list <<ln;
            // qDebug()<<ln;
             // list <<lst[0]+"|"+lst[1]+"|"+lst[2]+"|"+lst[3]+"|"+lst[4]+"|"+lst[5];

         }
    }
       return list;
 }
void Client::listToFile(QStringList list, QString filename)
 {
    FileCrud *fcc=new FileCrud();
    fcc->dosya=localDir+filename;
    fcc->fileRemove();
    for(int i=0;i<list.count();i++)
    {
         QString line=list[i];
         if(line!="")
         {
             //line.chop(1);
             QStringList lst=line.split("|");
             //qDebug()<<line;
             QString ln="";
             if(lst.count()>0)ln.append(lst[0]);
             if(lst.count()>1)ln.append("|").append(lst[1]);
             if(lst.count()>2)ln.append("|").append(lst[2]);
             if(lst.count()>3)ln.append("|").append(lst[3]);
             if(lst.count()>4)ln.append("|").append(lst[4]);
             if(lst.count()>5)ln.append("|").append(lst[5]);

             //qDebug()<<ln;
             fcc->fileWrite(ln);

         }

    }
 }

void Client::hostAddressMacButtonSlot()
{
    int a=0;
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
        {
            // Return only the first non-loopback MAC Address
            if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            {++a;
                if(a==1)
                {
                    myMacAddress=netInterface.hardwareAddress();


                }

            }

        }

    QHostAddress localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
               {
                //  qDebug() <<"deenem"<< address.toString();

            myIpAddress=address.toString();


            return;
            }
        }


}

void Client::commandExecuteSlot(QString command,QString username,QString password)
{
    //sshpass -p $parola ssh -o StrictHostKeyChecking=no -n $username@$name "echo $parola | sudo -S" $komut
///QString komut="echo '"+password+"'|sudo -S -u "+username+" -i "+command;
//echo '12' |sudo -S -u root -i apt-get update'
QString komut=command;


//mesajSlot(komut);
qDebug()<<komut;
QStringList arguments;
//QString  komut;
//komut.append("vncviewer ").append(pcIp->text()).append(" \-passwd \/usr\/bin\/x11vncpasswd");
arguments << "-c" << komut;
QProcess process;
process.start("/bin/bash",arguments);
process.waitForFinished(-1); // will wait forever until finished
QString stdout = process.readAllStandardOutput();
QString stderr = process.readAllStandardError();
stdout.chop(1);
stderr.chop(1);
 //qDebug()<<"out:"<<stdout<<stdout.count();
  //qDebug()<<"err:"<<stderr<<stderr.count();
 //if(stdout)
//this->ui->lineEdit_message->setText(stdout);
//on_pushButton_sendMessage_clicked();
//this->ui->lineEdit_message->setText(stderr);
//on_pushButton_sendMessage_clicked();

//mesajSlot("Komut Çalıştırıldı");
qDebug()<<"Komut Çalıştırıldı";
}
