#include "DocumentManager.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

DocumentManager::DocumentManager(QTextDocument* document, QObject* parent)
    : QObject(parent),
    m_document(document),
    m_format(DocumentFormat::PlainText)
{
}

bool DocumentManager::hasFile() const
{
    return !m_filePath.isEmpty();
}

QString DocumentManager::filePath() const
{
    return m_filePath;
}

//fetching document format from file extension
DocumentManager::DocumentFormat
DocumentManager::formatFromPath(const QString& path) const
{
    QFileInfo info(path);
    QString ext = info.suffix().toLower();

    if (ext == "rtf" || ext == "html" || ext == "htm")
        return DocumentFormat::RichText;

    return DocumentFormat::PlainText;
}

bool DocumentManager::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString data = in.readAll();

    m_format = formatFromPath(path);

    if (m_format == DocumentFormat::PlainText)
        m_document->setPlainText(data);
    else
        m_document->setHtml(data);

    m_filePath = path;
    m_document->setModified(false);

    return true;
}

bool DocumentManager::save()
{
    if (m_filePath.isEmpty())
        return false;

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    if (m_format == DocumentFormat::PlainText)
        out << m_document->toPlainText();
    else
        out << m_document->toHtml();

    m_document->setModified(false);
    return true;
}

bool DocumentManager::saveAs(const QString& path)
{
    QFileInfo info(path);
    QString finalPath = path;

    if (info.suffix().isEmpty()) {
        finalPath += ".txt";
    }

    m_format = formatFromPath(finalPath);
    m_filePath = finalPath;

    return save();
}
