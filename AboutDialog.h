#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT

    QLabel logoLbl;
    QPlainTextEdit infoEdit;
    QPushButton closeBtn;

public:
    explicit AboutDialog(QWidget *parent = 0);

};

#endif // ABOUTDIALOG_H
