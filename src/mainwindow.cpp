#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QListWidgetItem>
#include <QListWidget>
#include <QSpinBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QSaveFile>
#include <QDir>
#include <QDirIterator>
#include "assistant.h"
#include "mylabel.h"
#include "productview.h"
#include "photodescription.h"
#include <QFileDialog>
#include <QScroller>
#include <QPainter>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), settings_("Ozis", "Ozis0.1") {
    ui->setupUi(this);

    ReadSettings();
    ui->stackedWidget_2->setCurrentWidget(ui->page_4);
    if (user_.GetLogin() != "there is no login") {
        api_.RequestBearer(user_);
        connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseBearer);
    }
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_2, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_3, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_4, QScroller::LeftMouseButtonGesture);

    AnimationLeft();
    AnimationRight();
    beginning_path_ = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    file_product_ = new QFile(beginning_path_ + "/bd/product.csv");
    model_.SetFile(file_product_);
    model_.ReadFile();

    QDir dir(beginning_path_ + "/bd_local/orders");
    if (!dir.exists())
        dir.mkpath(".");
}

MainWindow::~MainWindow() {
    delete ui;
    delete file_product_;
    WriteSettings();
}


void MainWindow::on_pushButton_4_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_change_contend);
    ui->stackedWidget->setCurrentWidget(ui->page_contract);
    animation_right->start();
}


void MainWindow::on_pushButton_5_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_change_contend);
    ui->stackedWidget->setCurrentWidget(ui->page_product);
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_2->addItems(GetListBD(QFile(beginning_path_ + "/bd/contract.csv")));
    ui->comboBox_3->addItems(GetListBD(QFile(beginning_path_ + "/bd/category.csv")));
    data_.clear();
    animation_right->start();
}

void MainWindow::on_pushButton_6_clicked() {
    if (ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty()) return;
    user_.SetLogin(ui->lineEdit_2->text());
    user_.SetPassword(ui->lineEdit_3->text());
    api_.RequestBearer(user_);
    connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseBearer);
}

void MainWindow::ResponseBearer() {
    int status_code = api_.reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status_code == 200) {
        user_.SetBearer(api_.reply_->readAll().mid(17, 43));
//        on_pushButton_2_clicked();
        ui->stackedWidget_2->setCurrentWidget(ui->page_3);
        animation_right->start();
    } else {
        ui->label_4->setText("Не правильный пароль или логин");
    }
}

void MainWindow::on_pushButton_7_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_5);
    animation_right->start();
}

void MainWindow::on_pushButton_8_clicked() {
    if (ui->lineEdit_4->text().isEmpty() || ui->lineEdit_5->text().isEmpty()
        || ui->lineEdit->text().isEmpty()) return;
    user_.SetLogin(ui->lineEdit_4->text());
    user_.SetPassword(ui->lineEdit_5->text());
    user_.SetName(ui->lineEdit->text());
    api_.RequestRegister(user_);
    connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseRegister);
}

void MainWindow::ResponseRegister() {
    int status_code = api_.reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status_code == 201) {
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->stackedWidget_2->setCurrentWidget(ui->page_4);
        animation_left->start();
    } else if (status_code == 400) {
        ui->label_5->setText("Пользователь с такой почтой уже существует");
    } else if (status_code == 422) {
        ui->label_5->setText("Данные не коректны");
    } else {
        ui->label_5->setText("Ошибка");
    }
}

void MainWindow::ResponseBDAdd() {
    QString name = sender()->objectName();
    QList<QLineEdit*> list;
    QLabel *text_lable;
    if (name == "contract") {
        list << ui->lineEdit_6;
        text_lable = ui->label_6;
    } else if (name == "category") {
        list << ui->lineEdit_15;
        text_lable = ui->label_7;
    } else if (name == "retailer") {
        list << ui->lineEdit_7 << ui->lineEdit_9 << ui->lineEdit_10;
        text_lable = ui->label_14;
    } else if (name == "product") {
        list << ui->lineEdit_8 << ui->lineEdit_11 << ui->lineEdit_12 << ui->lineEdit_13;
        text_lable = ui->label_18;
    }
    int status_code = api_.reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status_code == 200) {
        for (auto &i: list) i->clear();
        text_lable->setText("Успешно");
        if (name == "product") {
            ui->textEdit->clear();
        }
    } else {
        text_lable->setText("Ошибка");
    }
}

