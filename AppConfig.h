#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QFont>
#include <QSize>
#include <QPoint>
#include <QStringList>

class AppConfig : public QObject
{
    Q_OBJECT
    QPoint m_point;
    QSize m_size;
    QFont m_editorFont;
    QStringList m_filepath;
    bool m_fullScreen;
    bool m_autoWrap;
    bool m_statusBar;
    bool m_toolBar;
    bool m_chinese;
    bool m_Valid;
    bool read();

public:
    explicit AppConfig(QObject *parent = 0);
    explicit AppConfig(QPoint point,QSize size,QFont font,bool m_fullScreen,bool chinese,bool autoWrap,bool sbBar,bool tbBar,QStringList filepath,QObject *parent = 0);
    bool write();
    QPoint getPoint();
    QSize getSize();
    QStringList getFilepath();
    QFont getEditorFont();
    bool isMaxScreen();
    bool isChinese();
    bool isAutoWrap();
    bool isSbVisible();
    bool isTbVisible();
    bool isValid();
};

#endif // APPCONFIG_H
