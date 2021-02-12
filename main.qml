import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Material 0.3
import Kuffman 1.0
import "qrc:///qml" as Kuffman

ApplicationWindow {
    width: 350
    height: 500
    id: kuffman_application
    title: "Kuffman"
    visible: true
    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }

    Component.onCompleted: {
        kuffman_connection.loadKuffmanList("/");
    }

    ActionBar{
        title: kuffman_application.title
        actions: [
            Action {
                iconName: "image/color_lens"
                name: "Theme"
                hoverAnimation: true
                onTriggered: kuffman_theme.open()
            },

            Action {
                iconName: "content/compress"
                name: "Compress"
                enabled: kuffman_list.compressable
                hoverAnimation: true
                onTriggered: {
                    kuffman_connection.compressKuffman(kuffman_list.get_selected()[0],"/home/koliber/test.kuf");
                }
            },

            Action {
                iconName: "content/decompress"
                name: "Decompress"
                enabled: kuffman_list.decompressable
                hoverAnimation: true
                onTriggered: {
                    kuffman_connection.decompressKuffman(kuffman_list.get_selected()[0],"/home/koliber/test.txt");
                }
            }
        ]
    }

    initialPage: Page {
        Kuffman.KuffmanList{
            id:kuffman_list
            anchors.fill: parent
        }
    }

    Kuffman.KuffmanTheme{
        id: kuffman_theme
    }
    Kuffman.KuffmanProgress{}
    KuffmanConnection{
        id: kuffman_connection
    }
}