void MainWindow::ResponseArchiveBD() {
    int status_code = api_.reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status_code == 200) {

        QFile *file = new QFile(beginning_path_ + "/bd/archiv.zip");
        if(file->open(QFile::WriteOnly)) {
            file->write(api_.reply_->readAll());
            file->close();
        }
        UnZip(beginning_path_ + "/bd/archiv.zip", beginning_path_ + "/bd/");
        model_.SetFile(file_product_);
        model_.ReadFile();

        qDebug() << "готово";
        api_.reply_->deleteLater();
        file->remove();
    } else {
        qDebug() << "что то пошло не так";
    }
}

void MainWindow::PriceChange(int number) {
    static_cast<QSpinBox*>(sender())->clearFocus();
    ui->label_15->setText(QString("<p align=center><font size=5>%L1</font>").arg(order_.GetTotalPrice()));

    QString ob_name = "label_" + sender()->objectName().section('_', 1, 1);
    double price = order_.QuantityInOrder(ob_name.section('_', 1, 1).toInt()).second;

    for (auto &widget: ui->scrollArea_3->widget()->children()) {
        if (widget->objectName() == ob_name) {
            static_cast<QLabel*>(widget)->setText(QString("%L1").arg(number ? (QString::number(price * number)) : "" ));
            break;
        }
    }
}


void MainWindow::on_pushButton_2_clicked() {
    QDir dir(beginning_path_ + "/bd/");
    if (dir.exists())
        dir.removeRecursively();
    dir.mkpath(".");
    api_.RequestGetBD(user_);
    qDebug() << "загрузка";
    connect(api_.reply_, &QNetworkReply::finished, this, &MainWindow::ResponseArchiveBD);
}

void MainWindow::on_pushButton_3_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_contend);
    animation_right->start();
}

void MainWindow::on_pushButton_10_clicked() {
    if (ui->lineEdit_6->text().isEmpty()) return;
    api_.RequestAddContract(user_, ui->lineEdit_6->text());
    api_.reply_->setObjectName("contract");
    connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseBDAdd);
}

void MainWindow::on_pushButton_9_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_change_contend);
    ui->stackedWidget->setCurrentWidget(ui->page_store);
    data_.clear();
    animation_right->start();
}

void MainWindow::on_pushButton_13_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_change_contend);
    ui->stackedWidget->setCurrentWidget(ui->page_catagory);
    animation_right->start();
}

void MainWindow::on_pushButton_17_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_contend);
    animation_left->start();
}

void MainWindow::on_pushButton_18_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->page_3);
    animation_left->start();
}

void MainWindow::on_pushButton_15_clicked() {
    if (ui->lineEdit_15->text().isEmpty()) return;
    api_.RequestAddCategory(user_, ui->lineEdit_15->text());
    api_.reply_->setObjectName("category");
    connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseBDAdd);
}

void MainWindow::on_pushButton_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->order);
    ui->label_16->setText("Магазины");
    RetailerOnWidgit(ui->scrollArea);  
    animation_right->start();
    order_.Clear();
    data_.clear();
    data_.resize(2);
}

