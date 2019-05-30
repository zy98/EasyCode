#include "AboutDialog.h"
#include <QPixmap>
#include <QPalette>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent,Qt::WindowCloseButtonHint),logoLbl(this),infoEdit(this),closeBtn(this)
{
    QPixmap mp(":/Res/Icon/logo.png");
    mp.scaled(130,130,Qt::IgnoreAspectRatio);

    logoLbl.setPixmap(mp);
    logoLbl.move(20,20);

    QPalette p=infoEdit.palette();
    p.setColor(QPalette::Active,QPalette::Base,palette().color(QPalette::Active,QPalette::Background));
    p.setColor(QPalette::Inactive,QPalette::Base,palette().color(QPalette::Inactive,QPalette::Background));

    infoEdit.resize(250,150);
    infoEdit.move(150,20);
    infoEdit.setReadOnly(true);
    infoEdit.setFrameStyle(QFrame::NoFrame);
    infoEdit.setPalette(p);
    infoEdit.insertPlainText("EasyCode Project\n\nPlatform: Qt 5.5.1\n\n"
                             "Version: 1.5.0\n\nCopyright: ZY.Software");

    closeBtn.setText(tr("Close"));
    closeBtn.move(310,200);

    setWindowTitle(tr("About EasyCode"));
    setFixedSize(430,250);
    connect(&closeBtn,SIGNAL(clicked()),this,SLOT(close()));

}
