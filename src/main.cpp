#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "yfel_config.h"

int main(int argc, char *argv[]) {
    QApplication _(argc, argv);

    qSetMessagePattern("[%{type}] %{time MM-dd hh:mm:ss}  %{function}:%{line} %{message}");

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
