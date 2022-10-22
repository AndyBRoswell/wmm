#include "TreeEditor.h"

#include <mutex>
#include <stdexcept>

#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QTextCodec>

#include "JSONFormatter.h"
#include "JSONHighlighter.h"
#include "TextArea.h"

#include "FileSystemAccessor.h"

namespace WritingMaterialsManager {
    const std::unordered_map<QByteArray, TreeEditor::SupportedFileType, CaseInsensitiveHasher, CaseInsensitiveStringComparator> TreeEditor::FileTypeToEnumID = {
        { "JSON",                  SupportedFileType::JSON },
        { "MongoDB Extended JSON", SupportedFileType::MongoDBExtendedJSON },
    }; // mainly for switch-case statement so far.

    TreeEditor::TreeEditor(const QByteArray& FileType, const std::shared_ptr<QtTreeModel>& TreeModel, QWidget* const parent) : QWidget(parent),
                                                                                                                               TabView(new QTabWidget),
                                                                                                                               IntuitiveView(new TreeView),
                                                                                                                               RawView(new TextArea),
                                                                                                                               TreeModel(TreeModel) {
        static std::once_flag StaticInitCompleted;
        std::call_once(StaticInitCompleted, [](){
            MenuAction::Open = new QAction(tr("打开"));
            MenuAction::Open->setShortcut(QKeySequence::Open);
            MenuAction::Open->setStatusTip(tr("打开一个文件"));

            Menu::Charset = new QMenu(tr("字符集"));
            auto AvailableCharsets = QTextCodec::availableCodecs();
            std::sort(AvailableCharsets.begin(), AvailableCharsets.end(), [](const QByteArray& A, const QByteArray& B) { return A < B; });
            for (const auto& Charset: AvailableCharsets) {
                MenuAction::SetCharset.emplace_back(new QAction(Charset));
                Menu::Charset->addAction(MenuAction::SetCharset.back());
            }
        });

        connect(IntuitiveView, &TreeView::MouseDown, this, &TreeEditor::ShouldUpdatePathName);
        connect(IntuitiveView, &TreeView::MouseDown, this, &TreeEditor::ShouldUpdateFileType);
        connect(IntuitiveView, &TreeView::MouseDown, this, &TreeEditor::ShouldUpdateCharset);
        connect(RawView, &TextArea::MouseDown, this, &TreeEditor::ShouldUpdatePathName);
        connect(RawView, &TextArea::MouseDown, this, &TreeEditor::ShouldUpdateFileType);
        connect(RawView, &TextArea::MouseDown, this, &TreeEditor::ShouldUpdateCharset);

        setFocusPolicy(Qt::StrongFocus);
        SetFileType(FileType);
        SetCharset("UTF-8");

        IntuitiveView->setModel(TreeModel.get());
        TabView->addTab(IntuitiveView, tr("直观"));
        TabView->addTab(RawView, tr("原始"));

        setLayout(new QGridLayout);
        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->addWidget(TabView);
    }

    TreeEditor::~TreeEditor() {}

    void TreeEditor::SetText(const QString& Text) { RawView->setPlainText(Text); }
    void TreeEditor::AppendText(const QString& Text) { RawView->appendPlainText(Text); }

    QByteArray TreeEditor::GetPathName() const { return PathName; }
    void TreeEditor::SetPathName(const QByteArray& FileName) {
        this->PathName = FileName;
        emit ShouldUpdatePathName();
    }

    QByteArray TreeEditor::GetFileType() const { return FileType; }
    void TreeEditor::SetFileType(const QByteArray& FileType) {
        using namespace std;
        using F = SupportedFileType;

        try {
            const auto I = FileTypeToEnumID.find(FileType);
            if (I == FileTypeToEnumID.cend()) return; // not supported file type
            switch (I->second) {
            case F::JSON: case F::MongoDBExtendedJSON:
                Formatter = make_shared<JSONFormatter>();
                Highlighter = make_shared<JSONHighlighter>(RawView->document());
                break;
            }
            this->FileType = I->first;
            emit ShouldUpdateFileType();
        }
        catch (const out_of_range& e) { qDebug() << "File type not supported:" << FileType; }
    }

