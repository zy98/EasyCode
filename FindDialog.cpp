#include "FindDialog.h"

FindDialog::FindDialog(QWidget *parent,TextEdit* edit)
    : QDialog(parent,Qt::WindowCloseButtonHint)
{
    m_edit=edit;
    m_findLabel.setText(tr("Find what:"));
    m_replaceLabel.setText(tr("Replace:"));

    m_findNextBtn.setText(tr("Find Next"));
    m_findLastBtn.setText(tr("Find Last"));
    m_replaceBtn.setText(tr("Replace"));
    m_replaceAllBtn.setText(tr("Replace All"));

    m_caseBox.setText(tr("Case"));
    m_wordBox.setText(tr("Word"));

    m_gLayout.addWidget(&m_findLabel,0,0);
    m_gLayout.addWidget(&m_findEdit,0,1,1,3);
    m_gLayout.addWidget(&m_replaceLabel,1,0);
    m_gLayout.addWidget(&m_replaceEdit,1,1,1,3);

    m_gLayout.addWidget(&m_caseBox,2,0);
    m_gLayout.addWidget(&m_findLastBtn,2,2);
    m_gLayout.addWidget(&m_findNextBtn,2,3);
    m_gLayout.addWidget(&m_wordBox,3,0);
    m_gLayout.addWidget(&m_replaceBtn,3,2);
    m_gLayout.addWidget(&m_replaceAllBtn,3,3);

    m_gLayout.setSpacing(15);
    setLayout(&m_gLayout);
    setWindowTitle("Find");

    connect(&m_findNextBtn,SIGNAL(clicked()),this,SLOT(slotFindNext()));
    connect(&m_findLastBtn,SIGNAL(clicked()),this,SLOT(slotFindLast()));
    connect(&m_replaceBtn,SIGNAL(clicked()),this,SLOT(slotReplace()));
    connect(&m_replaceAllBtn,SIGNAL(clicked()),this,SLOT(slotReplaceAll()));

}

bool FindDialog::event(QEvent * evt)
{
    if(evt->type() == QEvent::Close)
    {
        hide();
        return true;
    }
    return QDialog::event(evt);
}

bool FindDialog::find(bool forward)
{
    QString target=m_findEdit.text();
    int line=0,index=0;
    m_edit->getCursorPosition(&line,&index);
    bool ret = m_edit->findFirst(target,false,
                                 m_caseBox.isChecked(),
                                 m_wordBox.isChecked(),
                                 false,forward,line,index,true,false);
    if(!forward && ret)
    {
        m_edit->getCursorPosition(&line,&index);
        m_edit->setSelection(line,index,line,index-target.length());
    }
    return ret;
}

void FindDialog::showMessage(const QString& content,int num)
{
    QString text;
    if(num<0)
        text=m_findEdit.text();
    else
        text=QString::number(num);

    QMessageBox msg(this);
    msg.setWindowTitle("Find");
    msg.setText(content + text);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);

    msg.exec();
}

void FindDialog::slotFindNext()
{
    bool ret = find();
    if(!ret)
        showMessage(tr("can not find:"));
}

void FindDialog::slotFindLast()
{
    bool ret = find(false);
    if(!ret)
        showMessage(tr("can not find:"));
}

void FindDialog::slotReplace()
{
    QString text=m_replaceEdit.text();
    m_edit->replaceSelectedText(text);
    bool ret = find();
    if(!ret)
        showMessage(tr("can not find:"));
}

void FindDialog::slotReplaceAll()
{
    //查找文本框和替换文本框内容不同时才进行替换全部，否则会进入死循环
    QString text=m_replaceEdit.text();
    if(m_findEdit.text() != text)
    {
        int times=0;
        m_edit->setCursorPosition(0,0);
        while(true)
        {
            if(!find())
            {
                showMessage(tr("Repalced: "),times);
                break;
            }
            m_edit->replaceSelectedText(text);
            ++times;
        }
    }
}

void FindDialog::setTextEdit(TextEdit* edit)
{
    m_edit=edit;
}
