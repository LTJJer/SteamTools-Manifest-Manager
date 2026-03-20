#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include "functionlib.hpp"
#include "editdialog.hpp"
#include "addluafiledialog.hpp"
#include "searchdialog.hpp"
#include "aboutdialog.hpp"
#include "openwebsitedialog.hpp"

#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDir>
#include <QDirIterator>
#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QClipboard>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->steamExe = QDir::fromNativeSeparators(QSettings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat).value("SteamExe").toString());
    this->LuaDir = QDir::fromNativeSeparators(QSettings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat).value("SteamPath").toString() + "/config/stplug-in");

    if (this->steamExe.isEmpty() || !QFile::exists(this->steamExe) || this->LuaDir.isEmpty() || !QDir().mkpath(this->LuaDir))
    {
        QMessageBox::critical(this, "启动失败", "未找到有效 Steam 路径。");
        QTimer::singleShot(0, this, &MainWindow::close);
    }

    this->refresh();

    this->on_lst_Items_currentItemChanged(nullptr, nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::refresh()
{
    QListWidgetItem *lastSelectedItem = ui->lst_Items->currentItem();
    bool hasLastSelectedItem = lastSelectedItem;
    QString lastSelectedAppID;
    if (hasLastSelectedItem) lastSelectedAppID = lastSelectedItem->data(itemDataGameAppID).toString();

    clearList();


    QDirIterator it(this->LuaDir, { QString("*.%1").arg(enabledSuffix), QString("*.%1").arg(disabledSuffix) });

    while (it.hasNext())
    {
        const QString filePath = it.next();
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            continue;
        }

        FunctionLib::LuaInfo luaInfo = FunctionLib::detectLuaInfo(QString::fromUtf8(file.readAll()), "", QFileInfo(filePath).baseName());
        file.close();

        this->addItem(filePath, luaInfo.name, luaInfo.appID, hasLastSelectedItem && (luaInfo.appID == lastSelectedAppID), false);
    }

    ui->lst_Items->sortItems(Qt::AscendingOrder);

    emit this->refreshed();
}

void MainWindow::editItem(QListWidgetItem *item)
{
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }

    EditDialog editDialog(item->data(itemDataFilePath).toString(), item->data(itemDataGameName).toString(), item->data(itemDataGameAppID).toString(), this);

    connect(&editDialog, &EditDialog::editFinished, this,
            [item, widget = ui->lst_Items->itemWidget(item)](FunctionLib::FileEditErrorType error, const QString &path, const QString &name, const QString &appid)
            {
                if (!(error & FunctionLib::NewNameExisted) && !(error & FunctionLib::RenameFailed))
                {
                    item->setData(itemDataFilePath,  path);
                }
                if (!(error & FunctionLib::OpenFileFailed))
                {
                    item->setData(itemDataGameName,  name);
                    item->setData(itemDataGameAppID, appid);

                    if (widget)
                    {
                        QLabel *lbl_Appid = widget->findChild<QLabel*>("lbl_Appid");
                        if (lbl_Appid)
                        {
                            lbl_Appid->setText(appid);
                        }

                        QLabel *lbl_Name = widget->findChild<QLabel*>("lbl_Name");
                        if (lbl_Name)
                        {
                            lbl_Name->setText(name);
                        }
                    }
                }
            });

    editDialog.exec();
}

void MainWindow::filterItems()
{
    // 数据准备
    const QString content = ui->le_Filter->text();

    bool isSearchAppID = ui->rdo_AppID->isChecked();
    bool isCaseSensitive = ui->chk_CaseSensitive->isChecked();
    bool isUseRegex = ui->chk_UseRegularExpression->isChecked();
    bool doesContentValid = true;

    static QRegularExpression contentPattern;

    if (isUseRegex)
    {
        contentPattern.setPattern(content);
        contentPattern.setPatternOptions(isCaseSensitive
                                             ? QRegularExpression::NoPatternOption
                                             : QRegularExpression::CaseInsensitiveOption);

        doesContentValid = contentPattern.isValid();
    }

    ui->le_Filter->setStyleSheet(doesContentValid ? R"(color: #E0E0E0)" : R"(color: #E04040)");
    ui->le_Filter->setToolTip(doesContentValid ? "" : contentPattern.errorString());


    // 遍历
    for (int i = ui->lst_Items->count() - 1; 0 <= i; --i)
    {
        QListWidgetItem *item = ui->lst_Items->item(i);
        if (!item) continue;

        bool match = true;

        if (!content.isEmpty() && doesContentValid)
        {
            const QString target = (isSearchAppID ? item->data(itemDataGameAppID) : item->data(itemDataGameName)).toString();

            if (isUseRegex)
            {
                match = target.contains(contentPattern);
            }
            else
            {
                match = target.contains(content, (isCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive));
            }
        }

        item->setHidden(!match);
    }
}

