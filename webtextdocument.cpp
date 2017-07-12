#include "webtextdocument.h"
#include <QFile>
#include <QFileInfo>
#include <QtCore>
#include <QApplication>


//-----------------------------------------------------------------------------------
WebTextDocument::WebTextDocument(const bool setDefPaths, QWidget *parent) : QTextBrowser(parent)
{
    defValZoom = qApp->font().pointSizeF();
    zoomF = 1;

    if(setDefPaths)
        setDefaultPaths();


}
//-----------------------------------------------------------------------------------
void WebTextDocument::load(QUrl url)
{
    qDebug() << "load " << url << webStackBack.size() << defValZoom;

    webStackBack.push(url);
    webStackForward.clear();
    loadHtml(url);

}
//-----------------------------------------------------------------------------------
bool WebTextDocument::canGoBack()
{
    return !webStackBack.isEmpty();
}
//-----------------------------------------------------------------------------------
bool WebTextDocument::canGoForward()
{
    return !webStackForward.isEmpty();

}
//-----------------------------------------------------------------------------------
void WebTextDocument::setDefaultPaths()
{
    setSearchPathsExt(QString("cssFolder img_shared").split(" ", QString::SkipEmptyParts), qApp->applicationDirPath() + "/html", "file:///");
}
//-----------------------------------------------------------------------------------
void WebTextDocument::setSearchPathsExt(const QStringList &paths, const QString &path2html, const QString &filePrefics)
{
    QStringList searchPathList;
    filePath = path2html;
    for(int i = 0, iMax = paths.size(); i < iMax; i++)
        searchPathList.append(QString("%1/%2").arg(filePath).arg(paths.at(i)));

    setSearchPaths(searchPathList);
    filePath.prepend(filePrefics);
}
//-----------------------------------------------------------------------------------
qreal WebTextDocument::zoomFactor()
{
    return zoomF;
}
//-----------------------------------------------------------------------------------
QString WebTextDocument::title()
{
    return this->documentTitle();
}
//-----------------------------------------------------------------------------------
bool WebTextDocument::findTextInPage(const QString &text, const bool &caseSensitively, const bool findBackward)
{
    int ff = 0;
    if(caseSensitively)
        ff = (ff|QTextDocument::FindCaseSensitively);
    if(findBackward)
        ff = (ff|QTextDocument::FindBackward);

    QTextDocument::FindFlag flags = static_cast<QTextDocument::FindFlag>(ff);
    return find(text, flags);
}
//-----------------------------------------------------------------------------------
void WebTextDocument::back()
{
    if(!webStackBack.isEmpty()){
        qDebug() << "stack back " << webStackBack.last() << webStackBack.first();

        webStackForward.push( webStackBack.last());
        loadHtml(webStackBack.pop());
    }

}

void WebTextDocument::forwardHtml()
{
    qDebug() << "forward " << webStackForward.size();
    if(!webStackForward.isEmpty()){
        qDebug() << "top" << webStackForward.last();
        webStackBack.push(webStackForward.last());
        loadHtml(webStackForward.pop());
        if(!webStackForward.isEmpty())
            qDebug() << "top2" << webStackForward.last();

    }
}
//-----------------------------------------------------------------------------------
void WebTextDocument::setZoomFactor(qreal zoom)
{

    zoomF = zoom;
    QFont font = qApp->font();
    font.setPointSizeF(defValZoom * zoom);
    setFont(font);
}

//void WebTextDocument::checkLink(QString link)
//{
//    QDir dir(filePath);
//    if(link.contains("#"))
//        link = link.left(link.lastIndexOf("#"));
//    qDebug() << "link" << link;

//    bool add2stack = false;
//    if(link.contains("/")){
//        dir.cd(link.left(link.lastIndexOf("/")));

//        link = link.mid(link.lastIndexOf("/"));

//        qDebug() << "link2" << link;
//        add2stack = true;

//    }

//    if(link.isEmpty())
//        return;

//    if(link.left(1) != "/")
//        link.prepend("/");



//    if(add2stack)
//        load(QUrl("file:///" + dir.absolutePath() + link));
//    else
//        loadHtml(QUrl("file:///" + dir.absolutePath() + link));


//}


//-----------------------------------------------------------------------------------
void WebTextDocument::loadHtml(QUrl url)
{
    qDebug() << "loadHtml" << url.toLocalFile() << url.isValid() << url.isLocalFile() << url.toDisplayString() << url.toString();


    QString fileName = url.isLocalFile() ? url.toLocalFile() : url.toString();
    bool useAbsPaths = true;

    if(fileName.startsWith("qrc:/")){
        fileName = fileName.mid(3);
        useAbsPaths = false;
    }

    QFileInfo fi(fileName);

    filePath = fi.absolutePath();
    qDebug() << "loadHtml2" << filePath << fi.path() << useAbsPaths;

//    qDebug() << url.toString();

    QString lastlang = url.toLocalFile();
    lastlang = lastlang.mid(lastlang.lastIndexOf("/html/lang_"), 13 );

    //lang_en lang_en/img lang_ru lang_ru/img lang_ua lang_ua/img
    if(!lastlang.isEmpty()){
        QStringList listPahts = searchPaths();
        lastlang.prepend(qApp->applicationDirPath() + "/");

        qDebug()/* << url.toDisplayString()*/ << "html path " << lastlang;


        if(!listPahts.contains(lastlang)){
            for(int i = 0, iMax = listPahts.size(); i < iMax; i++){
                if(listPahts.at(i).contains("/html/lang_")){
                    listPahts.removeAt(i);
                    i--;
                    iMax--;
                }
            }

            listPahts.append(lastlang);
            listPahts.append(lastlang + "/img");
            setSearchPaths(listPahts);
        }
    }


    emit loadStarted();
    emit loadProgress(1);

    QFile file(fi.filePath());

    qDebug() << "fi.filePath " << fi.filePath() << fi.fileName();

    file.open(QFile::ReadOnly);

//    textDoc.setBaseUrl(url);// QUrl(filePath));// QUrl("file:///" + fi.absolutePath() + "/"));
//    textDoc.setHtml(file.readAll());

//    setBaseUrl(fi.absolutePath());
    setHtml(file.readAll());// textDoc.toHtml());// file.readAll());

    emit loadProgress(100);
    emit loadFinished(file.isOpen());
    file.close();
}
//-----------------------------------------------------------------------------------
