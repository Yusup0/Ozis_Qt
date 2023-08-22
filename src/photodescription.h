#ifndef PHOTODESCRIPTION_H
#define PHOTODESCRIPTION_H

#include <QHBoxLayout>
#include "mylabel.h"

class PhotoDescription : public QHBoxLayout {
public:
    PhotoDescription(QString name);
    ~PhotoDescription();
    MyLabel *GetLabel();
    MyLabel *GetPhoto();
private:
    MyLabel *label_;
    MyLabel *photo_;
};

#endif // PHOTODESCRIPTION_H