void MainWindow::runItem(QListWidgetItem *item)
{
    if (!item)
    {
        QMessageBox::warning(this, "运行游戏失败", "未选中任何项");
        return;
    }

    FunctionLib::steamRunGame(item->data(itemDataGameAppID).toString());
}



void MainWindow::clearList()
{
    for (int i = ui->lst_Items->count() - 1; i >= 0; --i)
    {
        delete ui->lst_Items->itemWidget(ui->lst_Items->item(i));
        delete ui->lst_Items->takeItem(i);
    }
}

void MainWindow::on_lst_Items_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);

    bool isEnabled = current;

    ui->btn_Edit->setEnabled(isEnabled);
    ui->btn_DeleteSelectedItem->setEnabled(isEnabled);
    ui->btn_Run->setEnabled(isEnabled);
    ui->btn_OpenShop->setEnabled(isEnabled);
    ui->btn_ToggleLuaEnabled->setEnabled(isEnabled);
    ui->btn_OpenFile->setEnabled(isEnabled);
    ui->btn_CopyAppID->setEnabled(isEnabled);
    ui->btn_CopyGameName->setEnabled(isEnabled);
    ui->btn_CopyInfo->setEnabled(isEnabled);
    ui->btn_CopyFileContent->setEnabled(isEnabled);
    // ui->btn_->setEnabled(isEnabled);
}

void MainWindow::on_btn_AddLuaFile_clicked()
{
    AddLuaFileDialog addDialog(this->LuaDir, this);

    connect(&addDialog, &AddLuaFileDialog::addingFinished, this,
            [this](const QString &filePath, const QString &gameName, const QString &appID)
            {
                this->addItem(filePath, gameName, appID);
                this->refresh();
            });

    addDialog.exec();
}

void MainWindow::on_btn_ToggleLuaEnabled_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }


    QFileInfo fileInfo(item->data(itemDataFilePath).toString());
    QString newSuffix = (fileInfo.suffix() == enabledSuffix ? disabledSuffix : enabledSuffix);
    QFile file(fileInfo.filePath());

    FunctionLib::FileEditErrorType renameError = FunctionLib::renameFile(&file, fileInfo.baseName() + "." + newSuffix);
    if (renameError)
    {
        QMessageBox::critical(this, "重命名失败", FunctionLib::generateFileEditErrorString(renameError));
        return;
    }

    item->setData(itemDataFilePath, file.fileName());

    QWidget *widget = ui->lst_Items->itemWidget(item);
    if (!widget)
    {
        return;
    }

    QLabel *lbl_Appid = widget->findChild<QLabel*>("lbl_Appid");
    if (!lbl_Appid)
    {
        return;
    }

    lbl_Appid->setStyleSheet(newSuffix == enabledSuffix ? "" : disabledItemStyleSheet);
}

void MainWindow::on_btn_About_clicked()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::on_btn_Edit_clicked()
{
    editItem(ui->lst_Items->currentItem());
}

void MainWindow::on_btn_DeleteSelectedItem_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }

    QFile file(item->data(itemDataFilePath).toString());
    if (!file.exists()) return;

    if (QMessageBox::question(this, "删除选中项", "确定删除？") == QMessageBox::No) return;

    bool isSucceeded = file.remove();

    if (isSucceeded)
    {
        this->refresh();
    }
    else
    {
        QMessageBox::critical(this, "删除失败", "错误信息：" + file.errorString());
    }
}

