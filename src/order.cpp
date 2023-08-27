#include "order.h"
#include <QDate>
#include <QDebug>
#include <QRandomGenerator>

Order::Order() {}

int Order::GetIdRetailer() { return id_retailer_; }

void Order::SetIdRetailer(int id) { id_retailer_ = id; }

void Order::PlaceOrder() {
  if (orders_.isEmpty())
    return;
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
  quint32 value = QRandomGenerator::global()->generate();
  QFile file(path + "/bd_local/orders/" + QString::number(value % 1000000));
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream out(&file);
    QMap<int, QPair<int, double>>::Iterator it = orders_.begin();
    out << id_retailer_ << ";"
        << QDateTime::currentDateTime().toString("HH:mm dd.MM.yy") << ";"
        << total_price_ << "\n";
    while (it != orders_.end()) {
      out << QString("%1;%2;%3\n")
                 .arg(QString::number(it.key()),
                      QString::number(it.value().first),
                      QString::number(it.value().second));
      ++it;
    }
    file.close();
  }
}

void Order::AddProduct(int count) {
  double price = sender()->objectName().section('_', 2, 2).toDouble();
  int id = sender()->objectName().section('_', 1, 1).toInt();
  int number_was = 0;
  if (orders_.contains(id))
    number_was = (*orders_.find(id)).first;
  total_price_ = total_price_ - (number_was - count) * price;
  if (count == 0) {
    orders_.erase(orders_.find(id));
  } else {
    orders_.insert(id, QPair<int, double>(count, price));
  }
}

void Order::AddProductFromFile(QString file_path) {
  auto file = QFile(file_path);
  if (file.open(QFile::ReadOnly)) {
    QString line = file.readLine();
    id_retailer_ = line.section(';', 0, 0).trimmed().toInt();
    total_price_ = line.section(';', 2, 2).trimmed().toDouble();
    while (!file.atEnd()) {
      line = file.readLine();
      QStringList list = line.split(';');
      QStringList::Iterator it = list.begin();
      orders_.insert((*it).toInt(), QPair<int, double>((*(++it)).toInt(),
                                                       (*(++it)).toDouble()));
    }
  }
}

double Order::GetTotalPrice() { return total_price_; }

void Order::Clear() {
  total_price_ = 0;
  id_retailer_ = 0;
  orders_.clear();
}

QPair<int, double> Order::QuantityInOrder(int id) {
  QPair<int, double> number_was(0, 0);
  if (orders_.contains(id))
    number_was = (*orders_.find(id));
  return number_was;
}
