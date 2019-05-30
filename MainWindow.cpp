#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AppConfig.h"

#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabWidget(new QTabWidget),
    findDialog(this,NULL)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    initTabWidget();
    initStatusBar();
    setCentralWidget(tabWidget);
    setAcceptDrops(true);
    trans=new QTranslator(this);
    watcher=new QFileSystemWatcher(this);

    font= QFont("Lingoes Unicode",12);

    readConfig();//读取上一次配置

    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(slotNew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(slotOpen()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(slotSave()));
    connect(ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(slotSaveAs()));
    connect(ui->actionRemove,SIGNAL(triggered()),this,SLOT(slotRemove()));
    connect(ui->actionRename,SIGNAL(triggered()),this,SLOT(slotRename()));
    connect(ui->actionPrint,SIGNAL(triggered()),this,SLOT(slotPrint()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAutoWrap,SIGNAL(triggered(bool)),this,SLOT(slotAutoWrap(bool)));
    connect(ui->actionFont,SIGNAL(triggered()),this,SLOT(slotFont()));
    connect(ui->actionGoto,SIGNAL(triggered()),this,SLOT(slotGoto()));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(slotFind()));
    connect(ui->actionChinese,SIGNAL(triggered()),this,SLOT(slotChangeLanguage()));
    connect(ui->actionEnglish,SIGNAL(triggered()),this,SLOT(slotChangeLanguage()));

    connect(ui->actionToolBar,SIGNAL(triggered(bool)),this,SLOT(slotShowToolBar(bool)));
    connect(ui->actionStatusBar,SIGNAL(triggered(bool)),this,SLOT(slotShowStatusBar(bool)));
    connect(ui->actionUser,SIGNAL(triggered()),this,SLOT(slotUser()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(slotAbout()));
    connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(slotFileWatcher(QString)));

    connect(ui->actionCSharp,SIGNAL(triggered()),this,SLOT(slotInitCS()));
    connect(ui->actionJava,SIGNAL(triggered()),this,SLOT(slotInitJava()));
    connect(ui->actionCPP,SIGNAL(triggered()),this,SLOT(slotInitCPP()));
    connect(ui->actionPython,SIGNAL(triggered()),this,SLOT(slotInitPython()));
}

void MainWindow::initTabWidget()
{
    CreateNewTab();
    tabWidget->setTabsClosable(true);
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(slotTabChanged(int)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(slotTabClosed(int)));

}

void MainWindow::initStatusBar()
{
        QLabel* la=new QLabel("Y.Software");
        if(la != NULL)
        {
            la->setMinimumWidth(200);
            la->setAlignment(Qt::AlignCenter);

            labelLength=new QLabel;
            labelLength->setMinimumWidth(200);
            labelLength->setAlignment(Qt::AlignCenter);
            labelLength->setText("Lines:1 Length:1");

            label=new QLabel;
            label->setMinimumWidth(200);
            label->setAlignment(Qt::AlignCenter);
            label->setText("ln:1 col:1");

            ui->statusBar->addPermanentWidget(label);
            ui->statusBar->addPermanentWidget(labelLength);
            ui->statusBar->addPermanentWidget(la);
        }
}

void MainWindow::closeEvent(QCloseEvent* evt)
{
    int ret=-100;
    QStringList openList;
    QStringList saveList;
    QString content=tr("save these files?\n");

    for(int i=0;i<tabWidget->count();i++)
    {
        mainEditor=dynamic_cast<TextEdit*>(tabWidget->widget(i));
        if(getPath() != "")
        {
            openList<<getPath();
            if(mainEditor->isModified())
            {
                saveList<<getPath();
                content+=getPath()+"\n";
            }
        }
    }

    if(!saveList.empty())
    {
        ret=showQuestionDialog(tr("Save"),content);
    }

    switch(ret)
    {
    case QMessageBox::Yes:
        for(auto i : saveList)
        {
            SaveFile(i);
        }
        evt->accept();
        break;
    case QMessageBox::No:
        evt->accept();
        break;
    case QMessageBox::Cancel:
        evt->ignore();
        break;
    default:
        evt->accept();
        break;
    }
    writeConfig(openList);//写入config
}