void MainWindow::OrderOnWidgit(QScrollArea *scroll_area, QList<QList<QString>> date) {
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *layoutV = new QVBoxLayout(scrollWidget);
    scroll_area->setWidget(scrollWidget);
    QStringList result;
    QList<QList<QString>>::Iterator date_it = date.begin();
    while (date_it != date.end()) {
        QList<QString>::Iterator data_dir = (date_it++)->begin();
        QDir dir(*(data_dir));
        auto list_file = dir.entryList(QDir::AllEntries | QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
        auto it = list_file.begin();
        if (!list_file.isEmpty()) {
            auto separator = new QLabel(scrollWidget);
            separator->setText(*(++data_dir));
            separator->setStyleSheet(QString("QLabel { background-color : %1; color : %2; }").arg(*(++data_dir), *(++data_dir)));
            layoutV->addWidget(separator);
        }
        while (it != list_file.end()) {
        auto file = QFile(dir.path() + "/" + *(it));
        QString number_order = *(it++);
            if (file.open(QIODevice::ReadOnly)) {
                QString line = file.readLine();
                file.close();
                result = line.split(';');
                QStringList::Iterator line_it = result.begin();
                auto layout_h = new PhotoDescription(*line_it);
                layout_h->GetPhoto()->setPixmap(QPixmap(QString(beginning_path_ + "/bd/photo_store/%1/logo.jpg").arg(*line_it)));
                layout_h->GetLabel()->setObjectName(dir.path()+ "/" + number_order);
                QString name = GetNameBD(beginning_path_ + "/bd/retailer.csv", *line_it);
                layout_h->GetLabel()->setText(QString("<p> <span  align=left><font size=5>№%1 %2</font><br/>"
                                       "<br/><font size=3>%3, сумма - %4 руб.</font></p>").arg(number_order, name, *(++line_it), *(++line_it)));
                connect(layout_h->GetLabel(), &MyLabel::clicked, this, &MainWindow::Label4Clicked);
                layoutV->addLayout(layout_h);
            }
        }
    }
    layoutV->addStretch();
}

void MainWindow::RetailerOnWidgit(QScrollArea *scroll_area) {
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *layoutV = new QVBoxLayout(scrollWidget);
    scroll_area->setWidget(scrollWidget);
    QStringList result;
    auto file = QFile(beginning_path_ + "/bd/retailer.csv");
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            result = line.split(';');
            QStringList::Iterator it = result.begin();
            auto layout_h = new PhotoDescription(*it);
            layout_h->GetPhoto()->setPixmap(QPixmap(QString(beginning_path_ + "/bd/photo_store/%1/logo.jpg").arg(*it)));
            layout_h->GetLabel()->setText(QString("<p align=left><font size=5>%1</font><br/><br/><font size=3>%2</font></p>").arg(*(++it), *(++(++it))));
            connect(layout_h->GetLabel(), &MyLabel::clicked, this, &MainWindow::LabelClicked);
            layoutV->addLayout(layout_h);
        }
        file.close();
    }
    layoutV->addStretch();
}

void MainWindow::WriteSettings() {
    settings_.beginGroup("/Settings");
    settings_.setValue("/login", user_.GetLogin());
    settings_.setValue("/password", user_.GetPassword());
    settings_.endGroup();
}

void MainWindow::ReadSettings() {
    settings_.beginGroup("/Settings");
    user_.SetLogin(settings_.value("/login", "there is no login").toString());
    user_.SetPassword(settings_.value("/password", "there is no password").toString());
    settings_.endGroup();
}

void MainWindow::ContractOnWidgit(QScrollArea *scroll_area) {
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *layoutV = new QVBoxLayout(scrollWidget);
    scroll_area->setWidget(scrollWidget);
    QList<QPair<QString, QString>> contracts;
    contracts << QPair<QString, QString>("0", "Все контракты");
    contracts <<  GetIdName((QFile(beginning_path_ + "/bd/contract.csv")));
    for (auto &contract: contracts) {
        auto lable = new MyLabel(scrollWidget);
        lable->setObjectName(contract.first);
        lable->setText(QString("<p align=left><font size=5>%1").arg(contract.second));
        connect(lable, &MyLabel::clicked, this, &MainWindow::Label1Clicked);
        layoutV->addWidget(lable);
    }
    layoutV->addStretch();
}

