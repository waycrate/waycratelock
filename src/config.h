#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>
#include <iostream>

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
        std::cout << "-- Config file not found, creating default: " << configPath.toStdString()
                  << std::endl;
        QDir().mkpath(QFileInfo(configPath).absolutePath());

        QFile defaultConfigFile(WAYCRATE_LOCK_DEF_CONF_PATH);
        if (!defaultConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Error: Default config file missing at " << WAYCRATE_LOCK_DEF_CONF_PATH
                      << std::endl;
            return;
        }

        QFile configFile(configPath);
        if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&configFile);
            out << QTextStream(&defaultConfigFile).readAll();
            configFile.close();
        } else {
            std::cerr << "Error: Failed to create " << configPath.toStdString() << std::endl;
        }
    }
}

#endif
