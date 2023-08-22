#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <stdio.h>
#include <QString>
#include <QBuffer>

void UnZip(QString archive, QString directories);
void Zip(QStringList files_path, QString archive_path, bool save_name);
QByteArray GetLogo(QString path);
#endif // ASSISTANT_H
