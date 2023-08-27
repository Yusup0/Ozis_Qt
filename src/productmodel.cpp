#include "productmodel.h"

ProductModel::ProductModel(QObject *parent) : QAbstractItemModel{parent} {}

ProductModel::ProductModel(QFile *file, QObject *parent)
    : QAbstractItemModel{parent} {
  SetFile(file);
}

ProductModel::~ProductModel() {}

QModelIndex ProductModel::index(int row, int column,
                                const QModelIndex &parent) const {
  return createIndex(row, column);
}

QModelIndex ProductModel::parent(const QModelIndex &index) const {
  return QModelIndex();
}

int ProductModel::rowCount(const QModelIndex &parent) const {
  return data_pos_.size();
}

int ProductModel::columnCount(const QModelIndex &parent) const { return 8; }

QVariant ProductModel::data(const QModelIndex &index, int role) const {
  //    if (!index.isValid() || data_pos_.isEmpty() || index.row() >
  //    data_pos_.size() || index.column() > 7)
  //        return QVariant();
  //    file_->seek(data_pos_.at(index.row()));

  //    ProductData tmp;
  //    QString(file_->readLine()) >> tmp;

  //    if (role == Qt::DisplayRole) return tmp.data_[index.column()];

  return QVariant();
}

void ProductModel::ReadFile() {
  if (!file_->open(QFile::ReadOnly))
    return;
  addresses_.clear();
  int count_contract_id = 0, count_category_id_ = 0;
  while (!file_->atEnd()) {
    qint64 pos = file_->pos();
    QString line = file_->readLine();
    int product_id = line.section(';', 0, 0).toInt();
    int contract_id = line.section(';', 2, 2).toInt();
    int category_id = line.section(';', 4, 4).toInt();
    id_addresses_.insert(product_id, pos);
    if (addresses_.isEmpty() || addresses_.back().id_ != contract_id) {
      addresses_.push_back(Contract(contract_id));
    } else {
      ++addresses_.back().count_;
    }
    if (addresses_.back().category_.isEmpty() ||
        addresses_.back().category_.back().id_ != category_id) {
      addresses_.back().category_.push_back(
          Contract::Category(category_id, pos));
    } else {
      ++addresses_.back().category_.back().count_;
    }
  }
  file_->close();
}

void ProductModel::SetAdress() {
  data_pos_.clear();
  if (contract_id_ != 0 && category_id_ != 0) {
    for (auto &i : addresses_) {
      if (i.id_ != contract_id_)
        continue;
      for (auto j : i.category_) {
        if (j.id_ != category_id_)
          continue;
        data_pos_.push_back(QPair<int, int>(j.pos_, j.count_));
      }
    }
  } else if (contract_id_ != 0) {
    for (auto &i : addresses_) {
      if (i.id_ != contract_id_)
        continue;
      for (auto j : i.category_) {
        data_pos_.push_back(QPair<int, int>(j.pos_, j.count_));
      }
    }
  } else if (category_id_ != 0) {
    for (auto &i : addresses_) {
      for (auto j : i.category_) {
        if (j.id_ != category_id_)
          continue;
        data_pos_.push_back(QPair<int, int>(j.pos_, j.count_));
      }
    }
  } else {
    for (auto &i : addresses_) {
      for (auto j : i.category_) {
        data_pos_.push_back(QPair<int, int>(j.pos_, j.count_));
      }
    }
  }
}

void ProductModel::SetId(int contract_id, int category_id) {
  contract_id_ = contract_id;
  category_id_ = category_id;
  SetAdress();
  count = 0;
  count1 = 0;
  if (!data_pos_.isEmpty())
    file_->seek(data_pos_[0].first);
}

ProductData ProductModel::GetData() {
  ProductData tmp;
  if (data_pos_[count].second <= count1) {
    file_->seek(data_pos_[++count].first);
    count1 = 1;
  } else {
    count1++;
  }
  QString(file_->readLine()) >> tmp;
  return tmp;
}

int ProductModel::SizeData() {
  int res = 0;
  for (auto &i : data_pos_) {
    res += i.second;
  }
  return res;
}

ProductData ProductModel::GetLine(int id) {
  ProductData res;
  if (file_->open(QFile::ReadOnly)) {
    file_->seek(*id_addresses_.find(id));
    file_->readLine() >> res;
    file_->close();
  }
  return res;
}

void ProductModel::SetFile(QFile *file) { file_ = file; }
