#include "wechatdonatedialog.hpp"
#include "ui_wechatdonatedialog.h"



WeChatDonateDialog::WeChatDonateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WeChatDonateDialog)
{
    ui->setupUi(this);
}

WeChatDonateDialog::~WeChatDonateDialog()
{
    delete ui;
}
