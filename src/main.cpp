/*!
 * Copyright (c) 2024 Emeric Grange - All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \date      2024
 * \author    Emeric Grange <emeric.grange@gmail.com>
 */

#include "NotificationManager.h"

#include "utils_app.h"
#include "utils_screen.h"
#include "utils_log.h"

#include <QtGlobal>
#include <QLibraryInfo>
#include <QVersionNumber>

#include <QCoreApplication>
#include <QGuiApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSurfaceFormat>

#if defined(Q_OS_ANDROID)
#include "AndroidService.h"
#include "private/qandroidextras_p.h" // for QAndroidService
#endif

/* ************************************************************************** */

int main(int argc, char *argv[])
{
    // Arguments parsing ///////////////////////////////////////////////////////

    bool background_service = false;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i])
        {
            //qDebug() << "> arg >" << argv[i];

            if (QString::fromLocal8Bit(argv[i]) == "--service")
                background_service = true;
        }
    }

    // Background service application //////////////////////////////////////////

    // Android daemon
    if (background_service)
    {
#if defined(Q_OS_ANDROID)
        QAndroidService app(argc, argv);
        app.setApplicationName("AndroidServiceDemo");
        app.setOrganizationName("emeric");
        app.setOrganizationDomain("emeric");

        {
            AndroidService *as = new AndroidService();
            if (!as) return EXIT_FAILURE;

            return app.exec();
        }

        return EXIT_SUCCESS;
#endif
    }

    // GUI application /////////////////////////////////////////////////////////

    QGuiApplication app(argc, argv, true);

    // Application name
    app.setApplicationName("AndroidServiceDemo");
    app.setApplicationDisplayName("AndroidServiceDemo");
    app.setOrganizationName("emeric");
    app.setOrganizationDomain("emeric");

    // Init components
    NotificationManager *nm = NotificationManager::getInstance();
    if (!nm)
    {
        qWarning() << "Cannot init AndroidServiceDemo components!";
        return EXIT_FAILURE;
    }

    // Init generic utils
    UtilsApp *utilsApp = UtilsApp::getInstance();
    UtilsScreen *utilsScreen = UtilsScreen::getInstance();
    if (!utilsScreen || !utilsApp)
    {
        qWarning() << "Cannot init AndroidServiceDemo utils!";
        return EXIT_FAILURE;
    }

    UtilsLog *utilsLog = UtilsLog::getInstance(false);
    utilsLog->pushLog("(app) starting");

    // Then we start the UI
    QQmlApplicationEngine engine;
    QQmlContext *engine_context = engine.rootContext();

    engine_context->setContextProperty("notificationManager", nm);
    engine_context->setContextProperty("utilsApp", utilsApp);
    engine_context->setContextProperty("utilsScreen", utilsScreen);
    engine_context->setContextProperty("utilsLog", utilsLog);

    // Load the main view
    engine.load(QUrl(QStringLiteral("qrc:/qml/MobileApplication.qml")));

    if (engine.rootObjects().isEmpty())
    {
        qWarning() << "Cannot init QmlApplicationEngine!";
        return EXIT_FAILURE;
    }

#if defined(Q_OS_ANDROID)
    QNativeInterface::QAndroidApplication::hideSplashScreen(333);
    AndroidService::service_start();
#endif

    return app.exec();
}

/* ************************************************************************** */
