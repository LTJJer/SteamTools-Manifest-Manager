#pragma once

#include "functionlib.hpp"

#include <QDialog>



namespace Ui { class EditDialog; }



class EditDialog : public QDialog
{
    Q_OBJECT

signals:
    // Args are all the Final values
    void editFinished(FunctionLib::FileEditErrorType error, const QString &path, const QString &name, const QString &appid);

public:
    explicit EditDialog(const QString &filePath, const QString &gameName, const QString &gameAppid, QWidget *parent = nullptr);
    ~EditDialog();

private slots:
    void on_CancelButton_clicked();

    void on_OKButton_clicked();

private:
    Ui::EditDialog *ui;

    QString path, appid, name;
};
