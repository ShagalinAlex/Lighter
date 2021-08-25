QT += quick serialport network
CONFIG += c++11

#QMAKE_LFLAGS_RELEASE += -static -static-libgcc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adapterbase.cpp \
    bme280adapter.cpp \
    buttonadapter.cpp \
    compressoradapter.cpp \
    devicesettings.cpp \
    espdeviceinformation.cpp \
    gateadapter.cpp \
    main.cpp \
    mqttsettings.cpp \
    myconicalgradient.cpp \
    rgbadapter.cpp \
    serialadapter.cpp \
    udpfinderclient.cpp \
    avaservice.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    adapterbase.h \
    bme280adapter.h \
    buttonadapter.h \
    compressoradapter.h \
    devicesettings.h \
    espdeviceinformation.h \
    gateadapter.h \
    message.h \
    mqttsettings.h \
    myconicalgradient.h \
    rgbdapter.h \
    serialadapter.h \
    udpfinderclient.h \
    avaservice.h

DISTFILES += \
    GateInterface.qml \
    ONOFFInterface.qml \
    RGBInterface.qml \
    SettingsInterface.qml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    main.qml \
    RemoteDeviceInterface.qml \
    CustomButton.qml \
    AvaInterface.qml \
    SettingsSerial.qml

    android {
        QMAKE_LFLAGS += -nostdlib++
    }

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