void MainWindow::dragEnterEvent(QDragEnterEvent* evt)
{
    if(evt->mimeData()->hasUrls())
    {
        evt->acceptProposedAction();
    }
    else
    {
        evt->ignore();
    }

}

void MainWindow::dropEvent(QDropEvent* evt)
{
    QList<QUrl> list=evt->mimeData()->urls();
    QString path=list[0].toLocalFile();
    QFileInfo file(path);
    if(file.isFile())
    {
        OpenFile(path);
        evt->accept();
    }
    else
    {
        evt->ignore();
    }
}

void MainWindow::wheelEvent(QWheelEvent *evt)
{
    if(evt->modifiers() == Qt::ControlModifier)
    {
        QPoint point=evt->angleDelta();
        if(point.y() > 0)
        {
            int point=font.pointSize()+1;
            font.setPointSize(point>40?40:point);
            mainEditor->setTextFont(font,getLineString());
        }
        else
        {
            int point=font.pointSize()-1;
            font.setPointSize(point<1?1:point);
            mainEditor->setTextFont(font,getLineString());
        }
        evt->accept();
    }
}

//每次发生重命名，打开，保存，新建，tabchange时根据文件路径命名
void MainWindow::setTitle()
{
    QString path=getPath();
    QFileInfo file(path);

    QString s="";
    //如果tab以星号开头，则加上星号
    if(mainEditor->isModified())
        s="*";


    if(path != "")
    {
        tabWidget->setTabText(tabWidget->currentIndex(),s+file.fileName());
        setWindowTitle(s+file.absoluteFilePath()+" - EasyCode");
    }
    else
    {
        QString tab=tabWidget->tabText(tabWidget->currentIndex());
        if(tab.startsWith("*"))
            s="";

        tabWidget->setTabText(tabWidget->currentIndex(),s+tab);
        setWindowTitle(s+tab+" - EasyCode");
    }

    //状态检查
    checkStatus();
}

QString MainWindow::getPath()
{
    return mainEditor->getFilePath();
}

//在new tab打开文件，删除tab，保存文件几种可能发生改名的情况下，将占用的数组位置0
void MainWindow::setNum0(int index)
{
    QString text=tabWidget->tabText(index);
    if(text.startsWith("new") || text.startsWith("*new"))
    {
        bool ok;
        int n=text.right(2).toInt(&ok);
        if(ok && 0 <= n && n < 64)
        {
            num[n]=0;
        }
    }
}

void MainWindow::showLength()
{
    labelLength->setText(QString("Lines:%1 Length:%2")
                         .arg(mainEditor->lines()).arg(mainEditor->length()));
}

void MainWindow::ConnectTextEdit()
{
    connect(ui->actionCopy,SIGNAL(triggered()),mainEditor,SLOT(copy()));
    connect(ui->actionCut,SIGNAL(triggered()),mainEditor,SLOT(cut()));
    connect(ui->actionPaste,SIGNAL(triggered()),mainEditor,SLOT(paste()));
    connect(ui->actionRedo,SIGNAL(triggered()),mainEditor,SLOT(redo()));
    connect(ui->actionUndo,SIGNAL(triggered()),mainEditor,SLOT(undo()));
    connect(ui->actionFoldAll,SIGNAL(triggered(bool)),mainEditor,SLOT(foldAll(bool)));
    connect(ui->actionSellctAll,SIGNAL(triggered()),mainEditor,SLOT(selectAll()));
    connect(ui->actionSizeUp,SIGNAL(triggered()),mainEditor,SLOT(zoomIn()));
    connect(ui->actionSizeDown,SIGNAL(triggered()),mainEditor,SLOT(zoomOut()));

    connect(mainEditor,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(slotCursorChanged(int,int)));

    //copy可用时，显示cut和copy按钮
    connect(mainEditor,SIGNAL(copyAvailable(bool)),ui->actionCopy,SLOT(setEnabled(bool)));
    connect(mainEditor,SIGNAL(copyAvailable(bool)),ui->actionCut,SLOT(setEnabled(bool)));

    connect(mainEditor,SIGNAL(textChanged()),this,SLOT(slotTextChanged()));
    connect(mainEditor,SIGNAL(linesChanged()),this,SLOT(slotLineChanged()));
}

