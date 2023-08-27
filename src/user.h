#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
  User() {}
  void SetLogin(QString login);
  void SetPassword(QString pass);
  void SetName(QString name);
  void SetBearer(QString bearer);
  QString GetLogin();
  QString GetPassword();
  QString GetBearer();
  QString GetName();

private:
  QString name_;
  QString login_;
  QString password_;
  QString bearer_;
};

#endif // USER_H
