#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QTextDocument>

class DocumentManager : public QObject
{
    Q_OBJECT

public:
    enum class DocumentFormat {
        PlainText,
        RichText
    };

    explicit DocumentManager(QTextDocument* document, QObject* parent = nullptr);

    bool openFile(const QString& path);
    bool save();
    bool saveAs(const QString& path);
    void createNew();

    bool hasFile() const;
    QString filePath() const;

private:
    DocumentFormat formatFromPath(const QString& path) const;

private:
    QTextDocument* m_document;
    QString m_filePath;
    DocumentFormat m_format;
};

#endif // DOCUMENTMANAGER_H
