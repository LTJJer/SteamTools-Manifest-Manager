# to-do:
# 用正则匹配格式化时 lua 里的 addappid
# 主界面拖拽导入



# 项目配置
TARGET = "SteamTools Manifest Manager"

TEMPLATE = app



CONFIG += c++23



QT += widgets



# 源文件
HEADERS += \
    mainwindow.hpp \
    functionlib.hpp \
    editdialog.hpp \
    addluafiledialog.hpp \
    aboutdialog.hpp \
    openwebsitedialog.hpp \
    searchdialog.hpp

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    functionlib.cpp \
    editdialog.cpp \
    addluafiledialog.cpp \
    aboutdialog.cpp \
    openwebsitedialog.cpp \
    searchdialog.cpp

FORMS += \
    mainwindow.ui \
    editdialog.ui \
    addluafiledialog.ui \
    aboutdialog.ui \
    openwebsitedialog.ui \
    searchdialog.ui

RESOURCES += \
    resources.qrc



# 应用信息
RC_ICONS = ./Resources/Icons/icon.ico

VERSION = 1.5.2
MAKE_TARGET_COMPANY = "LT_JJ"
QMAKE_TARGET_DESCRIPTION = "An application for manage SteamTools manifest (Lua) files."
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2025 LT_JJ. Licensed under MIT."



CONFIG(debug, debug|release) {
    BUILD_TYPE = "Debug"
} else {
    BUILD_TYPE = "Release"
}

DESTDIR = $${PWD}/bin/$${BUILD_TYPE}



# 部署
win32
{
    BUILD_EXE = $$replace(DESTDIR, /, \\)\\$${EXE_PATH}

    EXE_PATH = $${TARGET}.exe

    DEPLOY_ROOT = D:\\$${TARGET}
    DEPLOY_EXE = $${DEPLOY_ROOT}\\$${EXE_PATH}

    WINDEPLOYQT = $$[QT_INSTALL_LIBEXECS]/windeployqt.exe



    # 1: $${WINDEPLOYQT}
    # 2: $${BUILD_EXE}
    # 3: $${DEPLOY_ROOT}
    # 4: $${DEPLOY_EXE}

    deploy.commands = $${PWD}/deploy.bat \
                    \"$${WINDEPLOYQT}\"  \
                    \"$${BUILD_EXE}\"    \
                    \"$${DEPLOY_ROOT}\"  \
                    \"$${DEPLOY_EXE}\"



    INSTALLS += deploy
}
