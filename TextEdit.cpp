#include "TextEdit.h"
#include <QDebug>
#include <QColor>


TextEdit::TextEdit(QWidget *parent) : QsciScintilla(parent)
{
    initWidget();
}

void TextEdit::initWidget()
{
    textLexer=new QsciLexerCPP(this);
    //设置为UTF8编码，原本为ascii
    setUtf8(true);

    //keypress相关
    setFocusPolicy(Qt::ClickFocus);

    //设置margin0显示行号
    setMarginType(0,QsciScintilla::NumberMargin);
    setMarginLineNumbers(0,true);
    //setMarginWidth(0,QString("100"));
    //设置margin1显示折叠
    setFolding(QsciScintilla::BoxedTreeFoldStyle,1);
    setMarginWidth(1,15);

    //other margin
    setMarginsBackgroundColor(QColor("#CCCCCC"));
    setMarginsForegroundColor(QColor("#272727"));




//    //自动补全功能
//    apis=new QsciAPIs(textLexer);
//    if(apis->load("C:/AAStudy/QT/Test/QsciScintillaWidget/keyword.txt"))
//        apis->prepare();

    //设置自动补齐
    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionCaseSensitivity(true);
    setAutoCompletionThreshold(1);

    //设置文本框宽度
    setScrollWidth(10000);

    //设置tab缩进
    setIndentationsUseTabs(true);
    setIndentationWidth(4);
    setTabIndents(false);
    setAutoIndent(true);
    setBackspaceUnindents(true);
    setTabWidth(4);
    setIndentationGuides(true);

    //设置提示
    setCallTipsForegroundColor(QColor(Qt::white));
    setCallTipsHighlightColor(QColor(Qt::blue));
    setHotspotBackgroundColor(QColor(Qt::gray));
    setCallTipsStyle(QsciScintilla::CallTipsNoAutoCompletionContext);
    setWrapMode(QsciScintilla::WrapWhitespace);

    //设置所在行颜色
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor("#E8E8FF"));

    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    setMatchedBraceForegroundColor (QColor(Qt::red));

//    qDebug()<<"tab:"<<this->SendScintilla(SCI_SETEXTRAASCENT,SCTD_STRIKEOUT);
//    qDebug() << SendScintilla(SCI_SETWHITESPACECHARS," ");

    initCPPLexer();
}

void TextEdit::initCPPLexer()
{
     if(textLexer != NULL)
     {
         delete textLexer;
     }

    QsciLexerCPP* cpp = new QsciLexerCPP(this);
    textLexer=cpp;
    //设置cpp语法
//    cpp->setColor(QColor(Qt::green),QsciLexerCPP::PreProcessor);
//    cpp->setColor(QColor(Qt::blue),QsciLexerCPP::Keyword);
//    cpp->setColor(QColor(Qt::red),QsciLexerCPP::UnclosedString);
//    cpp->setColor(QColor("#800080"),QsciLexerCPP::UserLiteral);
//    cpp->setColor(QColor(Qt::darkGreen),QsciLexerCPP::TripleQuotedVerbatimString);
//    cpp->setColor(QColor(Qt::darkGreen),QsciLexerCPP::Comment);
//    cpp->setColor(QColor(Qt::darkGreen),QsciLexerCPP::HashQuotedString);
//    cpp->setColor(QColor(Qt::darkGreen),QsciLexerCPP::RawString);
//    cpp->setColor(QColor(Qt::darkGreen),QsciLexerCPP::PreProcessorComment);
//    cpp->setColor(QColor(Qt::darkGreen),QsciLexerCPP::CommentLine);
    cpp->setFoldPreprocessor(true);
    cpp->setStylePreprocessor(false);
    cpp->setFoldCompact(false);
    cpp->setFoldAtElse(true);
    cpp->setFoldComments(true);
    cpp->setAutoIndentStyle(QsciScintilla::AiClosing);

    setLexer(textLexer);
}

void TextEdit::initCS()
{
    if(textLexer != NULL)
    {
        delete textLexer;
    }
    textLexer=new QsciLexerCSharp(this);
    setLexer(textLexer);
}

void TextEdit::initJava()
{
    if(textLexer != NULL)
    {
        delete textLexer;
    }
    textLexer=new QsciLexerJava(this);
    setLexer(textLexer);
}

void TextEdit::initPython()
{
    if(textLexer != NULL)
    {
        delete textLexer;
    }
    textLexer=new QsciLexerPython(this);
    setLexer(textLexer);
}

void TextEdit::setTextFont(QFont font, const QString& line)
{
    textLexer->setFont(font,-1);

    setMarginWidth(0,line);
}

QString TextEdit::getFilePath()
{
    return filePath;
}

void TextEdit::setFilePath(QString path)
{
    filePath=path;
}

void TextEdit::wheelEvent(QWheelEvent *evt)
{
    if(evt->modifiers() == Qt::ControlModifier)
        evt->ignore();
    else
        QsciScintilla::wheelEvent(evt);

}

/*
void TextEdit::keyPressEvent(QKeyEvent* evt)
{
    QsciScintilla::keyPressEvent(evt);
}

void TextEdit::keyReleaseEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_BraceLeft)
    {
        insert("\n\r}");

        evt->accept();

    }
    else if(evt->key() == Qt::Key_ParenLeft)
    {
        insert(")");
        evt->accept();
    }
    else if(evt->key() == Qt::Key_QuoteDbl)
    {
        qDebug()<<"quote";
        evt->accept();
    }

    QsciScintilla::keyReleaseEvent(evt);
}*/

TextEdit::~TextEdit()
{
    //delete textLexer;//为什么这里delete不会报错
    //delete apis;
}
