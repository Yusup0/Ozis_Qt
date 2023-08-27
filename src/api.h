#ifndef API_H
#define API_H

#include "user.h"
#include <QFile>
#include <QHttpMultiPart>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>
#include <QString>

class Api : public QObject {

public:
  Api();
  void RequestBearer(User &user);
  void RequestRegister(User &user);
  void RequestAddContract(User &user, QString contract);
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
