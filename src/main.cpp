/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "yfel_config.h"

int main(int argc, char *argv[]) {
    QApplication _(argc, argv);

    /*
     * debug message output format
     */
#ifdef QT_NO_DEBUG
    qSetMessagePattern("[%{type}] %{time MM-dd hh:mm:ss} %{message}");
#else
    qSetMessagePattern("[%{type}] %{time MM-dd hh:mm:ss}  %{function}:%{line} %{message}");
#endif

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
        const QString baseName = "YFEL_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.setWindowTitle(PROJECT_NAME + QString(" - ") + PROJECT_DESCRIPTION);
    w.setWindowIcon(QIcon(":/assets/img/icon.png"));
    w.show();

    return QApplication::exec();
}
