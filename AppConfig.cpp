#include "AppConfig.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    //read
    m_Valid=read();
}

AppConfig::AppConfig(QPoint point, QSize size, QFont font,bool fullScreen, bool chinese, bool autoWrap, bool sbBar, bool tbBar, QStringList filepath, QObject *parent) : QObject(parent)
{
    //write
    m_filepath=filepath;
    m_point=point;
    m_size=size;
    m_editorFont=font;
    m_fullScreen=fullScreen;
    m_chinese=chinese;
    m_autoWrap=autoWrap;
    m_statusBar=sbBar;
    m_toolBar=tbBar;
    m_Valid=true;
}

bool AppConfig::read()
{
    bool ret=true;
    QFile file("./app.config");
    if(file.open(QIODevice::ReadOnly))
    {

        QDataStream in(&file);
        in>>m_point;
        in>>m_size;
        in>>m_editorFont;
        in>>m_fullScreen;
        in>>m_chinese;
        in>>m_autoWrap;
        in>>m_statusBar;
        in>>m_toolBar;
        in>>m_filepath;

        file.close();
    }
    else
    {
        ret=false;
    }
    return ret;
}

bool AppConfig::write()
{
    bool ret=true;
    QFile file("./app.config");

    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out<<m_point;
        out<<m_size;
        out<<m_editorFont;
        out<<m_fullScreen;
        out<<m_chinese;
        out<<m_autoWrap;
        out<<m_statusBar;
        out<<m_toolBar;
        out<<m_filepath;

        file.close();
    }
    else
    {
        ret=false;
    }
    return ret;
}

QStringList AppConfig::getFilepath()
{

    return m_filepath;
}

QFont AppConfig::getEditorFont()
{
    return m_editorFont;
}

bool AppConfig::isMaxScreen()
{
    return m_fullScreen;
}

bool AppConfig::isChinese()
{
    return m_chinese;
}

bool AppConfig::isAutoWrap()
{
    return m_autoWrap;
}

bool AppConfig::isSbVisible()
{
    return m_statusBar;
}

bool AppConfig::isTbVisible()
{
    return m_toolBar;
}

bool AppConfig::isValid()
{
    return m_Valid;
}

QPoint AppConfig::getPoint()
{
    return m_point;
}

QSize AppConfig::getSize()
{
    return m_size;
}

