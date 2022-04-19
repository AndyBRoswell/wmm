#ifndef WRITING_MATERIALS_MANAGER_EDITOR_H
#define WRITING_MATERIALS_MANAGER_EDITOR_H

#include <unordered_map>

#include <QFont>
#include <QMenu>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>

#include <Algorithm.h>
#include "QtTreeModel.h"
#include "TextFormatter.h"
#include "TextHighlighter.h"

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

        explicit Editor(const QString& FileType = "JSON", const std::shared_ptr<QtTreeModel>& TreeModel = std::make_shared<QtTreeModel>(), QWidget* const parent = nullptr);
        ~Editor();

        void SetText(const QString& Text = {});
        void AppendText(const QString& Text = {});

        QString GetFileType() const;
        void SetFileType(const QString& FileType);
        QString GetEncoding() const;
        void SetEncoding(const QString& Encoding);
    signals:
        void ShouldUpdateFileType();
        void ShouldUpdateEncoding();
    public slots:
        void ArrangeContentView();
    signals:
        void NoMoreReturn();
    protected:
        void contextMenuEvent(QContextMenuEvent* Event) override;
    private:
        static const std::unordered_map<QString, SupportedFileType, CaseInsensitiveHasher, CaseInsensitiveStringComparator> FileTypeToEnumID; // mainly for switch-case statement so far.
        struct {
            QAction* Open{ new QAction(tr("打开")) };
        } MenuAction;

        QString FileType;
        QString Encoding = "UTF-8";
        std::shared_ptr<TextFormatter> Formatter;
        std::shared_ptr<TextHighlighter> Highlighter;
        std::shared_ptr<QtTreeModel> TreeModel;
    private slots:
        void OpenFile();
        void OpenFile(const QString& FileName);
    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
