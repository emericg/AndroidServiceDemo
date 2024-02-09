TARGET  = AndroidServiceDemo

VERSION = 0.1
DEFINES+= APP_NAME=\\\"$$TARGET\\\"
DEFINES+= APP_VERSION=\\\"$$VERSION\\\"

CONFIG += c++17
QT     += core gui qml quick quickcontrols2

# Project modules ##############################################################

# Various utils
include(src/thirdparty/AppUtils/AppUtils.pri)

# Project files ################################################################

SOURCES  += src/main.cpp \
            src/NotificationManager.cpp

HEADERS  += src/NotificationManager.h

INCLUDEPATH += src/ src/thirdparty/

RESOURCES   += qml/qml.qrc

# Build settings ###############################################################

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(release, debug|release) : DEFINES += NDEBUG QT_NO_DEBUG QT_NO_DEBUG_OUTPUT

# Build artifacts ##############################################################

OBJECTS_DIR = build/$${QT_ARCH}/
MOC_DIR     = build/$${QT_ARCH}/
RCC_DIR     = build/$${QT_ARCH}/
UI_DIR      = build/$${QT_ARCH}/

DESTDIR     = bin/

# Application deployment steps #################################################

android {
    # ANDROID_TARGET_ARCH: [x86_64, armeabi-v7a, arm64-v8a]
    message("ANDROID_TARGET_ARCH: $$ANDROID_TARGET_ARCH")

    # Bundle name
    QMAKE_TARGET_BUNDLE_PREFIX = io.emeric
    QMAKE_BUNDLE = androidservicedemo

    SOURCES  += $${PWD}/src/AndroidService.cpp
    HEADERS  += $${PWD}/src/AndroidService.h

    OTHER_FILES += $${PWD}/assets/android/src/io/emeric/androidservicedemo/BootServiceBroadcastReceiver.java \
                   $${PWD}/assets/android/src/io/emeric/androidservicedemo/AndroidService.java \
                   $${PWD}/assets/android/src/io/emeric/androidservicedemo/AndroidNotifier.java \
                   $${PWD}/assets/android/src/io/emeric/utils/QGpsUtils.java

    DISTFILES += $${PWD}/assets/android/AndroidManifest.xml \
                 $${PWD}/assets/android/gradle.properties \
                 $${PWD}/assets/android/build.gradle

    ANDROID_PACKAGE_SOURCE_DIR = $${PWD}/assets/android
}
