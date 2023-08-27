#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QBuffer>
#include <QString>
#include <stdio.h>

void UnZip(QString archive, QString directories);
void Zip(QStringList files_path, QString archive_path, bool save_name);
QByteArray GetLogo(QString path);
#endif // ASSISTANT_H
