#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QWidget>
#include <QWheelEvent>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>
#include <Qsci/qscistyle.h>

class TextEdit : public QsciScintilla
{
public:
    explicit TextEdit(QWidget *parent = nullptr);
    ~TextEdit();

    QString getFilePath();
    void setTextFont(QFont font, const QString &line);
    void setFilePath(QString path);
    void wheelEvent(QWheelEvent* evt);
    void dragEnterEvent(QDragEnterEvent *evt);
    void dropEvent(QDropEvent *evt);

    //void keyPressEvent(QKeyEvent* evt);
    //void keyReleaseEvent(QKeyEvent* evt);

    void initCPPLexer();
    void initCS();
    void initJava();
    void initPython();

protected:
    QString filePath;
    QsciLexer* textLexer;
    QsciAPIs *apis;

    void initWidget();


};

#endif // TEXTEDIT_H
