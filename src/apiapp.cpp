#include "apiapp.h"
#include "assistant.h"
#include "user.h"
#include <QMainWindow>
#include <cstring>
#include <iostream>
#include <zip.h>

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>

long RequestBearer(User &user, QNetworkAccessManager *manager) {
  QNetworkRequest request;
  request.setUrl(QUrl("https://ozisapp.ru/"));
  request.setRawHeader("accept:", "application/json");
  QNetworkReply *reply = manager->get(request);
  QFile *file = new QFile("/home/usup/Desktop/file.txt");
  file->open(QFile::WriteOnly);
  file->write(reply->readAll());
  file->close();
  return reply->error();
}

// long RequestBearer(User& user, QNetworkAccessManager *manager) {
//     QNetworkRequest request;
//     request.setUrl(QUrl("https://ozisapp.ru/auth/jwt/login"));
//     request.setRawHeader("accept", "application/json");
//     request.setRawHeader("Content-Type",
//     "application/x-www-form-urlencoded"); QString login = "t%40yandex.ru";
//     login = "grant_type=&username=" + login + "&password=" + "ing" +
//     "&scope=&client_id=&client_secret='"; QByteArray bytes = login.toUtf8();
//     QNetworkReply *reply = manager->post(request, bytes);
//     QFile *file = new QFile("/home/usup/Desktop/file.xml");
//     file->open(QFile::WriteOnly);
//     file->write(reply->readAll());
//     file->close();
//     qDebug() <<  reply->readAll();
//     return reply->error();
// }

long RequestBearer1(User &user) {
  cpr::Session session;
  session.SetUrl(cpr::Url{"https://ozisapp.ru/auth/jwt/login"});
  session.SetHeader(cpr::Header{{"accept", "application/json"}});
  session.SetHeader(
      cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}});
  std::string login = user.GetLogin();
  login.replace(login.find("@"), 1, "%40");
  session.SetBody(cpr::Body{"grant_type=&username=" + login +
                            "&password=" + user.GetPassword() +
                            "&scope=&client_id=&client_secret='"});
  cpr::Response res = session.Post();
  ;
  if (res.status_code == 200) {
    user.SetBearer(res.text.substr(17, 43));
  }
  return res.status_code;
}

long RequestRegister(User &user) {
  cpr::Session session;
  session.SetUrl(cpr::Url{"https://ozisapp.ru/auth/register"});
  session.SetHeader(cpr::Header{{"accept", "application/json"}});
  session.SetHeader(cpr::Header{{"Content-Type", "application/json"}});
  session.SetBody(cpr::Body{"{\"email\": \"" + user.GetLogin() +
                            "\",\n\"password\": \"" + user.GetPassword() +
                            "\",\n\"is_active\": true,\n\"is_superuser\": "
                            "false,\n\"is_verified\": false,\n\"name\": \"" +
                            user.GetName() +
                            "\",\n\"role_id\": 1,\n\"chief\": 0\n}"});
  cpr::Response res = session.Post();
  return res.status_code;
}

long RequestAddContract(User &user, std::string contract) {
  cpr::Session session;
  qDebug() << url_encode(contract);
  session.SetUrl(cpr::Url{"https://ozisapp.ru/base/contract/add?contract_str=" +
                          url_encode(contract)});
  session.SetHeader(cpr::Header{{"accept", "application/json"}});
  session.SetBearer(cpr::Bearer(user.GetBearer()));
  cpr::Response res = session.Post();
  return res.status_code;
}

long RequestAddCategory(User &user, std::string category) {
  cpr::Session session;
  session.SetUrl(cpr::Url{"https://ozisapp.ru/base/category/add?category_str=" +
                          url_encode(category)});
  session.SetHeader(cpr::Header{{"accept", "application/json"}});
  session.SetBearer(cpr::Bearer(user.GetBearer()));
  cpr::Response res = session.Post();
  return res.status_code;
}

long RequestGetBD(User &user, std::string dir_add_zip) {
  cpr::Session session;
  session.SetUrl(cpr::Url{"https://ozisapp.ru/base/get"});
  session.SetHeader(cpr::Header{{"accept", "application/json"}});
  session.SetBearer(cpr::Bearer(user.GetBearer()));
  std::ofstream of(dir_add_zip, std::ios::binary);
  cpr::Response res = session.Download(of);
  of.close();
  return res.status_code;
}
