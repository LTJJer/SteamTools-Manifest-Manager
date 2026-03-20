#pragma once

#include <QDialog>
#include <qevent.h>
#include <QMimeData>



namespace Ui { class AddLuaFileDialog; }



class AddLuaFileDialog : public QDialog
{
    Q_OBJECT

signals:
    void addingFinished(const QString &filePath, const QString &gameName, const QString &appID, const QStringList &content);

public:
    explicit AddLuaFileDialog(const QString &luaDir, QWidget *parent = nullptr);
    ~AddLuaFileDialog();

    // QWidget interface
private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    // Returning empty indicats invalidity.
    QString getFilePath(const QMimeData *mime) const;

    bool checkDragAcceptability(const QMimeData *mime) const;

    void import(const QString &path);

private:
    Ui::AddLuaFileDialog *ui;

    QString luaDir;

private slots:
    void on_importButton_clicked();

    void on_CancelButton_clicked();

    void on_OKButton_clicked();
};