void MainWindow::CategoryOnWidgit(QScrollArea *scroll_area) {
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *layoutV = new QVBoxLayout(scrollWidget);
    scroll_area->setWidget(scrollWidget);
    QList<QPair<QString, QString>> categories;
    categories << QPair<QString, QString>("0", "Все категории");
    categories << GetIdName((QFile(beginning_path_ + "/bd/category.csv")));
    for (auto &category: categories) {
        auto lable = new MyLabel(scrollWidget);
        lable->setObjectName(category.first);
        lable->setText(QString("<p align=left><font size=5>%1").arg(category.second));
        connect(lable, &MyLabel::clicked, this, &MainWindow::Label2Clicked);
        layoutV->addWidget(lable);
    }
    layoutV->addStretch();
}

void MainWindow::ProductOnWidgit(QScrollArea *scroll_area) {
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *layoutV = new QVBoxLayout(scrollWidget);
    scroll_area->setWidget(scrollWidget);
    int contract_id = data_[0].toInt(), category_id = data_[1].toInt();
    if(file_product_->open(QFile::ReadOnly)) {
        model_.SetId(contract_id, category_id);
        int size  = model_.SizeData();
        for (int i = 0; i < size; i++) {
            auto line = model_.GetData();
            ProductView *layout_h = new ProductView(line.data_.first());
            layout_h->GetPhoto()->setPixmap(QPixmap(QString(beginning_path_ + "/bd/photo_product/%1/logo.jpg").arg(line.data_.first())));
            layout_h->GetLabel()->setText(QString("<p align=left><font size=5>%1</font><br/>"
                                                  "<br/><font size=3>%2 руб. %3 шт.</font></p>").arg(line.data_[1], line.data_[3], line.data_[7]));
            layout_h->GetSpinBox()->setObjectName(QString("spin_%1_%2").arg(line.data_.first(),  line.data_[3]));
            layout_h->GetSpinBox()->setRange(0, line.data_[7].toInt());
            layout_h->GetSpinBox()->setValue(order_.QuantityInOrder(line.data_[0].toInt()).first);
            if (layout_h->GetSpinBox()->value())
                layout_h->GetSumma()->setText(QString("%1").arg(QString::number(layout_h->GetSpinBox()->value() * line.data_[3].toDouble())));

            connect(layout_h->GetLabel(), &MyLabel::clicked, this, &MainWindow::Label3Clicked);
            connect(layout_h->GetSpinBox(), &QSpinBox::valueChanged, &order_, &Order::AddProduct);
            connect(layout_h->GetSpinBox(), &QSpinBox::valueChanged, this, &MainWindow::PriceChange);

            layoutV->addLayout(layout_h);
        }
        file_product_->close();
    }
    layoutV->addStretch();
}

void MainWindow::ProductWindow(QScrollArea *scroll_area, QString id) {
    auto data = model_.GetLine(id.toInt());
    QPixmap p(QString(beginning_path_ + "/bd/photo_product/%1/1.jpg").arg(data.data_[0]));
    ui->label->setPixmap(p);
    ui->label->setFixedHeight(p.height() * ui->label->width() / p.width());
    ui->label->setScaledContents(true);
    ui->label_19->setText(data.data_[1]);
    ui->label_9->setText(data.data_[3] + " руб.");
    ui->label_12->setText(QString("%2\n\nАртикул - %1").arg(data.data_[5], data.data_[6].replace("<?>", "\n")));
    ui->label_11->setText((data.data_[7]).trimmed() + " шт.");
    ui->spinBox->setRange(0, data.data_[7].toInt());
    ui->spinBox->setObjectName("spin_" + data.data_[0] + "_" + data.data_[3]);
    for (auto &widget: scroll_area->widget()->children()) {
        if (widget->objectName() == ui->spinBox->objectName()) {
            ui->spinBox->setValue(static_cast<QSpinBox*>(widget)->value());
            break;
        }
    }
}

