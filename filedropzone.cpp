#include "filedropzone.h"
#include <QStyle>
#include <QPushButton>
#include <QFileDialog>

FileDropZone::FileDropZone(QWidget* parent) : QWidget(parent) {
    setAcceptDrops(true);
    setFixedWidth(550);
    setFixedHeight(200);

    dropContainer = new QWidget(this);
    dropContainer->setObjectName("dropContainer");
    dropContainer->setProperty("isHovered", false);
    QLabel *label = new QLabel("Drag and drop files here", dropContainer);
    label->setAlignment(Qt::AlignCenter);
    label->setObjectName("text");
    QPushButton* browseFileBtn = new QPushButton("Browse file", dropContainer);
    connect(browseFileBtn, &QPushButton::clicked, this, &FileDropZone::browseFile);
    QVBoxLayout *dropLayout = new QVBoxLayout(dropContainer);
    dropLayout->addWidget(label);
    dropLayout->addWidget(browseFileBtn);

    QWidget* fileContainer = new QWidget(this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileContainer);

    QScrollArea* scrollArea = new QScrollArea(fileContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("scrollArea");

    scrollContent = new QWidget(scrollArea);
    scrollContent->setObjectName("scrollContent");
    scrollLayout = new QVBoxLayout(scrollContent);

    scrollArea->setWidget(scrollContent);

    fileLayout->addWidget(scrollArea);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(dropContainer);
    layout->addWidget(fileContainer);
    layout->setContentsMargins(0, 0, 0, 0);
}


void FileDropZone::dragEnterEvent(QDragEnterEvent* event){
    if (event->mimeData()->hasUrls() && isAllFilesAreCSV(event->mimeData()->urls())){
        event->acceptProposedAction();
        dropContainer->setProperty("isHovered", true);
        dropContainer->style()->unpolish(dropContainer);
        dropContainer->style()->polish(dropContainer);
        update();
    }else {
        event->ignore();
    }
}

void FileDropZone::dragLeaveEvent(QDragLeaveEvent* event){
    dropContainer->setProperty("isHovered", false);
    updateDropContainerStyle();
}

void FileDropZone::dropEvent(QDropEvent* event){
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            qDebug()<< filePath;
            QFile* aFile = new QFile(filePath);
            addFile(aFile);
            dropContainer->setProperty("isHovered", false);
            updateDropContainerStyle();
        }
        event->acceptProposedAction();
    }
}

void FileDropZone::updateDropContainerStyle() {
    dropContainer->style()->unpolish(dropContainer);
    dropContainer->style()->polish(dropContainer);
    update();
}

bool FileDropZone::isAllFilesAreCSV(const QList<QUrl> &urls) const
{
    for (const QUrl &url : urls) {
        if (!url.toLocalFile().endsWith(".csv", Qt::CaseInsensitive))
            return false;
    }
    return true;
}

void FileDropZone::browseFile()
{
    QString filter = "CSV file (*.csv)";
    QStringList file_names = QFileDialog::getOpenFileNames(this, "Open file", "C:/Users/user/Downloads", filter);
    foreach(const QString &file_name, file_names){
        if (!file_name.isEmpty()) {
            QFile* aFile = new QFile(file_name);
            addFile(aFile);
        }
    }
}

void FileDropZone::addFile(QFile* aFile)
{
    bool isNewFile = true;

    // Avoid duplicate file
    foreach(QFile* file, files)
    {
        if(file->fileName() == aFile->fileName())
            isNewFile = false;
    }

    if(isNewFile)
    {
        nbrFile++;
        QWidget *fileWidget = new QWidget(scrollContent);
        fileWidget->setObjectName("fileWidget");
        fileWidget->setFixedHeight(40);
        QHBoxLayout *fileLayout = new QHBoxLayout(fileWidget);

        // Button for removing file from files list
        QPushButton *fileBtn = new QPushButton("X", fileWidget);
        fileBtn->setFixedWidth(20);
        fileBtn->setObjectName("fileButton");
        connect(fileBtn, &QPushButton::clicked, this, [=]() {
            delete fileWidget;
            nbrFile--;

            //remove the file from the list
            int index = files.indexOf(aFile);
            if (index != -1) {
                delete files.takeAt(index);
            }
        });

        //remove the extension from the file name and reduce the number of characters to 18
        QString fileName = aFile->fileName().section('/', -1);
        QLabel *fileLabel = new QLabel(fileName, fileWidget);
        fileLabel->setObjectName("fileLabel");
        for(int i = 0; i<fileLabel->text().length(); i++){
            if(fileLabel->text()[i] == '.')
            {
                fileLabel->setText(fileLabel->text().left(i));
                break;
            }
        }
        if (fileLabel->text().length() > 18)
            fileLabel->setText(fileLabel->text().left(18));

        fileLayout->addWidget(fileBtn);
        fileLayout->addWidget(fileLabel);
        scrollLayout->addWidget(fileWidget);
        files.append(aFile);
    }
}
