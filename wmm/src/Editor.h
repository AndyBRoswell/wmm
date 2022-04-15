#ifndef WRITING_MATERIALS_MANAGER_EDITOR_H
#define WRITING_MATERIALS_MANAGER_EDITOR_H

#include <unordered_map>

#include <QFont>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>

#include "QtTreeModel.h"
#include "TextFormatter.h"

namespace WritingMaterialsManager {
    class Editor : public QWidget {
    Q_OBJECT
    public:
        enum class SupportedFileType : size_t {
            JSON = 1,
        };

        inline static const QFont DefaultFont{ "Consolas", 10 };

        QTabWidget* const TabView;
        QTreeView* const IntuitiveView;
        QPlainTextEdit* const RawView;

        explicit Editor(const QString& FileType = "JSON", const std::shared_ptr<QtTreeModel>& TreeModel = nullptr, QWidget* const parent = nullptr);
        ~Editor();

        void SetText(const QString& Text = {});
        void AppendText(const QString& Text = {});

        void ModifyFileType(const QString& FileType);
    public slots:
        void ArrangeContentView();
    signals:
        void NoMoreReturn();
    private:
        inline static const std::unordered_map<QString, SupportedFileType> SupportedFileInternalID = {
            { "JSON", SupportedFileType::JSON },
        };

        QString FileType;
        std::shared_ptr<TextFormatter> Formatter;
        std::shared_ptr<QSyntaxHighlighter> Highlighter;
        std::shared_ptr<QtTreeModel> TreeModel;
    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
