#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QDesktopServices>
#include <QFontDialog>
#include <QInputDialog>
#include <QPrintDialog>

#include <Qsci/qsciprinter.h>

char MainWindow::num[64]={0};

void MainWindow::slotOpen()
{
    QString path=showFileDialog(tr("Open"),QFileDialog::AcceptOpen);
    OpenFile(path); 
}

void MainWindow::slotSave()
{
    SaveFile(getPath());
}

void MainWindow::slotSaveAs()
{
    SaveFile();
}

void MainWindow::slotRemove()
{
    QString path=getPath();
    QFile file(path);

     if(file.exists())
     {
         int ret=showQuestionDialog(tr("delete"),tr("delete this file?"));
         switch(ret)
         {
         case QMessageBox::Yes:
             watcher->removePath(path);
             if(file.remove())
             {
                 deleteTab(tabWidget->currentIndex());
                 statusBar()->showMessage("Remove"+path,3000);
             }
             break;
         case QMessageBox::No:
         case QMessageBox::Cancel:
         default:break;
         }
     }
}

void MainWindow::slotRename()
{
    QString path  =getPath();
    QFile oldName(path);
    //路径不为空，则可以重命名
    if(path != "")
    {
        QString filePath=showFileDialog(tr("Save As"),QFileDialog::AcceptSave);
        if(oldName.exists())
        {
            QFileInfo newName=(filePath);
            watcher->removePath(path);
            bool b=oldName.rename(newName.fileName());
            if(!b)
            {
                watcher->addPath(path);
                showErrorMessage(tr("rename fialed"));
            }
            else
            { 
                watcher->addPath(filePath);
                mainEditor->setFilePath(newName.absoluteFilePath());
                setTitle();
                statusBar()->showMessage("Rename:"+newName.fileName(),3000);
            }
        }
        else
        {
            showErrorMessage(oldName.errorString());
        }
    }
}

void MainWindow::slotPrint()
{
    QPrintDialog dialog(this);
    dialog.setWindowTitle(tr("Print"));

    if(dialog.exec() == QPrintDialog::Accepted)
    {
        (new QsciPrinter)->printRange(mainEditor);
    }
}

void MainWindow::slotFind()
{
    findDialog.setTextEdit(mainEditor);
    findDialog.show();
}

void MainWindow::slotGoto()
{
    bool ok;
    int line=mainEditor->lines();

    int ln=QInputDialog::getInt(this,tr("Goto"),tr("line:"),1,1,line,1,&ok,
                                Qt::WindowCloseButtonHint);
    if(ok)
        mainEditor->setCursorPosition(ln-1,0);
}

void MainWindow::slotTabChanged(int index)
{
    mainEditor=dynamic_cast<TextEdit*>(tabWidget->widget(index));//获取当前文本编辑器地址

    mainEditor->setTextFont(font,getLineString());//设置当前文本框字体
    showLength();

    setTitle();

    if(!findDialog.isHidden())
        findDialog.setTextEdit(mainEditor);
}

void MainWindow::slotTabClosed(int index)
{
    int ret=-1;
    if(mainEditor->isModified())
    {
        ret=showQuestionDialog(tr("Save"),tr("Save this file?"));
    }

    QString path = getPath();
    switch(ret)
    {
    case QMessageBox::Yes:
        SaveFile(path);
        if(path != "")//如果path不为空，则先删除监视器，再移除tab
            watcher->removePath(path);
        deleteTab(index);        
        break;
    case QMessageBox::Cancel:
        break;
    case QMessageBox::No:
    default:
        if(path != "")//如果path不为空，则先删除监视器，再移除tab
            watcher->removePath(path);
        deleteTab(index);
        break;
    }
}

void MainWindow::slotCursorChanged(int line,int index)
{
    showLength();
    QString str=QString("ln:%1 col:%2").arg(line+1).arg(index+1);
    label->setText(str);

}

//settings
void MainWindow::slotFont()
{
    bool ok;
    font=QFontDialog::getFont(&ok,mainEditor->font());
    if(ok)
        mainEditor->setTextFont(font,getLineString());
}

void MainWindow::slotChangeFont(QFont f)
{
    font=f;
}

//help
void MainWindow::slotUser()
{
    QFileInfo file("./Readme.txt");

    if(file.isFile())
        OpenFile(file.absoluteFilePath());
    else
        QDesktopServices::openUrl(QUrl("www.baidu.com"));
}

void MainWindow::slotFileWatcher(const QString &path)
{
    int i;
    TextEdit* temp=NULL;
    for(i=0;i<tabWidget->count();i++)
    {
        temp=dynamic_cast<TextEdit*>(tabWidget->widget(i));
        if(temp->getFilePath() == path)
            break;
    }
    if(temp)
    {
        watcher->blockSignals(true);
        mainEditor=temp;
        QString text=path+"\n"+tr("has been modified by another application \n reload?");
        int ret = showQuestionDialog(tr("Reload"),text);
        watcher->blockSignals(false);
        if(ret == QMessageBox::Yes)
        {
            deleteTab(i);
            OpenFile(path);
        }
        else
        {
            setTitle();
        }
        qDebug()<<watcher->signalsBlocked();
    }

}

void MainWindow::slotInitCPP()
{
    mainEditor->initCPPLexer();
    mainEditor->setTextFont(font,getLineString());
}

void MainWindow::slotInitCS()
{
    mainEditor->initCS();
    mainEditor->setTextFont(font,getLineString());
}

void MainWindow::slotInitJava()
{
    mainEditor->initJava();
    mainEditor->setTextFont(font,getLineString());
}

void MainWindow::slotInitPython()
{
    mainEditor->initPython();
    mainEditor->setTextFont(font,getLineString());
}

void MainWindow::slotChangeLanguage()
{
    bool status=ui->actionChinese->isEnabled();
    ui->actionChinese->setEnabled(!status);
    ui->actionEnglish->setEnabled(status);

    if(!ui->actionChinese->isEnabled())
    {
        if(trans->load("./EasyCode.qm"))
        {
            qApp->installTranslator(trans);
            ui->retranslateUi(this);
        }
        else
        {
            statusBar()->showMessage(tr("can not find this translation file!"),5000);
        }
    }
    else
    {
        qApp->removeTranslator(trans);
        ui->retranslateUi(this);
    }
}

void MainWindow::slotTextChanged()
{
    ui->actionRedo->setEnabled(mainEditor->isRedoAvailable());
    ui->actionUndo->setEnabled(mainEditor->isUndoAvailable());

    if(!mainEditor->isModified())
        mainEditor->setModified(true);
    setTitle();
}

void MainWindow::slotLineChanged()
{
    mainEditor->setMarginWidth(0,getLineString());
}

