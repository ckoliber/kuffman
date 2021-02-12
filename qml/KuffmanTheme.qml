import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import Material 0.2
import Material.ListItems 0.1 as ListItem

Dialog {
    hasActions: false
    title: "Theme Editor"

    function setThemeColor(type,clr){
        switch(type) {
            case 0:
                theme.primaryColor = clr
                break;
            case 1:
                theme.accentColor = clr
                break;
        }
    }

    Column{
        width: parent.width - 20
        x: 10
        bottomPadding: 20

        MenuField {
            x:  parent.width/2 - width/2
            id: selection
            model: ["Primary Color", "Text Color"]
            width: dp(160)
        }

        Grid {
            x:  parent.width/2 - width/2
            columns: 7
            spacing: dp(8)

            ListModel{
                id:colorsModel
                ListElement{name:"red"}
                ListElement{name:"pink"}
                ListElement{name:"purple"}
                ListElement{name:"deepPurple"}
                ListElement{name:"indigo"}
                ListElement{name:"blue"}
                ListElement{name:"lightBlue"}
                ListElement{name:"cyan"}
                ListElement{name:"teal"}
                ListElement{name:"green"}
                ListElement{name:"lime"}
                ListElement{name:"lightGreen"}
                ListElement{name:"yellow"}
                ListElement{name:"amber"}
                ListElement{name:"orange"}
                ListElement{name:"deepOrange"}
                ListElement{name:"grey"}
                ListElement{name:"blueGrey"}
                ListElement{name:"brown"}
            }

            Repeater {
                model: colorsModel
                Rectangle {
                    width: dp(30)
                    height: dp(30)
                    radius: dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent
                        onPressed: {
                            setThemeColor(selection.selectedIndex,colorsModel.get(index).name)
                        }
                    }
                }
            }
        }

    }

}
