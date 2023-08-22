#ifndef PRODUXTCONTROLLER_H
#define PRODUXTCONTROLLER_H

#include "productmodel.h"

class ProductController
{
public:
    ProductController();
    void SetData(QFile *file, int contract_id, int category_id);
    QVector<int> GetProducts();
    void SetFile(QFile *file);
    void SetContractId(int contract_id);
    void SetCategoryId(int category_id);
private:
    ProductModel model_;
    QFile *file_;
    int contract_id_;
    int category_id_;
};

#endif // PRODUXTCONTROLLER_H
