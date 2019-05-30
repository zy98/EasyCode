#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QPointer>
#include <QMessageBox>
#include "TextEdit.h"

class FindDialog : public QDialog
{
    Q_OBJECT
protected:
    QLabel m_findLabel;
    QLineEdit m_findEdit;
    QLabel m_replaceLabel;
    QLineEdit m_replaceEdit;

    QCheckBox m_caseBox;
    QCheckBox m_wordBox;
    QPushButton m_findNextBtn;
    QPushButton m_findLastBtn;
    QPushButton m_replaceBtn;
    QPushButton m_replaceAllBtn;

    QGridLayout m_gLayout;
    QPointer<TextEdit> m_edit;

    bool find(bool forward=true);
    void showMessage(const QString& content,int num=-1);

protected slots:
    void slotFindNext();
    void slotFindLast();
    void slotReplace();
    void slotReplaceAll();

public:
    explicit FindDialog(QWidget *parent = 0,TextEdit* edit=0);
    bool event(QEvent * evt);
    void setTextEdit(TextEdit* edit);

};

#endif // FINDDIALOG_H
