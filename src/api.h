#ifndef API_H
#define API_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QFile>
#include <QIODevice>
#include "user.h"
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegExp>

class Api : public QObject                                                          {

public:
    Api();
    void RequestBearer(User& user);
    void RequestRegister(User& user);
    void RequestAddContract(User& user, QString contract);
    void RequestAddCategory(User &user, QString category);
    void RequestGetBD(User &user);
    void RequestAddRetailer(User &user, QStringList data);
    void RequestAddProduct(User &user, QStringList data);
    void RequestAddOrder(User &user, QString path);
    QNetworkReply *reply_;

private slots:
    void ResponseBearer();
private:
    QNetworkAccessManager manager_;

};

#endif // API_H
