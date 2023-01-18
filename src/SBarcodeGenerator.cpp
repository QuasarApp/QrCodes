#include <QStandardPaths>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include "SBarcodeGenerator.h"
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

SBarcodeGenerator::SBarcodeGenerator() {
    _exportDir = QStandardPaths::writableLocation(
                QStandardPaths::DocumentsLocation);
}

bool SBarcodeGenerator::process(const QString &inputString)
{
    if (inputString.isEmpty()){
        return false;
    }
    else {
    ZXing::MultiFormatWriter writer = ZXing::MultiFormatWriter(_format).setMargin(_margin).setEccLevel(_eccLevel);

    try {
        _bitmap = ZXing::ToMatrix<uint8_t>(writer.encode(ZXing::TextUtfEncoding::FromUtf8(inputString.toStdString()), _width, _height));
    } catch (const std::exception& err) {
        qWarning() << err.what();
        return false;
    }

    _filePath = QDir::tempPath() + "/" + _fileName + "." + _extension;
    emit filePathChanged(_filePath);

    if (_extension == "png") {
        stbi_write_png(_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 1, _bitmap.data(), 0);
    }
    else if (_extension == "jpg" || _extension == "jpeg") {
        stbi_write_jpg(_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 1, _bitmap.data(), 0);
    }

    emit processFinished();

    return true;
    }
}



bool SBarcodeGenerator::saveImage()
{
    if (_filePath.isEmpty()) {
        return false;
    }

#ifdef Q_OS_ANDROID
    if (QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")) == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.WRITE_EXTERNAL_STORAGE"}));
        if (resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }
#endif

    _exportFilePath = _exportDir + "/" + _fileName + "." + _extension;
    emit exportFilePathChanged(_exportFilePath);

    if (QFileInfo::exists(_exportFilePath)) {
        QFile::remove(_exportFilePath);
    } else {
        QDir().mkpath(QFileInfo(_exportFilePath).absolutePath());
    }

    return QFile::copy(_filePath, _exportFilePath);
}

void SBarcodeGenerator::barcodeFormatFromQMLString(const QString &format)
{
    _format = ZXing::BarcodeFormatFromString(format.toStdString());
}
