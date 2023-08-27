#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>

class MyLabel : public QLabel {
  Q_OBJECT
public:
  explicit MyLabel(QWidget *parent = Q_NULLPTR,
                   Qt::WindowFlags f = Qt::WindowFlags());
  ~MyLabel();

signals:
  void clicked(QString name);

protected:
  void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MYLABEL_H
