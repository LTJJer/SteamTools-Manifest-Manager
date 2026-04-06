#pragma once

#include "functionlib.hpp"
#include "lua.hpp"

#include <QDialog>
#include <qevent.h>
#include <QMimeData>



namespace Ui { class AddLuaFileDialog; }



class AddLuaFileDialog : public QDialog
{
    Q_OBJECT

signals:
    void addingFinished(FunctionLib::FileEditErrorType error, const Lua::LuaData &data);

public:
    explicit AddLuaFileDialog(const QString &luaDir, QWidget *parent = nullptr);
    ~AddLuaFileDialog();

    // QWidget interface
private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void import(const QString &path, bool append = false);

private:
    Ui::AddLuaFileDialog *ui;

    const QString mv_luaDir;

private slots:
    void on_importButton_clicked();

    void on_CancelButton_clicked();

    void on_OKButton_clicked();
};