void MainWindow::OrderWindow(QScrollArea *scroll_area, QString file_path) {
    QWidget *scroll_widget = new QWidget;
    QVBoxLayout *layout_v = new QVBoxLayout(scroll_widget);
    scroll_area->setWidget(scroll_widget);
    auto file = QFile(file_path);
    if(file.open(QFile::ReadOnly)) {
        QString line = file.readLine();
        ui->label_17->setText(QString("%L1").arg(line.section(';', 2, 2).trimmed().toDouble()));
        while (!file.atEnd()) {
            line = file.readLine();
            QString id = line.section(';', 0,0);
            QString count = line.section(';', 1,1);
            ProductView *layout_h = new ProductView(id);
            auto data = model_.GetLine(id.toInt());
                layout_h->GetPhoto()->setPixmap(QPixmap(QString(beginning_path_ + "/bd/photo_product/%1/logo.jpg").arg(id)));
            layout_h->GetLabel()->setText(QString("<p align=left><font size=5>%1</font><br/>"
                                                  "<br/><font size=3>%2 руб. %3 шт.</font></p>").arg(data.data_[1], data.data_[3], data.data_[7]));
            layout_h->GetSpinBox()->setRange(0, data.data_[7].toInt());
            layout_h->GetSpinBox()->setValue(count.toInt());
            if (layout_h->GetSpinBox()->value())
                layout_h->GetSumma()->setText(QString("%1").arg(QString::number(layout_h->GetSpinBox()->value() * data.data_[3].toDouble())));
            layout_h->GetSpinBox()->setReadOnly(true);
            connect(layout_h->GetLabel(), &MyLabel::clicked, this, &MainWindow::Label5Clicked);
            layout_v->addLayout(layout_h);
        }
        auto *button_delete = new QPushButton("Удалить");
        button_delete->setMinimumSize(QSize(120, 40));
        button_delete->setStyleSheet("QPushButton { background-color : red; color : black; }");
        connect(button_delete, &QPushButton::clicked, this, &MainWindow::PushButtonClicked);
        layout_v->addWidget(button_delete);
        file.close();
    }
    layout_v->addStretch();
}

void MainWindow::PushButtonClicked() {
    QFile file(data_.back());
    file.remove();
    ui->stackedWidget_2->move(QPoint(-1000, 0));
    on_pushButton_22_clicked();
}

void MainWindow::LabelClicked(QString text) {
    ui->stackedWidget_2->move(QPoint(-1000, 0));
    delete ui->scrollArea->widget();
    order_.SetIdRetailer(text.toInt());
    ui->label_16->setText("Контракты");
    ContractOnWidgit(ui->scrollArea);
    animation_right->start();
 }

void MainWindow::Label1Clicked(QString text) {
    ui->stackedWidget_2->move(QPoint(-1000, 0));
    delete ui->scrollArea->widget();
    data_[0] = text;
    ui->label_16->setText("Категории");
    CategoryOnWidgit(ui->scrollArea);
    animation_right->start();
}

void MainWindow::Label2Clicked(QString text) {
    data_[1] = text;
    ui->stackedWidget_2->setCurrentWidget(ui->page);
    ui->label_15->setText(QString("<p align=center><font size=5>%L1</font>").arg(order_.GetTotalPrice()));
    ProductOnWidgit(ui->scrollArea_3);
    animation_right->start();
}

void MainWindow::Label3Clicked(QString text) {
    ui->stackedWidget_2->setCurrentWidget(ui->page_2);
    ui->spinBox->setReadOnly(false);
    ProductWindow(ui->scrollArea_3, text);
    animation_right->start();
}

void MainWindow::Label4Clicked(QString text) {
    ui->stackedWidget_2->setCurrentWidget(ui->page_6);
    OrderWindow(ui->scrollArea_4, text);
    data_.push_back(text);
    animation_right->start();
}

