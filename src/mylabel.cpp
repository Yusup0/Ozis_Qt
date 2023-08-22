#include "mylabel.h"

MyLabel::MyLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

MyLabel::~MyLabel() {}

void MyLabel::mouseReleaseEvent(QMouseEvent* event) {
    emit clicked(this->objectName());
}
