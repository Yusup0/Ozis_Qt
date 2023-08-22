#include "api.h"
#include <QStandardPaths>

Api::Api() {}

void Api::RequestBearer(User& user) {
    QNetworkRequest request;
    request.setUrl(QUrl("https://ozisapp.ru/auth/jwt/login"));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    QString body =  QUrl::toPercentEncoding(user.GetLogin());
    body = "grant_type=&username=" + body + "&password=" + user.GetPassword() + "&scope=&client_id=&client_secret='";
    reply_ = manager_.post(request, body.toUtf8());
}

void Api::RequestRegister(User& user) {
    QNetworkRequest request;
    request.setUrl(QUrl("https://ozisapp.ru/auth/register"));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    QString body = "{\"email\": \"" + user.GetLogin() + "\",\n\"password\": \"" + user.GetPassword()
            + "\",\n\"is_active\": true,\n\"is_superuser\": false,\n\"is_verified\": false,\n\"name\": \""
                                           + user.GetName() + "\",\n\"role_id\": 1,\n\"chief\": 0\n}";
    reply_ = manager_.post(request, body.toUtf8());
}

void Api::RequestAddContract(User &user, QString contract) {
    QNetworkRequest request;
    request.setUrl(QUrl("https://ozisapp.ru/base/contract/add?contract_str=" + QUrl::toPercentEncoding(contract)));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Authorization", ("Bearer " + user.GetBearer()).toUtf8());
    reply_ = manager_.post(request, QByteArray());
}

void Api::RequestAddCategory(User &user, QString category) {
    QNetworkRequest request;
    request.setUrl(QUrl("https://ozisapp.ru/base/category/add?category_str=" + QUrl::toPercentEncoding(category)));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Authorization", ("Bearer " + user.GetBearer()).toUtf8());
    reply_ = manager_.post(request, QByteArray());
}


void Api::RequestAddRetailer(User &user, QStringList data) {
    QNetworkRequest request(QUrl("https://ozisapp.ru/base/store/add"));
    request.setRawHeader("Authorization", ("Bearer " + user.GetBearer()).toUtf8());

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QStringList::Iterator it = data.begin();
    QHttpPart textPart;
    QJsonObject obj;
    obj["name"] = *it++;
    obj["number"] = *it++;
    obj["address"] = *it++;
    QJsonDocument doc(obj);
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"new_store\""));
    textPart.setBody(doc.toJson());
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"photo\";filename=\"send.zip\""));
    QFile *file = new QFile(*it);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(textPart);
    multiPart->append(imagePart);
    reply_ = manager_.post(request, multiPart);
    multiPart->setParent(reply_);
}

void Api::RequestAddProduct(User &user, QStringList data) {
    QStringList::Iterator it = data.begin();
    QNetworkRequest request(QUrl("https://ozisapp.ru/base/product/add?count1=" + *it++));
    request.setRawHeader("Authorization", ("Bearer " + user.GetBearer()).toUtf8());

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    QJsonObject obj;
    obj["name"] = *it++;
    obj["contract_id"] = *it++;
    obj["price"] = *it++;
    obj["category"] = *it++;
    obj["article"] = *it++;
    obj["description"] = *it++;
    QJsonDocument doc(obj);
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"new_product\""));
    textPart.setBody(doc.toJson());
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"archive\";filename=\"send.zip\""));
    QFile *file = new QFile(*it);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(textPart);
    multiPart->append(imagePart);
    reply_ = manager_.post(request, multiPart);
    multiPart->setParent(reply_);
}

void Api::RequestGetBD(User &user) {
    QNetworkRequest request;
    request.setUrl(QUrl("https://ozisapp.ru/base/get"));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Authorization", ("Bearer " + user.GetBearer()).toUtf8());
    reply_ = manager_.get(request);
}

void Api::RequestAddOrder(User &user, QString path) {
    QNetworkRequest request;
    request.setUrl(QUrl("https://ozisapp.ru/base/order/add"));
    request.setRawHeader("Authorization", ("Bearer " + user.GetBearer()).toUtf8());
    QHttpMultiPart *multi_part = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart file_image_part;
    QString r = QString("form-data; name=\"order\";filename=\"%1\"").arg(path.split('/').back().trimmed());
    file_image_part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(r));
    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    file_image_part.setBodyDevice(file);
    file->setParent(multi_part);
    multi_part->append(file_image_part);
    reply_ = manager_.post(request, multi_part);
    multi_part->setParent(reply_);
}

//int Api::RequestBearer(User& user) {
//    cpr::Session session;
//    session.SetUrl(cpr::Url{"https://ozisapp.ru/auth/jwt/login"});
//    session.SetHeader(cpr::Header{{"accept", "application/json"}});
//    session.SetHeader(cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}});
//    std::string login = user.GetLogin();
//    login.replace(login.find("@"), 1, "%40");
//    session.SetBody(cpr::Body{"grant_type=&username=" + login + "&password=" + user.GetPassword() + "&scope=&client_id=&client_secret='"});
//    cpr::Response res = session.Post();;
//    if (res.status_code == 200) {
//        user.SetBearer(res.text.substr(17, 43));
//    }
//    return res.status_code;
//    return 0;
//}

//int Api::RequestRegister(User& user) {
//    cpr::Session session;
//    session.SetUrl(cpr::Url{"https://ozisapp.ru/auth/register"});
//    session.SetHeader(cpr::Header{{"accept", "application/json"}});
//    session.SetHeader(cpr::Header{{"Content-Type", "application/json"}});
//    session.SetBody(cpr::Body{"{\"email\": \"" + user.GetLogin() + "\",\n\"password\": \"" + user.GetPassword()
//                              + "\",\n\"is_active\": true,\n\"is_superuser\": false,\n\"is_verified\": false,\n\"name\": \""
//                              + user.GetName() + "\",\n\"role_id\": 1,\n\"chief\": 0\n}" });
//    cpr::Response res = session.Post();
//    return res.status_code;
//    return 0;
//}

//int Api::RequestAddContract(User &user, std::string contract) {
//    cpr::Session session;
//    qDebug() << url_encode(contract);
//    session.SetUrl(cpr::Url{"https://ozisapp.ru/base/contract/add?contract_str=" + url_encode(contract)});
//    session.SetHeader(cpr::Header{{"accept", "application/json"}});
//    session.SetBearer(cpr::Bearer(user.GetBearer()));
//    cpr::Response res = session.Post();
//    return res.status_code;
//    return 0;
//}

//int Api::RequestAddCategory(User &user, std::string category) {
//    cpr::Session session;
//    session.SetUrl(cpr::Url{"https://ozisapp.ru/base/category/add?category_str=" + url_encode(category)});
//    session.SetHeader(cpr::Header{{"accept", "application/json"}});
//    session.SetBearer(cpr::Bearer(user.GetBearer()));
//    cpr::Response res = session.Post();
//    return res.status_code;
//    return 0;
//}

//int Api::RequestGetBD(User &user, std::string dir_add_zip) {
//    cpr::Session session;
//    session.SetUrl(cpr::Url{"https://ozisapp.ru/base/get"});
//    session.SetHeader(cpr::Header{{"accept", "application/json"}});
//    session.SetBearer(cpr::Bearer(user.GetBearer()));
//    std::ofstream of(dir_add_zip, std::ios::binary);
//    cpr::Response res = session.Download(of);
//    of.close();
//    return res.status_code;
//    return 0;
//}











