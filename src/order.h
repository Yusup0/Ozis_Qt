#ifndef ORDER_H
#define ORDER_H

#include <QFile>
#include <QMap>
#include <QObject>
#include <QStandardPaths>

class Order : public QObject {
    Q_OBJECT
public:
    Order();
    void AddProductFromFile(QString file_path);
    int GetIdRetailer();
    void SetIdRetailer(int id);
    void PlaceOrder();
    QPair<int, double> QuantityInOrder(int id);
public slots:
    void AddProduct(int id);
    double GetTotalPrice();
    void Clear();
private:
    QFile *file_;
    QMap<int, QPair<int, double>> orders_;
    int id_retailer_;
    double total_price_;

};

#endif // ORDER_H
