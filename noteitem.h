#ifndef NOTEITEM_H
#define NOTEITEM_H

#include <QtGUI>
#include <QtWebKit>
#include <QtNetwork>

#include "KompexSQLiteStatement.h"
using namespace Kompex;
class RendererReply : public QNetworkReply
{
    Q_OBJECT
public:
    RendererReply(QObject* object, const QNetworkRequest& request);
    qint64 readData(char* data, qint64 maxSize);
    virtual qint64 bytesAvailable() const;
    virtual qint64 pos () const;
    virtual bool seek( qint64 pos );
    virtual qint64 size () const;

public slots:
    void abort();
private:
    QByteArray m_buffer;
    int m_position;
};
class UrlBasedRenderer : public QNetworkAccessManager
{
public:
    UrlBasedRenderer(QObject* parent = 0) : QNetworkAccessManager(parent)
    {
    }

    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
    {
        if (request.url().scheme() != "wike")
            return QNetworkAccessManager::createRequest(op, request, outgoingData);
        return new RendererReply(this, request);
    }
};

class LocalFileDialog : public QObject {
    Q_OBJECT
public:
    LocalFileDialog(QWidget * parent = 0) : QObject(parent){ }
public slots:
    QString selectFiles(const QString& filters);
};
class PlainTextBrowser : public QPlainTextEdit {
    Q_OBJECT
public:
    PlainTextBrowser(QWidget * parent = 0);
    ~PlainTextBrowser();
protected:
    virtual void contextMenuEvent(QContextMenuEvent * event);
    QMenu* m_contextMenu;
};
class TextBrowser : public QTextBrowser {
    Q_OBJECT
public:
    TextBrowser(QWidget * parent = 0);
    ~TextBrowser();
protected:
    virtual void contextMenuEvent(QContextMenuEvent * event);
    QMenu* m_contextMenu;
};
class NoteItem : public QFrame
{
    Q_OBJECT
    public:
        NoteItem(QWidget *parent = 0, int row = 0, bool readOnly = true, bool rich = true);
        int getNoteId() const;
        int getGID() const;
        void setStatus(int status);
        int getStatus() const;
        QString getTitle();
        QString getContent();
        QStringList getTags();
        void setFont(const QFont& font);
        static NoteItem* getActiveItem();
        static void setActiveItem(NoteItem* item);
        void autoSize();
        void initControls();
        bool isReadOnly();
        bool isRich();
        QString selectedText();

    private:
        bool eventFilter(QObject *obj, QEvent *ev);
        bool shortCut(int k);
        static NoteItem* s_activeNote;
        SQLiteStatement* m_q;
        int m_noteId;
        int m_gid;
        int m_status;
        bool m_readOnly;
        bool m_rich;
        bool m_sized;
        int m_totalLine;

        QVBoxLayout * m_verticalLayout;
        QLabel *m_title;
        QString m_content;

        QLineEdit *m_titleEdit;
        QLineEdit *m_tagEdit;
        QPlainTextEdit* m_textEdit;
        QWebView* m_webView;

        void active();
        void loadResource();
        void exportFile();
        static UrlBasedRenderer s_urlBasedRenderer; 
};

#endif
