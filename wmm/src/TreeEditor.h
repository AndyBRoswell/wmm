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
    class TreeEditor : public QWidget {
    Q_OBJECT
    public:
        enum class SupportedFileType : size_t {
            JSON = 1,
            MongoDBExtendedJSON = 2,
        };

        QTabWidget* const TabView;
        TreeView* const IntuitiveView;
        TextArea* const RawView;

        explicit TreeEditor(const QByteArray& FileType = "<File Type>", const std::shared_ptr<QtTreeModel>& TreeModel = std::make_shared<QtTreeModel>(), QWidget* const parent = nullptr);
        ~TreeEditor();

        void SetText(const QString& Text = {});
        void AppendText(const QString& Text = {});

    signals:
        void ShouldUpdatePathName();
        void ShouldUpdateFileType();
        void ShouldUpdateCharset();
    public slots:
        void ArrangeContentView();

        QByteArray GetPathName() const;
        void SetPathName(const QByteArray& FileName);
        QByteArray GetFileType() const;
        void SetFileType(const QByteArray& FileType);
        QByteArray GetCharset() const;
        void SetCharset(); // This slot is for QAction::triggered()
        void SetCharset(const QByteArray& Charset);
    protected:
        void contextMenuEvent(QContextMenuEvent* const Event) override;
    private:
        static const std::unordered_map<QByteArray, SupportedFileType, CaseInsensitiveHasher, CaseInsensitiveStringComparator> FileTypeToEnumID; // mainly for switch-case statement so far.
        struct Menu {
            inline static QMenu* Charset;
            Menu() = delete;
            Menu(const Menu&) = delete;
            Menu(Menu&&) = delete;
            Menu& operator=(const Menu&) = delete;
            Menu& operator=(Menu&&) = delete;
        };
        struct MenuAction {
            inline static QAction* Open;
            inline static QList<QAction*> SetCharset;
            MenuAction() = delete;
            MenuAction(const MenuAction&) = delete;
            MenuAction(MenuAction&&) = delete;
            MenuAction& operator=(const MenuAction&) = delete;
            MenuAction& operator=(MenuAction&&) = delete;
        };

        QByteArray PathName{};
        QByteArray FileType{};
        QByteArray Charset{};
        std::shared_ptr<TextFormatter> Formatter;
        std::shared_ptr<TextHighlighter> Highlighter;
        std::shared_ptr<QtTreeModel> TreeModel;
    private slots:
        void OpenFile();
        void OpenFile(const QString& PathName);
    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
