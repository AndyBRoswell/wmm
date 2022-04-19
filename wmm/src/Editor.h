#ifndef WRITING_MATERIALS_MANAGER_EDITOR_H
#define WRITING_MATERIALS_MANAGER_EDITOR_H

#include <unordered_map>

#include <QFont>
#include <QMenu>
#include <QSyntaxHighlighter>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>

#include <Algorithm.h>
#include "QtTreeModel.h"
#include "TextArea.h"
#include "TextFormatter.h"
#include "TextHighlighter.h"
#include "TreeView.h"

namespace WritingMaterialsManager {
    class Editor : public QWidget {
    Q_OBJECT
    public:
        enum class SupportedFileType : size_t {
            JSON = 1,
            MongoDBExtendedJSON = 2,
        };

        inline static const QFont DefaultFont{ "Consolas", 10 };

        QTabWidget* const TabView;
        TreeView* const IntuitiveView;
        TextArea* const RawView;

        static void OneOffInit();
        explicit Editor(const QString& FileType = "<File Type>", const std::shared_ptr<QtTreeModel>& TreeModel = std::make_shared<QtTreeModel>(), QWidget* const parent = nullptr);
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
        void focusInEvent(QFocusEvent* Event) override;
    private:
        static const std::unordered_map<QString, SupportedFileType, CaseInsensitiveHasher, CaseInsensitiveStringComparator> FileTypeToEnumID; // mainly for switch-case statement so far.
        struct MenuAction {
            inline static QAction* Open;
            MenuAction() = delete;
            MenuAction(const MenuAction&) = delete;
            MenuAction(MenuAction&&) = delete;
            MenuAction& operator=(const MenuAction&) = delete;
            MenuAction& operator=(MenuAction&&) = delete;
        };

        QString FileType;
        QString Encoding;
        std::shared_ptr<TextFormatter> Formatter;
        std::shared_ptr<TextHighlighter> Highlighter;
        std::shared_ptr<QtTreeModel> TreeModel;
    private slots:
        void OpenFile();
        void OpenFile(const QString& FileName);
    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
