#include "photodescription.h"

PhotoDescription::PhotoDescription(QString name) {
    photo_ = new MyLabel();
    photo_->setObjectName("photo_" + name);
    photo_->setFixedSize(80, 80);
    label_ = new MyLabel();
    label_->setObjectName(name);
    label_->setWordWrap(true);
    this->addWidget(photo_);
    this->addWidget(label_);
}

PhotoDescription::~PhotoDescription() {
    delete label_;
    delete photo_;
}

MyLabel *PhotoDescription::GetLabel() {
    return label_;
}

MyLabel *PhotoDescription::GetPhoto() {
    return photo_;
}
