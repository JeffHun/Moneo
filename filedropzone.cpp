#include "filedropzone.h"
#include "buttonutility.h"
#include <QStyle>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

FileDropZone::FileDropZone(QWidget* parent) : QWidget(parent) {
    setAcceptDrops(true);
    setFixedWidth(550);
    setFixedHeight(200);

    m_dropContainer = new QWidget(this);
    m_dropContainer->setObjectName("dropContainer");
    m_dropContainer->setProperty("isHovered", false);
    QLabel *label = new QLabel("Drag and drop files here", m_dropContainer);
    label->setAlignment(Qt::AlignCenter);
    label->setObjectName("text");
    QPushButton* browseFileBtn = new QPushButton("Browse file", m_dropContainer);
    ButtonUtility::connectButton(browseFileBtn);
    connect(browseFileBtn, &QPushButton::clicked, this, &FileDropZone::browseFile);
    QVBoxLayout *dropLayout = new QVBoxLayout(m_dropContainer);
    dropLayout->addStretch();
    dropLayout->addWidget(label);
    dropLayout->addStretch();
    dropLayout->addWidget(browseFileBtn);
    dropLayout->addStretch();

    fileContainer = new QWidget(this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileContainer);

    QScrollArea* scrollArea = new QScrollArea(fileContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("scrollArea");

    m_scrollContent = new QWidget(scrollArea);
    m_scrollContent->setObjectName("scrollContent");
    m_scrollLayout = new QVBoxLayout(m_scrollContent);

    scrollArea->setWidget(m_scrollContent);

    fileLayout->addWidget(scrollArea);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_dropContainer);
    layout->addWidget(fileContainer);
    layout->setContentsMargins(0, 0, 0, 0);

    fileContainer->hide();
}


void FileDropZone::dragEnterEvent(QDragEnterEvent* event){
    if (event->mimeData()->hasUrls() && isAllFilesAreCSV(event->mimeData()->urls())){
        event->acceptProposedAction();
        m_dropContainer->setProperty("isHovered", true);
        m_dropContainer->style()->unpolish(m_dropContainer);
        m_dropContainer->style()->polish(m_dropContainer);
        update();
    }else {
        event->ignore();
    }
}

void FileDropZone::dragLeaveEvent(QDragLeaveEvent* event){
    m_dropContainer->setProperty("isHovered", false);
    updateDropContainerStyle();
}

void FileDropZone::dropEvent(QDropEvent* event){
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urls = event->mimeData()->urls();
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            QFile* aFile = new QFile(filePath);
            addFile(aFile);
            m_dropContainer->setProperty("isHovered", false);
            updateDropContainerStyle();
        }
        event->acceptProposedAction();
    }
}

void FileDropZone::updateDropContainerStyle() {
    m_dropContainer->style()->unpolish(m_dropContainer);
    m_dropContainer->style()->polish(m_dropContainer);
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
    fileContainer->show();

    QMessageBox messageBox;
    messageBox.setFixedSize(500,200);

    // Check if file exist
    if (!aFile->exists()) {
        messageBox.critical(0,"Error", aFile->fileName() + " doesn't exist.");
        delete aFile;
        return;
    }

    // Avoid duplicate file
    foreach(QFile* file, m_files)
    {
        if(file->fileName() == aFile->fileName())
        {
            messageBox.critical(0,"Warning", aFile->fileName() + " is not added because it already exists in the processing files list.");
            delete aFile;
            return;
        }
    }
    QWidget *fileWidget = new QWidget(m_scrollContent);
    fileWidget->setObjectName("fileWidget");
    fileWidget->setFixedHeight(40);
    QHBoxLayout *fileLayout = new QHBoxLayout(fileWidget);

    // Button for removing file from files list
    QPushButton *fileBtn = new QPushButton("X", fileWidget);
    fileBtn->setFixedWidth(20);
    fileBtn->setObjectName("fileButton");
    connect(fileBtn, &QPushButton::clicked, this, [this, fileWidget, aFile]() {
        delete fileWidget;

        //remove the file from the list
        int index = m_files.indexOf(aFile);
        if (index != -1)
            delete m_files.takeAt(index);

        if(m_files.empty())
            fileContainer->hide();
        else
            fileContainer->show();
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
    if (fileLabel->text().length() > 40)
        fileLabel->setText(fileLabel->text().left(40));

    fileLayout->addWidget(fileBtn);
    fileLayout->addWidget(fileLabel);
    m_scrollLayout->addWidget(fileWidget);
    m_files.append(aFile);
}

QList<QFile*> FileDropZone::getFiles(){
    return m_files;
}

FileDropZone::~FileDropZone()
{
    for(QFile* file: m_files)
        delete file;
}