    QByteArray TreeEditor::GetCharset() const { return Charset; }
    void TreeEditor::SetCharset() {
        this->Charset = static_cast<QAction*>(sender())->text().toUtf8();
        emit ShouldUpdateCharset();
    }
    void TreeEditor::SetCharset(const QByteArray& Charset) {
        this->Charset = Charset;
        emit ShouldUpdateCharset();
    }

    void TreeEditor::ArrangeContentView() {
        auto PlainText = RawView->toPlainText();
        auto PlainTextCopy = PlainText;
        Highlighter->setDocument(nullptr);
        try {
            Formatter->Format(PlainText);
            RawView->setPlainText(PlainText);
        }
        catch (...) {
            RawView->setPlainText(PlainTextCopy); // restore the original text
        }
        Highlighter->setDocument(RawView->document());
//        Highlighter->Highlight(Highlighter->document()->toPlainText());
//        RawView->update();
    }

    void TreeEditor::contextMenuEvent(QContextMenuEvent* const Event) {
        QMenu* const ContextMenu = new QMenu(this);
        ContextMenu->addAction(MenuAction::Open);
        const auto Connection = connect(MenuAction::Open, &QAction::triggered, this, qOverload<>(&TreeEditor::OpenFile));
        QList<QMetaObject::Connection> CharsetEventHandlerConnections;
        for (auto* const SetCharsetAction: MenuAction::SetCharset) {
            CharsetEventHandlerConnections.emplace_back(connect(SetCharsetAction, &QAction::triggered, this, qOverload<>(&TreeEditor::SetCharset)));
        }
        ContextMenu->addMenu(Menu::Charset);
        ContextMenu->exec(Event->globalPos());
        disconnect(Connection);
        for (const auto& Connection: CharsetEventHandlerConnections) disconnect(Connection);
    }

    void TreeEditor::OpenFile() {
        const QString FileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath(), tr("JSON (*.json)"));
        if (FileName.isEmpty() == false) OpenFile(FileName);
    }

    void TreeEditor::OpenFile(const QString& PathName) {
        using namespace std;

        shared_ptr<QFile> File = FileSystemAccessor::Open(PathName);
        shared_ptr<QFileInfo> FileInfo = FileSystemAccessor::GetFileInfo(File);
        SetPathName(PathName.toUtf8());
        SetFileType(FileInfo->suffix().toUtf8());
        QByteArray FileContentsUTF8;
        QString FileContentsUTF16;
        if (Charset == "<Charset>") Charset = "UTF-8"; // default encoding: UTF-8
        if (Charset == "UTF-8") {
            FileContentsUTF8 = FileSystemAccessor::GetAllRawContents(File);
            FileContentsUTF16 = QString::fromUtf8(FileContentsUTF8);
        }
        else if (Charset == "UTF-16") {
            QTextStream IFStream(File.get());
            IFStream.setEncoding(QStringConverter::Utf16);
            FileContentsUTF16 = IFStream.readAll();
            FileContentsUTF8 = FileContentsUTF16.toUtf8();
        }
        else {
            QTextCodec* const TextCodec = QTextCodec::codecForName(Charset);
            shared_ptr<QTextDecoder> TextDecoder(TextCodec->makeDecoder());
            const QByteArray FileContentsRaw = FileSystemAccessor::GetAllRawContents(File);
            FileContentsUTF16 = TextDecoder->toUnicode(FileContentsRaw);
            FileContentsUTF8 = FileContentsUTF16.toUtf8();
        }
        RawView->setPlainText(FileContentsUTF16);
        TreeModel->FromJSON(FileContentsUTF8);
    }
} // namespace WritingMaterialsManager
