#ifndef CONFIG_H
#define CONFIG_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>

#define WAYCRATE_LOCK_DEF_CONF_PATH "assets/config/setting.toml"

constexpr static const char *CONFIG_FILE = "setting.toml";
constexpr static const char *CONFIGDIR   = "waycratelock";

static QString
get_config_path()
{
    return QString("%1/%2/%3")
      .arg(
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation), CONFIGDIR, CONFIG_FILE);
}

inline void
ensureConfigFile()
{
    QString configPath = get_config_path();

    if (!QFile::exists(configPath)) {
        qDebug() << "-- Config file not found, creating default:" << configPath;
        QDir().mkpath(QFileInfo(configPath).absolutePath());

        QFile defaultConfigFile(WAYCRATE_LOCK_DEF_CONF_PATH);
        if (!defaultConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical() << "Error: Default config file missing at" << WAYCRATE_LOCK_DEF_CONF_PATH;
            return;
        }

        QFile configFile(configPath);
        if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&configFile);
            out << QTextStream(&defaultConfigFile).readAll();
            configFile.close();
        } else {
            qCritical() << "Error: Failed to create" << configPath;
        }
    }
}

#endif
