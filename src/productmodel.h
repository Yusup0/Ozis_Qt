#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include <QAbstractItemModel>
#include <QFile>

struct ProductData {
  QVector<QString> data_;

  friend QString &operator<<(QString &writer, const ProductData &item) {
    for (const auto &i : item.data_) {
      writer += i;
    }
    return writer;
  }

  friend void operator>>(const QString &writer, ProductData &item) {
    for (const auto &i : writer.split(';')) {
      item.data_.push_back(i);
    }
  }
};

struct Contract {
  struct Category {
    Category() {}
    Category(int id, int address) : id_(id), pos_(address) {}
    int id_;
    int pos_;
    int count_ = 1;
  };
  Contract(int id) : id_(id) {}
  int id_;
  int count_ = 1;
  QVector<Category> category_;
};

class ProductModel : public QAbstractItemModel {

public:
  explicit ProductModel(QObject *parent = nullptr);
  ProductModel(QFile *file, QObject *parent = nullptr);
  ~ProductModel();
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  void SetFile(QFile *file);
  void SetId(int contract_id, int category_id);
  ProductData GetData();
  int SizeData();
  ProductData GetLine(int id);
  void ReadFile();

private:
  void SetAdress();
  QFile *file_;
  QVector<QPair<int, int>> data_pos_;
  QVector<Contract> addresses_;
  QMap<int, int> id_addresses_;
  int contract_id_, category_id_;
  int line_numer;
  QString lact_category_id_;
  QString lact_cantract_id_;
  int count, count1;
};

#endif // PRODUCTMODEL_H