void MainWindow::checkStatus()
{
    bool status;

    //设置保存状态
    status=mainEditor->isModified();
    ui->actionSave->setEnabled(status);

    //设置重命名和删除文件状态
    status=(mainEditor->getFilePath() != "");
    ui->actionRename->setEnabled(status);
    ui->actionRemove->setEnabled(status);

    //设置undo redo状态
    status=mainEditor->isRedoAvailable();
    ui->actionRedo->setEnabled(status);
    status=mainEditor->isUndoAvailable();
    ui->actionUndo->setEnabled(status);

    //设置copy cut状态
    status= (mainEditor->selectedText() != "") ;
    ui->actionCopy->setEnabled(status);
    ui->actionCut->setEnabled(status);

    status= ui->actionAutoWrap->isChecked();
    mainEditor->setWrapMode(status?QsciScintilla::WrapWhitespace:QsciScintilla::WrapNone);
}

void MainWindow::readConfig()
{
    //加载上次保存的来记录
    AppConfig config;
    if(config.isValid())
    {
        if(config.isMaxScreen())
        {
            showMaximized();
        }
        else
        {
            resize(config.getSize());
            move(config.getPoint());
        }

        ui->actionAutoWrap->setChecked(config.isAutoWrap());

        //初始状态都为true，如果为false则改变状态
        if(!config.isAutoWrap())
        {

            ui->actionAutoWrap->setChecked(false);
            slotAutoWrap(false);
        }

        if(!config.isTbVisible())
        {
            ui->actionToolBar->setChecked(false);
            ui->mainToolBar->hide();
        }


        if(!config.isSbVisible())
        {
            ui->actionStatusBar->setChecked(false);
            ui->statusBar->hide();
        }


        if(!config.isChinese())
        {
            slotChangeLanguage();
        }

        QStringList list=config.getFilepath();
        if(!list.empty())
        {
            for(auto i:list)
            {
                if(QFile(i).exists())
                    OpenFile(i);
            }
        }

        font=config.getEditorFont();
        mainEditor->setTextFont(font,getLineString());
    }

    showLength();
}

void MainWindow::writeConfig(QStringList openList)
{
    AppConfig config(pos(),size(),font,isMaximized(),
                     ui->actionChinese->isEnabled(),
                     ui->actionAutoWrap->isChecked(),
                     ui->actionStatusBar->isChecked(),
                     ui->actionToolBar->isChecked(),openList);
    config.write();
}

/////////////////////////////辅助函数////////////////////////////////
QString MainWindow::showFileDialog(const QString& title,QFileDialog::AcceptMode mode)
{
    QString fileName="";
    QFileDialog dialog(this);  
    dialog.setWindowTitle(title);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(mode);  
    dialog.setDefaultSuffix(".cpp");
    QStringList filters;
    QMap<QString,QString> map;



    const char* name[][2]={
        {"All files(*)","*"},
        {"Text(*.txt)",".txt"},
        {"C++(*.cpp)",".cpp"},
        {NULL,NULL}
    };

    for(int i=0;name[i][0] != NULL;i++)
    {
        filters<<name[i][0];
        map.insert(name[i][0],name[i][1]);
    }

    dialog.setNameFilters(filters);
    dialog.selectNameFilter(filters[0]);//设置默认选择的后缀过滤

    //区分对话框是打开模式还是保存模式
    if(mode == QFileDialog::AcceptOpen)
    {
        dialog.setFileMode(QFileDialog::ExistingFile);
    }
    else
    {
        dialog.setFileMode(QFileDialog::AnyFile);
        if(getPath() != "")
        {
            dialog.selectFile(getPath());
        }
    }

    //取得文件路径
    if(dialog.exec())
    {
        fileName=dialog.selectedFiles()[0];
        //添加默认后缀,自带后缀.txt????

        if(mode == QFileDialog::AcceptSave)
        {
            QString postfix=map[dialog.selectedNameFilter()];
            if(fileName.endsWith("*") && !fileName.endsWith(postfix) )
            {
                fileName+=postfix;
            }
        }

    }
    return fileName;
}

