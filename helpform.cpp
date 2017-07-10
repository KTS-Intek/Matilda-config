#include "helpform.h"
#include "ui_helpform.h"
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QWebEngineHistory>
#include "settloader.h"
#include <QSaveFile>


//--------------------------------------------------------------
HelpForm::HelpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpForm)
{
    ui->setupUi(this);


    myWeb = new QWebEngineView;

    timerDec.setInterval(350);
    timerInc.setInterval(350);

    ui->vltWeb->addWidget(myWeb);

    connect(ui->pbBack, SIGNAL(clicked()), myWeb, SLOT(back()));
    connect(ui->pbForward, SIGNAL(clicked()), myWeb,SLOT(forward()));

    connect(ui->pbFontDec, SIGNAL(clicked()), this, SLOT(zoomDec()));
    connect(ui->pbFontInc, SIGNAL(clicked()), this, SLOT(zoomInc()));
    connect(ui->pbFontNorm, SIGNAL(clicked()), this, SLOT(zoomNorm()));

    connect(ui->pbFontDec, SIGNAL(pressed()), &timerDec, SLOT(start()));
    connect(ui->pbFontInc, SIGNAL(pressed()), &timerInc, SLOT(start()));

    connect(ui->pbFontDec,SIGNAL(released()), &timerDec, SLOT(stop()));
    connect(ui->pbFontInc,SIGNAL(released()), &timerInc, SLOT(stop()));

    connect(&timerDec,SIGNAL(timeout()), this, SLOT(zoomDec()));
    connect(&timerInc, SIGNAL(timeout()), this, SLOT(zoomInc()));

    connect(myWeb, SIGNAL(loadFinished(bool)), SLOT(slotFinished(bool)));





//    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)));

    loadAvLang();
    loadSettings();




    ui->pbFontNorm->setToolTip(tr("x1"));
    QTimer::singleShot(50, this, SLOT(zoomChanged()));
  /*  QTreeWidgetItem *vykor = new QTreeWidgetItem(myItem);
    vykor->setText(0, "Использование");*/

    reloadLangSett();
    connect(ui->cbLang, SIGNAL(currentIndexChanged(int)), SLOT(reloadLangSett()) );


}
//--------------------------------------------------------------
HelpForm::~HelpForm()
{
    saveSettings();
    delete ui;
}
//--------------------------------------------------------------
void HelpForm::zoomDec()
{
    myWeb->setZoomFactor(myWeb->zoomFactor()-0.05);
    zoomChanged();
}
//--------------------------------------------------------------
void HelpForm::zoomInc()
{
    myWeb->setZoomFactor(myWeb->zoomFactor()+0.05);
    zoomChanged();
}
//--------------------------------------------------------------
void HelpForm::zoomNorm()
{
    myWeb->setZoomFactor(1);
     zoomChanged();
}
//--------------------------------------------------------------
void HelpForm::slotFinished(bool bOk)
{

    if (!bOk) {
        myWeb->setHtml(tr("<CENTER>An error has occured while loading the web page</CENTER>"));
    }



    ui->pbBack->setEnabled(myWeb->page()->history()->canGoBack());
    if(ui->pbBack->isEnabled())
         ui->pbBack->setToolTip(myWeb->page()->history()->backItem().title());
    else
        ui->pbBack->setToolTip(tr("Back"));

    ui->pbForward->setEnabled(myWeb->page()->history()->canGoForward());
    if(ui->pbForward->isEnabled())
         ui->pbForward->setToolTip(myWeb->page()->history()->forwardItem().title());
    else
        ui->pbForward->setToolTip( tr("Forward"));


    qDebug() << "load finished " << myWeb->url().toString();
}

//--------------------------------------------------------------
void HelpForm::zoomChanged()
{
    ui->pbFontDec->setToolTip(tr("x%1").arg(myWeb->zoomFactor()-0.05));
    ui->pbFontInc->setToolTip(tr("x%1").arg(myWeb->zoomFactor()+0.05));
}


//--------------------------------------------------------------
void HelpForm::loadSettings()
{
    QString lang = SettLoader::loadSett(SETT_MAIN_CURRLANG).toString();
    ui->cbLang->setCurrentIndex(ui->cbLang->findText(lang));
    if(ui->cbLang->currentIndex() < 0 && ui->cbLang->count() > 0)
        ui->cbLang->setCurrentIndex(0);
}
//--------------------------------------------------------------
void HelpForm::saveSettings()
{

}
//--------------------------------------------------------------
void HelpForm::on_goHome_clicked()
{
    //:/help/help-uk.html
    myWeb->load(QUrl(  QString("qrc:/help/help-%1.html").arg(ui->cbLang->currentText()) ));

}
//--------------------------------------------------------------
void HelpForm::loadAvLang()
{
    QString pathToDir = ":/help";

//    qDebug() << pathToDir;
    QDir dir(pathToDir);
    QStringList dirNames = dir.entryList(QStringList("help*"));

    for(int i = 0, iMax = dirNames.size(); i < iMax; i++){

        QString s = dirNames.at(i);
        if(s.right(5) != ".html")
            continue;

        s = s.split("-").last();
        s.chop(5);

        ui->cbLang->addItem(s);
    }
}

//--------------------------------------------------------------
void HelpForm::reloadLangSett()
{
    on_goHome_clicked();
}
//--------------------------------------------------------------

void HelpForm::on_toolButton_6_clicked()
{
    if(ui->checkBox->isChecked())
        myWeb->findText(ui->leFindPteLog_2->text(), QWebEnginePage::FindCaseSensitively);
    else
        myWeb->findText(ui->leFindPteLog_2->text());

}

void HelpForm::on_toolButton_4_clicked()
{
    QWebEnginePage::FindFlag flags;
    int ff = (QWebEnginePage::FindCaseSensitively|QWebEnginePage::FindBackward);
    flags = static_cast<QWebEnginePage::FindFlag>(ff);
    if(ui->checkBox->isChecked())
        myWeb->findText(ui->leFindPteLog_2->text(), flags);
    else
        myWeb->findText(ui->leFindPteLog_2->text(), QWebEnginePage::FindBackward);


}

void HelpForm::on_pushButton_clicked()
{
//    QProcess::startDetached(QString("qrc:/help/help-%1.").arg(ui->cbLang->currentText()) );
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QString("/help-%1.pdf").arg(ui->cbLang->currentText());
    QString sourceFileName = QString(":/help/help-%1.pdf").arg(ui->cbLang->currentText());
    int sourceFileSize = 0;
    if(true){
        QFileInfo fi(sourceFileName);
        sourceFileSize = fi.size();
    }

    QFileInfo fi(fileName);
    if(!fi.exists() || fi.size() < 1000 || fi.size() != sourceFileSize){
        QSaveFile sfile(fileName);

        QFile file(sourceFileName);

        if(sfile.open(QSaveFile::WriteOnly|QSaveFile::Unbuffered)){
            file.open(QFile::ReadOnly);
            sfile.write(file.readAll());
            sfile.commit();
            file.close();
        }else{
            qDebug() << "can't write sfile " << sfile.errorString();
        }
    }
    if(fileName.startsWith("/"))
        fileName = fileName.mid(1);
    fileName.prepend("file:///");

    QDesktopServices::openUrl(QUrl(fileName ));
}
