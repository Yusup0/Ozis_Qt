#ifndef PRODUCTVIEW_H
#define PRODUCTVIEW_H

#include <QHBoxLayout>
#include "mylabel.h"
#include "qspinbox.h"

class ProductView : public QHBoxLayout {
public:
    ProductView(QString name);
    ~ProductView();
    QSpinBox *GetSpinBox();
    QLabel *GetSumma();
    MyLabel *GetLabel();
    MyLabel *GetPhoto();
private:
    MyLabel *label_;
    MyLabel *photo_;
    QSpinBox *spin_box_;
    QLabel *summa_;

};

#endif // PRODUCTVIEW_H