void MainWindow::showErrorMessage(const QString& content)
{
    QMessageBox box(this);
    box.setWindowTitle("Error");
    box.setText(content);
    box.setIcon(QMessageBox::Warning);
    box.setStandardButtons(QMessageBox::Ok);

    box.exec();
}

int MainWindow::showQuestionDialog(const QString& title,const QString& content)
{
    QMessageBox box(this);
    box.setIcon(QMessageBox::Question);
    box.setWindowTitle(title);
    box.setText(content);
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Yes);

    return box.exec();
}

QString MainWindow::SaveFile(QString path)
{
    if(path == "")
    {
        path=showFileDialog(tr("Save As"),QFileDialog::AcceptSave);
        setNum0(tabWidget->currentIndex());
    }

    if(path != "")
    {
        QFile file(path);
        watcher->removePath(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out<<mainEditor->text();
            file.close();
            watcher->addPath(path);

            mainEditor->setFilePath(path);
            mainEditor->setModified(false);
            setTitle();
            statusBar()->showMessage("Saved:"+path,3000);
        }
        else
        {
            showErrorMessage(file.errorString());
        }
    }
    return path;
}

void MainWindow::OpenFile(QString path)
{    
    if(path != "")
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //如果以new开头并且文本为空就在此tab中打开，不新建
            QString name=tabWidget->tabText(tabWidget->currentIndex());
            if( mainEditor->text() == "" && name.startsWith("new") )
            {
                setNum0(tabWidget->currentIndex());
            }
            else
            {
                CreateNewTab();
                setNum0(tabWidget->currentIndex());
            }

            QTextStream in(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            mainEditor->setText(in.readAll());
            QApplication::restoreOverrideCursor();
            file.close();


            mainEditor->setFilePath(path);
            mainEditor->setModified(false);
            watcher->addPath(path);
            setTitle();
            showLength();
            statusBar()->showMessage("Open:"+path,3000);
        }
        else
        {
            showErrorMessage(file.errorString());
        }
    }
}

void MainWindow::deleteTab(int index)
{
    //////有问题，如果tab中途被改名，则此序号不会被置0
    //使用setNum0代替，并在save as, openFile处使用
    setNum0(index);

    if(tabWidget->count() == 1)
    {
        CreateNewTab();
    }
    tabWidget->removeTab(index);
}

int MainWindow::CreateNewTab()
{
    //新建并切换tab
    int index=-1;
    //每次新建时遍历数组，查看那个序号空闲
    for(int i=0;i<64;i++)
    {
        if(!num[i])
        {
            index=tabWidget->addTab(new TextEdit(this),"new "+QString::number(i));

            //如果没有新建则出错
            if(index == -1)
            {
                showErrorMessage(tr("can not create a new tab"));
            }

            tabWidget->setCurrentIndex(index);
            num[i]=1;//新建完成后置1
            //将当前tab绑定到mainTextWidget上
            mainEditor=dynamic_cast<TextEdit*>(tabWidget->currentWidget());
            if(mainEditor)
            {
                //链接mainTextWidget的信号与槽，每新建一个tab链接一次
                ConnectTextEdit();
                mainEditor->setTextFont(font,getLineString());
            }
            break;
        }
    }

    return index;
}

QString MainWindow::getLineString() const
{
    return QString::number(mainEditor->lines() * 10);
}
