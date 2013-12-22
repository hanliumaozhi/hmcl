#include "login.h"

OfficalLogin::reply_readyRead()
{

}

OfficalLogin::login(QString usr, QString pwd)
{
    QNetworkAccessManager manager;
    QHttpMultiPart part;
    QHttpPart part1;
    part1.setBody();
    part.append();
    QNetworkReply reply = manager.post(QNetworkRequest(QUrl("https://login.minecraft.net")), );
    QObject::connect(&reply, SIGNAL(readyRead()), SLOT(reply_readyRead()));
}
