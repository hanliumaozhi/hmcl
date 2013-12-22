#ifndef LOGIN_H
#define LOGIN_H

#include <QString>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpMultiPart>

class Login
{
public:
    virtual bool login(QString usr, QString pwd);
};

class OfficalLogin : Login
{
public:
    void reply_readyRead();
    bool login(QString usr, QString pwd);
    OfficalLogin();
};

#endif // LOGIN_H
