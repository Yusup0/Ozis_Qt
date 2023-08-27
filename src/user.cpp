#include "user.h"

void User::SetLogin(QString login) { login_ = login; }

void User::SetPassword(QString pass) { password_ = pass; }

void User::SetName(QString name) { name_ = name; }

void User::SetBearer(QString bearer) { bearer_ = bearer; }

QString User::GetLogin() { return login_; }

QString User::GetPassword() { return password_; }

QString User::GetName() { return name_; }

QString User::GetBearer() { return bearer_; }
