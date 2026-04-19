#include "messagedialog.h"
#include "settings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

MessageDialog::MessageDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    Settings::applyTheme(Settings::currentTheme(), this);
}

void MessageDialog::setupUI()
{
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint | Qt::MSWindowsFixedSizeDialogHint);
    setMinimumWidth(400);
    setMaximumWidth(400);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

#ifdef Q_OS_WIN
    HWND hwnd = (HWND)winId();
    if (hwnd) {
        LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
        style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
        SetWindowLongPtrW(hwnd, GWL_STYLE, style);
    }
#endif

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    outerLayout->addStretch();

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(20);
    topLayout->setContentsMargins(28, 16, 28, 0);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    topLayout->addWidget(m_iconLabel, 0, Qt::AlignTop);

    m_textLabel = new QLabel(this);
    m_textLabel->setWordWrap(true);
    m_textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    topLayout->addWidget(m_textLabel, 1, Qt::AlignTop);

    outerLayout->addLayout(topLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 12, 28, 16);
    buttonLayout->addStretch();

    m_yesButton = new QPushButton("确定", this);
    m_noButton = new QPushButton("取消", this);
    m_okButton = new QPushButton("确定", this);

    m_yesButton->setMinimumWidth(70);
    m_noButton->setMinimumWidth(70);
    m_okButton->setMinimumWidth(70);

    connect(m_yesButton, &QPushButton::clicked, this, [this]() { done(QDialog::Accepted); });
    connect(m_noButton, &QPushButton::clicked, this, [this]() { done(QDialog::Rejected); });
    connect(m_okButton, &QPushButton::clicked, this, [this]() { done(QDialog::Accepted); });

    buttonLayout->addWidget(m_yesButton);
    buttonLayout->addWidget(m_noButton);
    buttonLayout->addWidget(m_okButton);

    outerLayout->addLayout(buttonLayout);
    outerLayout->addStretch();

    setStandardButtons(false);
}

void MessageDialog::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void MessageDialog::setText(const QString &text)
{
    m_textLabel->setText(text);
}

void MessageDialog::setIcon(MessageIcon icon)
{
    QStyle::StandardPixmap pixmap;
    switch (icon) {
    case MessageIcon::Information:
        pixmap = QStyle::SP_MessageBoxInformation;
        break;
    case MessageIcon::Warning:
        pixmap = QStyle::SP_MessageBoxWarning;
        break;
    case MessageIcon::Critical:
        pixmap = QStyle::SP_MessageBoxCritical;
        break;
    case MessageIcon::Question:
        pixmap = QStyle::SP_MessageBoxQuestion;
        break;
    default:
        m_iconLabel->hide();
        return;
    }

    int iconSize = style()->pixelMetric(QStyle::PM_MessageBoxIconSize);
    QIcon qIcon = style()->standardIcon(pixmap);
    m_iconLabel->setPixmap(qIcon.pixmap(QSize(iconSize, iconSize)));
    m_iconLabel->show();
}

void MessageDialog::setStandardButtons(bool yesNo)
{
    if (yesNo) {
        m_yesButton->show();
        m_noButton->show();
        m_okButton->hide();
    } else {
        m_yesButton->hide();
        m_noButton->hide();
        m_okButton->show();
    }
}

void MessageDialog::information(QWidget *parent, const QString &title, const QString &text)
{
    MessageDialog dlg(parent);
    dlg.setTitle(title);
    dlg.setText(text);
    dlg.setIcon(MessageIcon::Information);
    dlg.setStandardButtons(false);
    dlg.exec();
}

void MessageDialog::warning(QWidget *parent, const QString &title, const QString &text)
{
    MessageDialog dlg(parent);
    dlg.setTitle(title);
    dlg.setText(text);
    dlg.setIcon(MessageIcon::Warning);
    dlg.setStandardButtons(false);
    dlg.exec();
}

void MessageDialog::critical(QWidget *parent, const QString &title, const QString &text)
{
    MessageDialog dlg(parent);
    dlg.setTitle(title);
    dlg.setText(text);
    dlg.setIcon(MessageIcon::Critical);
    dlg.setStandardButtons(false);
    dlg.exec();
}

bool MessageDialog::question(QWidget *parent, const QString &title, const QString &text)
{
    MessageDialog dlg(parent);
    dlg.setTitle(title);
    dlg.setText(text);
    dlg.setIcon(MessageIcon::Question);
    dlg.setStandardButtons(true);
    return dlg.exec() == QDialog::Accepted;
}
