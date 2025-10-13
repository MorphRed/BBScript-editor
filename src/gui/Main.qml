import QtCore
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    
    menuBar: MenuBar {
        FileDialog {
            id: fileDialog
            currentFolder: StandardPaths.standardLocations()
        }
        Menu {
            id: menu
            title: qsTr("&File")
            MenuItem { text: qsTr("&New...") }
            MenuItem { 
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            MenuItem { text: qsTr("&Save") }
            MenuItem { text: qsTr("Save &As...") }
            MenuSeparator { }
            MenuItem { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            MenuItem { text: qsTr("Cu&t") }
            MenuItem { text: qsTr("&Copy") }
            MenuItem { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            MenuItem { text: qsTr("&About") }
        }
    }
    
}