void MainWindow::on_btn_CopyFileContent_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }


    QFile file(item->data(itemDataFilePath).toString());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "失败", "读取失败");
        return;
    }

    QApplication::clipboard()->setText(QString::fromUtf8(file.readAll()));
}

void MainWindow::on_btn_CopyAppID_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }


    QApplication::clipboard()->setText(ui->lst_Items->currentItem()->data(itemDataGameAppID).toString());
}

void MainWindow::on_btn_CopyGameName_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }


    QApplication::clipboard()->setText(ui->lst_Items->currentItem()->data(itemDataGameName).toString());
}

void MainWindow::on_btn_CopyInfo_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }


    QApplication::clipboard()->setText(
        FunctionLib::LuaInfo::toString(
            item->data(itemDataGameName).toString(),
            item->data(itemDataGameAppID).toString()
        )
    );
}

void MainWindow::on_btn_Run_clicked()
{
    runItem(ui->lst_Items->currentItem());
}

void MainWindow::on_btn_OpenShop_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }

    FunctionLib::steamOpenShop(item->data(itemDataGameAppID).toString());
}

void MainWindow::on_btn_FormatAll_clicked()
{
    bool shouldUpdateFileName = QMessageBox::question(this, "全部格式化", "是否要更新文件名？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes;

    int count = ui->lst_Items->count();

    while (--count >= 0)
    {
        QListWidgetItem *item = ui->lst_Items->item(count);

        if (!item) continue;

        FunctionLib::editLuaFile(item->data(itemDataFilePath).toString(), item->data(itemDataGameName).toString(), item->data(itemDataGameAppID).toString(), shouldUpdateFileName);
    }

    if (shouldUpdateFileName) this->refresh();
}

void MainWindow::on_btn_OpenFile_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();
    if (!item)
    {
        QMessageBox::warning(this, "失败", "未选中任何项");
        return;
    }


    QDesktopServices::openUrl(QUrl::fromLocalFile(item->data(itemDataFilePath).toString()));
}

void MainWindow::on_btn_OpenPath_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();

    if (!item)
    {
        FunctionLib::explorerOpenPath(this->LuaDir);
    }
    else
    {
        FunctionLib::explorerSelectPath(item->data(itemDataFilePath).toString());
    }
}

void MainWindow::on_btn_Search_clicked()
{
    QListWidgetItem *item = ui->lst_Items->currentItem();

    SearchDialog searchDialog(item ? item->data(itemDataGameName).toString() : "", this);
    searchDialog.exec();
}

void MainWindow::on_btn_CloseSteam_clicked()
{
    FunctionLib::closeSteamDetached();
}

void MainWindow::on_btn_RestartSteam_clicked()
{
    FunctionLib::restartSteam(this->steamExe);
}

void MainWindow::on_btn_Website_clicked()
{
    OpenWebsiteDialog dlg(this);
    dlg.exec();
}



void MainWindow::addItem(const QString &path, const QString &name, const QString &appID, bool select, bool sort)
{
    QListWidgetItem *item = new QListWidgetItem(name, ui->lst_Items);
    item->setSizeHint(QSize(0, 30));

    item->setData(itemDataFilePath,  path);
    item->setData(itemDataGameName,  name);
    item->setData(itemDataGameAppID, appID);

    QWidget *widget = new QWidget(ui->lst_Items);

    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(9, 9, 9, 9);

    QLabel *lbl_Appid = new QLabel(appID, widget);
    lbl_Appid->setObjectName("lbl_Appid");
    hLayout->addWidget(lbl_Appid, 1);
    if (QFileInfo(path).suffix() == disabledSuffix)
    {
        lbl_Appid->setStyleSheet(disabledItemStyleSheet);
    }

    QLabel *lbl_Name = new QLabel(name, widget);
    lbl_Name->setObjectName("lbl_Name");
    hLayout->addWidget(lbl_Name, 4);

    ui->lst_Items->setItemWidget(item, widget);

    if (select) ui->lst_Items->setCurrentItem(item);
    if (sort) ui->lst_Items->sortItems();
}
