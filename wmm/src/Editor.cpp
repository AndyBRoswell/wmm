#include "Editor.h"

#include <mutex>
#include <stdexcept>

#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>

#include "JSONFormatter.h"
#include "JSONHighlighter.h"
#include "TextArea.h"

#include "global.h"

namespace WritingMaterialsManager {
    const std::unordered_map<QString, Editor::SupportedFileType, CaseInsensitiveHasher, CaseInsensitiveStringComparator> Editor::FileTypeToEnumID = {
        { "JSON",                  SupportedFileType::JSON },
        { "MongoDB Extended JSON", SupportedFileType::MongoDBExtendedJSON },
    }; // mainly for switch-case statement so far.

    void Editor::OneOffInit() {
        MenuAction::Open = new QAction(tr("打开"));
        MenuAction::Open->setShortcut(QKeySequence::Open);
        MenuAction::Open->setStatusTip(tr("打开一个文件"));
    }

    Editor::Editor(const QString& FileType, const std::shared_ptr<QtTreeModel>& TreeModel, QWidget* const parent) : QWidget(parent),
                                                                                                                    TabView(new QTabWidget),
                                                                                                                    IntuitiveView(new TreeView),
                                                                                                                    RawView(new TextArea),
                                                                                                                    TreeModel(TreeModel) {
        static std::once_flag StaticInitCompleted;
        std::call_once(StaticInitCompleted, OneOffInit);

        connect(IntuitiveView, &TreeView::MouseDown, this, &Editor::ShouldUpdateFileType);
        connect(IntuitiveView, &TreeView::MouseDown, this, &Editor::ShouldUpdateCharset);
        connect(RawView, &TextArea::MouseDown, this, &Editor::ShouldUpdateFileType);
        connect(RawView, &TextArea::MouseDown, this, &Editor::ShouldUpdateCharset);

        setFocusPolicy(Qt::StrongFocus);
        SetFileType(FileType);
        SetCharset("UTF-8");

        RawView->setFont(DefaultCodeFont);

        IntuitiveView->setModel(TreeModel.get());
        TabView->addTab(IntuitiveView, tr("直观"));
        TabView->addTab(RawView, tr("原始"));

        setLayout(new QGridLayout);
        layout()->setContentsMargins(0, 0, 0, 0);
        layout()->addWidget(TabView);
    }

    Editor::~Editor() {}

    void Editor::SetText(const QString& Text) { RawView->setPlainText(Text); }
    void Editor::AppendText(const QString& Text) { RawView->appendPlainText(Text); }

    QString Editor::GetFileType() const { return FileType; }
    void Editor::SetFileType(const QString& FileType) {
        using namespace std;
        using F = SupportedFileType;

        try {
            const auto I = FileTypeToEnumID.find(FileType);
            if (I == FileTypeToEnumID.cend()) return; // not supported file type
            switch (I->second) {
            case F::JSON:
            case F::MongoDBExtendedJSON:
                Formatter = make_shared<JSONFormatter>();
                Highlighter = make_shared<JSONHighlighter>(RawView->document());
                break;
            }
            this->FileType = I->first;
            emit ShouldUpdateFileType();
        }
        catch (const out_of_range& e) { qDebug() << "File type not supported:" << FileType; }
    }

    QString Editor::GetCharset() const { return Charset; }
    void Editor::SetCharset(const QString& Charset) {
        this->Charset = Charset;
        emit ShouldUpdateCharset();
    }

    void Editor::ArrangeContentView() {
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

    void Editor::contextMenuEvent(QContextMenuEvent* Event) {
        QMenu* const ContextMenu = new QMenu(this);
        ContextMenu->addAction(MenuAction::Open);
        const auto Connection = connect(MenuAction::Open, &QAction::triggered, this, qOverload<>(&Editor::OpenFile));
        ContextMenu->exec(Event->globalPos());
        disconnect(Connection);
    }

    void Editor::focusInEvent(QFocusEvent* Event) {
        emit ShouldUpdateFileType();
        emit ShouldUpdateCharset();
    }

    void Editor::OpenFile() {
        const QString FileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QDir::currentPath(), tr("JSON (*.json)"));
        if (FileName.isEmpty() == false) OpenFile(FileName);
    }

    void Editor::OpenFile(const QString& FileName) {
        using namespace std;

        QFile File(FileName);
        if (File.open(QIODevice::ReadWrite) == false) {
            throw std::runtime_error(("Open file " + FileName + " failed.").toUtf8().constData());
        }
        QFileInfo FileInfo(File);
        SetFileType(FileInfo.suffix());
        const QByteArray FileContents = File.readAll();
        RawView->setPlainText(FileContents);
        TreeModel->FromJSON(FileContents);
    }
} // namespace WritingMaterialsManager