void MainWindow::Label5Clicked(QString text) {
    ui->stackedWidget_2->setCurrentWidget(ui->page_2);
    ui->spinBox->setReadOnly(true);
    ProductWindow(ui->scrollArea_4, text);
}

void MainWindow::on_pushButton_12_clicked() {
    if (ui->lineEdit_7->text().isEmpty() || ui->lineEdit_9->text().isEmpty()
        || ui->lineEdit_10->text().isEmpty() || data_.empty()) return;
    QStringList data;
    data.push_back(ui->lineEdit_7->text());
    data.push_back(ui->lineEdit_9->text());
    data.push_back(ui->lineEdit_10->text());
    Zip(data_, beginning_path_ + "/bd/send.zip", false);
    data << beginning_path_ + "/bd/send.zip";
    api_.RequestAddRetailer(user_, data);
    api_.reply_->setObjectName("retailer");
    connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseBDAdd);
}

void MainWindow::on_pushButton_19_clicked() {
    if (ui->lineEdit_13->text().isEmpty() || ui->lineEdit_8->text().isEmpty()
        || ui->lineEdit_11->text().isEmpty() || data_.empty() || ui->comboBox_2->isWindowModified()
        || ui->comboBox_2->currentText().isEmpty() || ui->comboBox_3->currentText().isEmpty()) return;
    QStringList data;
    data << ui->lineEdit_13->text().trimmed();
    data << ui->lineEdit_8->text().trimmed();
    data << GetIdBD(QFile(beginning_path_ + "/bd/contract.csv"), ui->comboBox_2->currentText());
    data << ui->lineEdit_11->text().trimmed();
    data << GetIdBD(QFile(beginning_path_ + "/bd/category.csv"), ui->comboBox_3->currentText());
    data << ui->lineEdit_12->text().trimmed();
    data << ui->textEdit->toPlainText().replace('\n', "<?>");
    Zip(data_, beginning_path_ + "/bd/send.zip", false);
    data << beginning_path_ + "/bd/send.zip";
    api_.RequestAddProduct(user_, data);
    api_.reply_->setObjectName("product");
    connect(api_.reply_, &QIODevice::readyRead, this, &MainWindow::ResponseBDAdd);
}

void MainWindow::on_pushButton_14_clicked() {
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString dir = picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last();
    data_.push_front(QFileDialog::getOpenFileName(this, "Select one file to open", dir, "Images (*.jpeg *.jpg *.JPG *.JPEG)"));
}

void MainWindow::on_pushButton_20_clicked() {
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString dir = picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last();
    data_ = QFileDialog::getOpenFileNames(this, "Select one file to open", dir, "Images (*.jpeg *.jpg *.JPG *.JPEG)");
}

QList<QPair<QString, QString>> MainWindow::GetIdName(QFile file) {
    QList<QPair<QString, QString>> result;
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            result << QPair<QString, QString>(line.section(';', 0, 0), line.section(';', 1, 1).trimmed());
        }
        file.close();
    }
    return result;
}

QString MainWindow::GetIdBD(QFile file, QString name) {
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            if (line.section(';', 1, 1).trimmed() == name) {
                return line.section(';', 0, 0);
            }
        }
        file.close();
    }
    return "0";
}

QString MainWindow::GetNameBD(QFile file, QString id) {
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            if (line.section(';', 0, 0).trimmed() == id) {
                return line.section(';', 1, 1);
            }
        }
        file.close();
    }
    return "0";
}

QStringList MainWindow::GetListBD(QFile file) {
    QStringList result;
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            result << line.section(';', 1, 1).trimmed();
        }
        file.close();
    }
    return result;
}

void MainWindow::on_pushButton_24_clicked() {
    QString text = ui->label_16->text();
    if (text == "Категории") {
        delete ui->scrollArea->widget();
        order_.SetIdRetailer(order_.GetIdRetailer());
        ui->label_16->setText("Контракты");
        ContractOnWidgit(ui->scrollArea);
    } else if (text == "Контракты") {
        RetailerOnWidgit(ui->scrollArea);
        ui->label_16->setText("Магазины");
    } else if (text == "Магазины") {
        on_pushButton_21_clicked();
    } else {
        ui->stackedWidget_2->setCurrentWidget(ui->page_3);
    }
    animation_left->start();
}

