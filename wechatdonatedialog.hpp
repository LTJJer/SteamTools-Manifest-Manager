#pragma once

#include <QDialog>



namespace Ui { class WeChatDonateDialog; }



class WeChatDonateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeChatDonateDialog(QWidget *parent = nullptr);
    ~WeChatDonateDialog();

private:
    Ui::WeChatDonateDialog *ui;
};
