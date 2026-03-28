# to-do:
# 用正则匹配查找 lua 信息
# 编辑支持直接编辑
# 支持从开源仓库导入 Lua



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

OTHER_FILES += \
    .gitignore \
    deploy.bat \
    LICENSE.txt \
    README.md



# 应用信息
RC_ICONS = ./Resources/Icons/icon.ico

VERSION = 1.6.2
MAKE_TARGET_COMPANY = "LT_JJ"
QMAKE_TARGET_DESCRIPTION = "An application for manage SteamTools manifest (Lua) files."
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2025-2026 LT_JJ. Licensed under MIT."



# 环境配置
CONFIG(debug, debug|release) {
    BUILD_TYPE = "Debug"
} else {
    BUILD_TYPE = "Release"
}

DESTDIR = $${PWD}/bin/$${BUILD_TYPE}



# 部署
win32
{
    EXE_NAME = $${TARGET}.exe
    BUILD_EXE = $${DESTDIR}/$${EXE_NAME}

    DEPLOY_EXE = D:/$${TARGET}/$${EXE_NAME}

    WINDEPLOYQT = $$[QT_INSTALL_LIBEXECS]/windeployqt.exe

    BUILD_LICENSE = $${PWD}/LICENSE.txt
    BUILD_README = $${PWD}/README.md



    # 1: $${WINDEPLOYQT}
    # 2: $${BUILD_EXE}
    # 3: $${DEPLOY_EXE}
    # 4: $${BUILD_LICENSE}
    # 5: $${BUILD_README}

    deploy.commands = $${PWD}/deploy.bat                 \
                    \"$$system_path($${WINDEPLOYQT})\"   \
                    \"$$system_path($${BUILD_EXE})\"     \
                    \"$$system_path($${DEPLOY_EXE})\"    \
                    \"$$system_path($${BUILD_LICENSE})\" \
                    \"$$system_path($${BUILD_README})\"  \
    
    QMAKE_EXTRA_TARGETS += deploy
}
