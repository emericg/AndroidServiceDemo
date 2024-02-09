/*!
 * This file is part of AndroidServiceDemo.
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

#include "AndroidService.h"

#if defined(Q_OS_ANDROID)


#include "NotificationManager.h"
#include "utils_log.h"

#include <QtCore/private/qandroidextras_p.h>
#include <QCoreApplication>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

/* ************************************************************************** */

AndroidService::AndroidService(QObject *parent) : QObject(parent)
{
    m_notificationManager = NotificationManager::getInstance(); // DEBUG
    m_notificationManager->setNotification("AndroidService starting", QDateTime::currentDateTime().toString());

    qDebug() << "AndroidService::AndroidServiceAndroidService::AndroidServiceAndroidService::AndroidServiceAndroidService::AndroidService";

    // Configure update timer
    connect(&m_workTimer, &QTimer::timeout, this, &AndroidService::gotowork);
    setWorkTimer(5);
}

AndroidService::~AndroidService()
{
    //
}

/* ************************************************************************** */

void AndroidService::setWorkTimer(int workInterval_mins)
{
    m_workTimer.setInterval(workInterval_mins*60*1000);
    m_workTimer.start();
}

void AndroidService::gotowork()
{
    m_notificationManager = NotificationManager::getInstance(); // DEBUG
    m_notificationManager->setNotification("AndroidService gotowork", QDateTime::currentDateTime().toString());

    UtilsLog *utilsLog = UtilsLog::getInstance();
    utilsLog->pushLog("(daemon) gotowork()");

    //

    // Restart timer
    setWorkTimer(5);
}

/* ************************************************************************** */

void AndroidService::service_start()
{
    QJniObject::callStaticMethod<void>("io.emeric.androidservicedemo.AndroidService",
                                       "serviceStart",
                                       "(Landroid/content/Context;)V",
                                       QNativeInterface::QAndroidApplication::context());
}

void AndroidService::service_stop()
{
    QJniObject::callStaticMethod<void>("io.emeric.androidservicedemo.AndroidService",
                                       "serviceStop", "(Landroid/content/Context;)V",
                                       QNativeInterface::QAndroidApplication::context());
}

void AndroidService::service_registerCommService()
{
    QJniEnvironment env;
    jclass javaClass = env.findClass("io/emeric/androidservicedemo/ActivityUtils");
    QJniObject classObject(javaClass);

    classObject.callMethod<void>("registerServiceBroadcastReceiver",
                                 "(Landroid/content/Context;)V",
                                 QNativeInterface::QAndroidApplication::context());
}

/* ************************************************************************** */
#endif // Q_OS_ANDROID
