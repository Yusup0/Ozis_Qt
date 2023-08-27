#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "api.h"
#include "order.h"
#include "productmodel.h"
#include "ui_mainwindow.h"
#include "user.h"
#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QSettings>
#include <QStandardPaths>
#include <string.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  //    void on_listWidget_itemClicked(QListWidgetItem *item);
  //    void loadImage();

  void ResponseBearer();
  void ResponseRegister();
  void ResponseBDAdd();
  void ResponseArchiveBD();
  void ResponseAddOrder();
  void PriceChange(int number);
  void PushButtonClicked();
  void LabelClicked(QString text);
  void Label1Clicked(QString text);
  void Label2Clicked(QString text);
  void Label3Clicked(QString text);
  void Label4Clicked(QString text);
  void Label5Clicked(QString text);
  void on_pushButton_4_clicked();
  void on_pushButton_5_clicked();
  void on_pushButton_6_clicked();
  void on_pushButton_7_clicked();
  void on_pushButton_8_clicked();
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_10_clicked();
  void on_pushButton_9_clicked();
  void on_pushButton_13_clicked();
  void on_pushButton_17_clicked();
  void on_pushButton_18_clicked();
  void on_pushButton_15_clicked();
  void on_pushButton_clicked();
  void on_pushButton_12_clicked();
  void on_pushButton_14_clicked();
  void on_pushButton_19_clicked();
  void on_pushButton_20_clicked();
  void on_pushButton_24_clicked();
  void on_spinBox_valueChanged(int arg1);
  void on_pushButton_25_clicked();
  void on_pushButton_26_clicked();
  void on_pushButton_21_clicked();
  void on_pushButton_22_clicked();

  void on_pushButton_27_clicked();

  void on_pushButton_23_clicked();

  void on_pushButton_28_clicked();

  void on_pushButton_29_clicked();

private:
  void AnimationLeft();
  void AnimationRight();
  void OrderWindow(QScrollArea *scroll_area, QString file_path);
  void OrderOnWidgit(QScrollArea *scroll_area, QList<QList<QString>> date);
  void ProductWindow(QScrollArea *scroll_area, QString id);
  void ProductOnWidgit(QScrollArea *scroll_area);
  void CategoryOnWidgit(QScrollArea *scroll_area);
  void ContractOnWidgit(QScrollArea *scroll_area);
  void RetailerOnWidgit(QScrollArea *scroll_area);
  void WriteSettings();
  void ReadSettings();
  QString GetIdBD(QFile file, QString name);
  QString GetNameBD(QFile file, QString id);
  QList<QPair<QString, QString>> GetIdName(QFile file);
  QStringList GetListBD(QFile file);
  Ui::MainWindow *ui;
  User user_;
  Api api_;
  QLabel *active_label_;
  QLineEdit *active_line_;
  QStringList data_;
  QString beginning_path_;
  ProductModel model_;
  QFile *file_product_;
  QSettings settings_;
  Order order_;
  QPropertyAnimation *animation_right;
  QPropertyAnimation *animation_left;
};
#endif // MAINWINDOW_H