void MainWindow::on_spinBox_valueChanged(int arg1) {
    ui->label_13->setText(QString::number(ui->label_9->text().section(' ', 0, 0).toDouble() * arg1) + " руб.");
}

void MainWindow::on_pushButton_25_clicked() {
    QString text = ui->label_16->text();
    if (text == "Заказы") {
        ui->stackedWidget_2->setCurrentWidget(ui->page_6);
    } else {
        ui->stackedWidget_2->setCurrentWidget(ui->page);
        for (auto &widget: ui->scrollArea_3->widget()->children()) {
            if (widget->objectName() == ui->spinBox->objectName()) {
                static_cast<QSpinBox*>(widget)->setValue(ui->spinBox->value());
                break;
            }
        }
    }
    animation_left->start();
}

void MainWindow::on_pushButton_26_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->order);
    animation_left->start();
}

void MainWindow::on_pushButton_21_clicked() {
    order_.PlaceOrder();
    ui->stackedWidget_2->setCurrentWidget(ui->page_3);
    animation_left->start();
}

void MainWindow::on_pushButton_22_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->order);
    QStringList directories;
    directories << beginning_path_ + "/bd_local/orders" << "Локально" << "red" << "white" << "local_";
    QStringList separators;
    separators <<  beginning_path_ + "/bd/orders"  << "В базе" << "green" << "white" << "bd_";
    QList<QList<QString>> data;
    data << directories << separators;
    ui->label_16->setText("Заказы");
    data_.clear();
    OrderOnWidgit(ui->scrollArea, data);
//    animation_right->start();
}

void MainWindow::on_pushButton_27_clicked() {
    ui->stackedWidget_2->setCurrentWidget(ui->order);
    animation_left->start();
}

void MainWindow::on_pushButton_23_clicked() {
    QString path = data_.back();
    on_pushButton_clicked();
    order_.AddProductFromFile(path);

    QFile file(path);
    file.remove();
}

void MainWindow::on_pushButton_28_clicked() {
    api_.RequestAddOrder(user_, data_.back());
    connect(api_.reply_, &QNetworkReply::finished, this, &MainWindow::ResponseAddOrder);
}

void MainWindow::ResponseAddOrder() {
    int status_code = api_.reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status_code == 200) {
        QString file_name = QString(api_.reply_->rawHeader("Content-Disposition")).section('"', 1, 1);
        QFile *order = new QFile(beginning_path_ + "/bd/orders/" + file_name);
        if(order->open(QFile::WriteOnly)) {
            order->write(api_.reply_->readAll());
            order->close();
        }
        QFile order_local(beginning_path_ + "/bd_local/orders/" + file_name);
        order_local.remove();
        on_pushButton_22_clicked();
        api_.reply_->deleteLater();
        animation_left->start();
    } else {
        qDebug() << "что то пошло не так";
    }
}

void MainWindow::on_pushButton_29_clicked() {

}

void MainWindow::AnimationLeft() {
    animation_left = new QPropertyAnimation(ui->stackedWidget_2, "pos");
    animation_left->setDuration(500);
    animation_left->setStartValue(QPoint(-400, 0));
    animation_left->setEndValue(QPoint(0, 0));
    animation_left->setEasingCurve(QEasingCurve::OutExpo);
}

void MainWindow::AnimationRight() {
    animation_right = new QPropertyAnimation(ui->stackedWidget_2, "pos");
    animation_right->setDuration(500);
    animation_right->setStartValue(QPoint(400, 0));
    animation_right->setEndValue(QPoint(0, 0));
    animation_right->setEasingCurve(QEasingCurve::OutExpo);
}













































