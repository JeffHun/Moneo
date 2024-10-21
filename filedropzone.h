#ifndef FILEDROPZONE_H
#define FILEDROPZONE_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QScrollArea>

class FileDropZone : public QWidget{
    Q_OBJECT

public:
    FileDropZone(QWidget* parent = nullptr);
    QList<QFile*> getFiles();
    ~FileDropZone();

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    QWidget* m_dropContainer;
    QList<QFile*> m_files;
    QWidget* m_scrollContent;
    QVBoxLayout* m_scrollLayout;
    QWidget* fileContainer;

    void addFile(QFile* aFile);
    void browseFile();
    bool isAllFilesAreCSV(const QList<QUrl> &urls) const;
    void updateDropContainerStyle();
};

#endif // FILEDROPZONE_H
