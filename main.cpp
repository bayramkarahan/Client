#include <QCoreApplication>
#include<client.h>
#include <QString>
#include <QDir>
#include<QDebug>
#include<singleinstance.h>
#define SINGLE_INSTANCE ".Client"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

     QString name = SINGLE_INSTANCE;

    SingleInstance cInstance;
    if(cInstance.hasPrevious(name, QCoreApplication::arguments()))
    {
        qDebug() << "Client Zaten Açık...";
        return 0;
    }
    if (cInstance.listen(name)) {
        qDebug() << "Client Çalışıyor..";
    } else {
        qDebug() << "Client Çalışması İptal Edildi...";
        return 0;
    }


        Client *cl=new Client();

    return a.exec();
}
