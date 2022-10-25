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
        enum class SupportedFileType : size_t { // file types supported by the tree editor
            JSON = 1,
            MongoDBExtendedJSON = 2,
        };

        QTabWidget* const TabView; // the main tab widget containing IntuitiveView and RawView
        TreeView* const IntuitiveView; // show the tree structure of the open JSON
        TextArea* const RawView; // show the raw content of the open JSON

        explicit TreeEditor(const QByteArray& FileType = "<File Type>", const std::shared_ptr<QtTreeModel>& TreeModel = std::make_shared<QtTreeModel>(), QWidget* const parent = nullptr);
        ~TreeEditor();

        QString GetText();
        void SetText(const QString& Text = {}); // set content for this tree editor
        void AppendText(const QString& Text = {}); // append content for this tree editor

    signals:
        void ShouldUpdatePathName();
        void ShouldUpdateFileType();
        void ShouldUpdateCharset();
    public slots:
        void ArrangeContentView(); // format & highlight the displaying content
        void OpenFile(); // open a file and show its content using both IntuitiveView and RawView in this tree editor
        void OpenFile(const QString& PathName);

        QByteArray GetPathName() const; // get the pathname of the open file of this tree editor
        void SetPathName(const QByteArray& FileName); // set the pathname of this tree editor as the pathname of the open file
        QByteArray GetFileType() const; // get the extension of the open file of this tree editor
        void SetFileType(const QByteArray& FileType); // set the file type of this tree editor as the extension of the open file so as to perform the proper operations
        QByteArray GetCharset() const;
        void SetCharset(); // This slot is for QAction::triggered()
        void SetCharset(const QByteArray& Charset); // set the charset of this tree editor as the proper charset for appropriately reading the content of the open file
    protected:
        void contextMenuEvent(QContextMenuEvent* const Event) override; // context menu event handler
    private:
        static const std::unordered_map<QByteArray, SupportedFileType, CaseInsensitiveHasher, CaseInsensitiveStringComparator> FileTypeToEnumID; // mainly for switch-case statement so far.
        struct Menu { // menu items
            inline static QMenu* Charset; // charset menu item
            Menu() = delete;
            Menu(const Menu&) = delete;
            Menu(Menu&&) = delete;
            Menu& operator=(const Menu&) = delete;
            Menu& operator=(Menu&&) = delete;
        };
        struct MenuAction { // actions of menu items
            inline static QAction* Open;
            inline static QList<QAction*> SetCharset;
            MenuAction() = delete;
            MenuAction(const MenuAction&) = delete;
            MenuAction(MenuAction&&) = delete;
            MenuAction& operator=(const MenuAction&) = delete;
            MenuAction& operator=(MenuAction&&) = delete;
        };

        QByteArray PathName{}; // the pathname of the open file
        QByteArray FileType{}; // the extension of the open file
        QByteArray Charset{}; // the charset used for reading the open file
        std::shared_ptr<TextFormatter> Formatter; // formatter for the open file
        std::shared_ptr<TextHighlighter> Highlighter; // highlighter for the open file
        std::shared_ptr<QtTreeModel> TreeModel; // for IntuitiveView
    };
} // namespace WritingMaterialsManager

#endif // WRITING_MATERIALS_MANAGER_EDITOR_H
