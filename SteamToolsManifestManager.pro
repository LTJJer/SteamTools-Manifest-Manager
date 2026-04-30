# to-do:
# 使编辑可以直接编辑
# 添加自动更新功能
# 添加窗口导入支持导入压缩包
# 支持从开源仓库导入 Lua



# 项目配置
TARGET = "SteamTools Manifest Manager"

TEMPLATE = app



CONFIG += c++23



QT += widgets



# 源文件
HEADERS += \
    constant.hpp \
    lua.hpp \
    mainwindow.hpp \
    functionlib.hpp \
    editdialog.hpp \
    addluafiledialog.hpp \
    aboutdialog.hpp \
    openwebsitedialog.hpp \
    searchdialog.hpp \
    settingsdialog.hpp

SOURCES += \
    constant.cpp \
    lua.cpp \
    main.cpp \
    mainwindow.cpp \
    functionlib.cpp \
    editdialog.cpp \
    addluafiledialog.cpp \
    aboutdialog.cpp \
    openwebsitedialog.cpp \
    searchdialog.cpp \
    settingsdialog.cpp

FORMS += \
    mainwindow.ui \
    editdialog.ui \
    addluafiledialog.ui \
    aboutdialog.ui \
    openwebsitedialog.ui \
    searchdialog.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    .gitignore \
    LICENSE.txt \
    README.md \
    index.html \
    _base_theme.scss \
    DarkThemeStyleSheet.scss \
    LightThemeStyleSheet.scss \
    SCSSCompilation.bat



# 应用信息
RC_ICONS = Resources/Icons/icon.ico

VERSION = 1.7.4
MAKE_TARGET_COMPANY = "LT_JJ"
QMAKE_TARGET_DESCRIPTION = $${TARGET}
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



    # Args: ^<BUILD_EXE^> ^<DEPLOY_EXE^> ^<BUILD_README^> ^<BUILD_LICENSE^> ^<WINDEPLOYQT^> ^<DEPLOY_ARGS^> ^<DO_ARCHIVE^>

    deploy.commands = E:/Qt/Tools/deploy.bat             \
                    \"$$system_path($${BUILD_EXE})\"     \
                    \"$$system_path($${DEPLOY_EXE})\"    \
                    \"$$system_path($${BUILD_README})\"  \
                    \"$$system_path($${BUILD_LICENSE})\" \
                    \"$$system_path($${WINDEPLOYQT})\"   \
                    \"--no-translations --no-system-d3d-compiler --no-opengl-sw -no-svg -no-network --skip-plugin-types imageformats\" \
                    \"1\"

    QMAKE_EXTRA_TARGETS += deploy
}
