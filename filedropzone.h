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

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    QWidget* dropContainer;
    void addFile(QFile* aFile);
    QList<QFile*> files;
    int nbrFile;
    QWidget* scrollContent;
    QVBoxLayout *scrollLayout;
    void browseFile();
    bool isAllFilesAreCSV(const QList<QUrl> &urls) const;
    void updateDropContainerStyle();
};

#endif // FILEDROPZONE_H
