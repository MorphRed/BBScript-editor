import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

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
            MenuItem {
                text: qsTr("&New...")
            }
            MenuItem {
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            MenuItem {
                text: qsTr("&Save")
            }
            MenuItem {
                text: qsTr("Save &As...")
            }
            MenuSeparator {
            }
            MenuItem {
                text: qsTr("&Quit")
            }
        }
        Menu {
            title: qsTr("&Edit")
            MenuItem {
                text: qsTr("Cu&t")
            }
            MenuItem {
                text: qsTr("&Copy")
            }
            MenuItem {
                text: qsTr("&Paste")
            }
        }
        Menu {
            title: qsTr("&Help")
            MenuItem {
                text: qsTr("&About")
            }
        }
    }
    ScrollView {
        id: commandMenu
        width: 500
        height: parent.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        Layout.fillHeight: true
        // Get the implicit height through cpp instead of manually adding it ig?
        component CommandDelegate : TextField {
            
        }
        component ViewerDelegate : Rectangle {
            ListView {
                
            }
        }
        // ListView {
        //     width: 180; height: 200
        //
        //     model: nameModel {}
        //     delegate: Text {
        //         required property string name
        //         required property string number
        //         text: name + ": " + number
        //     }
        // }
        
    }
}

