#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

class QLabel;
class QPushButton;

enum class MessageIcon { None, Information, Warning, Critical, Question };

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = nullptr);
    
    static void information(QWidget *parent, const QString &title, const QString &text);
    static void warning(QWidget *parent, const QString &title, const QString &text);
    static void critical(QWidget *parent, const QString &title, const QString &text);
    static bool question(QWidget *parent, const QString &title, const QString &text);
    
    void setTitle(const QString &title);
    void setText(const QString &text);
    void setIcon(MessageIcon icon);
    void setStandardButtons(bool yesNo);

private:
    QLabel *m_iconLabel;
    QLabel *m_textLabel;
    QPushButton *m_yesButton;
    QPushButton *m_noButton;
    QPushButton *m_okButton;
    
    void setupUI();
};

#endif // MESSAGEDIALOG_H
