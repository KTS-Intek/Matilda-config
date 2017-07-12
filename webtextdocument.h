#ifndef WEBTEXTDOCUMENT_H
#define WEBTEXTDOCUMENT_H

#include <QObject>
#include <QTextDocument>
#include <QTextBrowser>
#include <QStack>



class WebTextDocument : public QTextBrowser
{
    Q_OBJECT
public:
    explicit WebTextDocument(const bool setDefPaths = true, QWidget *parent = 0);
    void load(QUrl url);
    bool canGoBack();
    bool canGoForward();

    void setDefaultPaths();

    void setSearchPathsExt(const QStringList &paths, const QString &path2html, const QString &filePrefics);

    qreal zoomFactor();
    QString title();

    bool findTextInPage(const QString &text, const bool &caseSensitively, const bool findBackward = false);

signals:
    void loadStarted();
    void loadFinished(bool);
    void loadProgress(int);

    void linkActivated(QString link);

public slots:
    void back();
    void forwardHtml();

    void setZoomFactor(qreal zoom);

private:
    void loadHtml(QUrl url);

    QStack<QUrl> webStackBack, webStackForward;
    qreal zoomF, defValZoom;
    QString filePath;
    QTextDocument textDoc;

    QString clickedAnchor;

};

#endif // WEBTEXTDOCUMENT_H
