#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QFileDialog>
#include <QCloseEvent>
#include <QFont>
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMap>
#include <QTranslator>
#include <QFileSystemWatcher>
#include <QMimeData>

#include "TextEdit.h"
#include "FindDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void OpenFile(QString path);//

protected:
    void closeEvent(QCloseEvent* evt);
    void dragEnterEvent(QDragEnterEvent* evt);
    void dropEvent(QDropEvent* evt);
    void wheelEvent(QWheelEvent* evt);

protected slots:
    //file
    void slotNew();
    void slotOpen();
    void slotSave();
    void slotSaveAs();
    void slotRemove();
    void slotRename();
    void slotPrint();

    //edit  
    void slotFind();
    void slotGoto();

    //view
    void slotShowToolBar(bool a);
    void slotShowStatusBar(bool a);

    //settings
    void slotAutoWrap(bool a);
    void slotFont();
    void slotChangeFont(QFont f);
    void slotChangeLanguage();

    //help
    void slotUser();
    void slotAbout();

    //other
    void slotTabChanged(int index);
    void slotTabClosed(int index);
    void slotTextChanged();
    void slotCursorChanged(int line, int index);
    void slotFileWatcher(const QString& path);
    void slotLineChanged();

    void slotInitCPP();
    void slotInitCS();
    void slotInitJava();
    void slotInitPython();

private:
    Ui::MainWindow *ui;
    QTabWidget* tabWidget;
    TextEdit* mainEditor;
    FindDialog findDialog;
    QTranslator* trans;
    static char num[64];
    QLabel* label;
    QLabel* labelLength;
    QFileSystemWatcher* watcher;
    QFont font;

    void initMainWindow();
    void initTabWidget();
    void initStatusBar();
    int CreateNewTab();//
    void ConnectTextEdit();//

    //辅助功能函数封装
    void deleteTab(int index);//
    QString SaveFile(QString path="");//
    QString showFileDialog(const QString& title, QFileDialog::AcceptMode mode);
    void showErrorMessage(const QString& content);
    int showQuestionDialog(const QString& title,const QString& content);
    void setTitle();////

    //用于每次切换tab时检查保存，重命名，删除文件，undo，redo，cut，copy的状态
    void checkStatus();
    QString getPath();
    void readConfig();
    void writeConfig(QStringList openList);
    void setNum0(int index);//
    void showLength();
    QString getLineString() const;
};

#endif // MAINWINDOW_H
