#include "productview.h"
#include "mylabel.h"
#include "productmodel.h"
#include "qspinbox.h"

ProductView::ProductView(QString name) {
  photo_ = new MyLabel();
  photo_->setFixedSize(80, 80);
  label_ = new MyLabel();
  label_->setObjectName(name);
  label_->setWordWrap(true);
  spin_box_ = new QSpinBox();
  spin_box_->setObjectName("spin_" + name);
  spin_box_->setWrapping(true);
  spin_box_->setMinimumSize(QSize(1, 60));
  spin_box_->setAlignment(Qt::AlignRight);
  QHBoxLayout *layout_h_lable = new QHBoxLayout();
  layout_h_lable->addStretch();
  layout_h_lable->addWidget(spin_box_);
  summa_ = new QLabel();
  summa_->setObjectName("label_" + name);
  QVBoxLayout *layout_v_lable = new QVBoxLayout();
  layout_v_lable->addLayout(layout_h_lable);
  layout_v_lable->addWidget(summa_);
  this->addWidget(photo_);
  this->addWidget(label_);
  this->addLayout(layout_v_lable);
  this->setStretch(0, 0);
  this->setStretch(1, 5);
  this->setStretch(2, 0);
}

// void ProductView::FillData(ProductData data){
//     photo_->setPixmap(QPixmap(QString(beginning_path_ +
//     "/bd/photo_product/%1/logo.jpg").arg(data.data_.first())));
//     label_->setText(QString("<p align=left><font size=5>%1</font>"
//                          "<br/><font size=3>%2 руб. %3
//                          шт.</font></p>").arg(data.data_[1], data.data_[3],
//                          data.data_[7]));
//     spin_box_->setRange(0, data.data_[7].toInt());
// }

ProductView::~ProductView() {
  delete label_;
  delete photo_;
  delete spin_box_;
  delete summa_;
}

MyLabel *ProductView::GetLabel() { return label_; }

MyLabel *ProductView::GetPhoto() { return photo_; }

QSpinBox *ProductView::GetSpinBox() { return spin_box_; }

QLabel *ProductView::GetSumma() { return summa_; }
