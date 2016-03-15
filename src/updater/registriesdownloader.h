#ifndef REGISTRIESDOWNLOADER_H
#define REGISTRIESDOWNLOADER_H

#include "src/json.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <QJsonDocument>
#include <QJsonObject>

namespace SoftwareRegistry
{
    class Manager
    {
        public:
            Manager();
            QJsonObject getStackSoftwareRegistry();
            //QJsonObject getPhpSoftwareRegistry();
        private:
            void download();
            void downloadRegistry(QUrl url, QString file);
            bool fileNotExistingOrOutdated(QString fileName);
        protected:
            QJsonDocument stackSoftwareRegistry;
            //QJsonDocument phpSoftwareRegistry;
    };

}

#endif // REGISTRIESDOWNLOADER_H
