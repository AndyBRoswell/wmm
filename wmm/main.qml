import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true // don't omit this, or the main window will be invisible
    visibility: "Maximized" // maximized window when open
    title: qsTr("Hello World")
    Universal.theme: Universal.Dark // default theme
    Universal.accent: Universal.Cobalt // default accent
}
