#include "Editor.h"

#include <QGridLayout>

#include "JSONFormatter.h"

namespace WritingMaterialsManager {
    Editor::Editor(const QString& FileType, const std::shared_ptr<QtTreeModel>& TreeModel, QWidget* const parent) : QWidget(parent),
                                                                                                                    TabView(new QTabWidget),
                                                                                                                    IntuitiveView(new QTreeView),
                                                                                                                    RawView(new QPlainTextEdit),
                                                                                                                    TreeModel(TreeModel) {
        ModifyFileType(FileType);

        RawView->setFont(DefaultFont);

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

    void Editor::ModifyFileType(const QString& FileType) {
        using namespace std;
        using F = SupportedFileType;

        try {
            switch (SupportedFileInternalID.at(FileType)) {
            case F::JSON:
                Formatter = make_shared<JSONFormatter>();
                Highlighter = make_shared<JSONHighlighter>();
                break;
            }
        }
        catch (const out_of_range& e) { qDebug() << "File type not supported:" << FileType; }
    }
} // namespace WritingMaterialsManager
