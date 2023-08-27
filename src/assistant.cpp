#include "assistant.h"
#include "qzipreader_p.h"
#include "qzipwriter_p.h"
#include <qpixmap.h>

void UnZip(QString archive, QString directories) {
  QZipReader zip_reader(QLatin1String(archive.toLatin1()));
  if (zip_reader.exists()) {
    zip_reader.extractAll(QLatin1String(directories.toLatin1()));
  }
}

void Zip(QStringList files_path, QString archive_path, bool save_name) {
  QZipWriter *writer = new QZipWriter(archive_path);
  int i = 1;
  for (auto path : files_path) {
    QString file_name =
        save_name ? path.split('/').back() : QString::number(i++) + ".jpg";
    QFile file(path);
    if (!file.exists())
      continue;
    file.open(QIODevice::ReadOnly);
    writer->addFile(file_name, file.readAll());
    file.close();
  }
  writer->addFile(save_name ? "logo_" + files_path.first().split('/').back()
                            : "logo.jpg",
                  GetLogo(files_path.first()));
  writer->close();
}

QByteArray GetLogo(QString path) {
  QPixmap pixmap(path);
  pixmap =
      pixmap.scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  QByteArray bArray;
  QBuffer buffer(&bArray);
  buffer.open(QIODevice::WriteOnly);
  pixmap.save(&buffer, "PNG");
  return bArray;
}
