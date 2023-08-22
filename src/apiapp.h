#ifndef API_H
#define API_H

#include <cpr/cpr.h>
#include "user.h"
#include <QNetworkAccessManager>


long RequestBearer(User& user, QNetworkAccessManager *manager);
long RequestRegister(User& user);
long RequestAddContract(User& user, std::string contract);
long RequestAddCategory(User &user, std::string category);
long RequestGetBD(User &user, std::string dir_add_zip);
std::string url_encode(const std::string &value);
#endif // API_H
