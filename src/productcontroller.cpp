#include "productcontroller.h"

ProductController::ProductController() {}

void ProductController::SetData(QFile *file, int contract_id, int category_id) {
  file_ = file;
  contract_id_ = contract_id;
  category_id_ = category_id;
}

QVector<int> ProductController::GetProducts() {
  model_.SetFile(file_);
  model_.SetContractId(contract_id_);
  model_.SetCategoryId(category_id_);
  return model_.GetAdress();
}

void ProductController::SetFile(QFile *file) { file_ = file; }

void ProductController::SetContractId(int contract_id) {
  contract_id_ = contract_id;
}

void ProductController::SetCategoryId(int category_id) {
  category_id_ = category_id;
}
