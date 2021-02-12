import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem

Flickable{
    property bool compressable: {
        var selected = get_selected();
        if(selected.length == 1){
            !selected[0].match(".kuf")
        }else{
            false
        }
    }
    property bool decompressable: {
        var selected = get_selected();
        if(selected.length == 1){
            selected[0].match(".kuf")
        }else{
            false
        }
    }
    Connections{
        target: kuffman_connection
        onClearKuffmanList:{
            kuffman_list_model.clear()
        }
        onAddKuffmanList:{
            kuffman_list_model.append({"path":path,"name":name,"type":type})
        }
    }
    function get_selected(){
        var result = [];
        for(var cursor = 0 ; cursor < kuffman_list_model.count ; cursor++){
            if(kuffman_list_repeater.itemAt(cursor).selected == 1){
                result[result.length] = kuffman_list_model.get(cursor).path
            }
        }
        return result;
    }
    function clear_selected(){
        for(var cursor = 0 ; cursor < kuffman_list_model.count ; cursor++){
            kuffman_list_repeater.itemAt(cursor).selected = 0;
        }
    }
    contentHeight: kuffman_list_column.height
    Column{
        id:kuffman_list_column
        width:parent.width
        Repeater {
            id: kuffman_list_repeater
            model: ListModel{
                id: kuffman_list_model
            }

            delegate: ListItem.Subtitled {               
                backgroundColor: if(kuffman_list_repeater.itemAt(index).selected == 1){Palette.colors[theme.primaryColor]["200"]}else{"white"}
                iconName: kuffman_list_model.get(index).type == 1 ? "content/folder" : "content/file"
                subText: kuffman_list_model.get(index).type == 1 ? "Folder" : "File"
                valueText: kuffman_list_model.get(index).name
                text: kuffman_list_model.get(index).name
                onClicked: {
                    clear_selected();
                    if(kuffman_list_model.get(index).type == 0){
                        kuffman_list_repeater.itemAt(index).selected = kuffman_list_repeater.itemAt(index).selected == 1 ? 0 : 1
                    }else{
                        kuffman_connection.loadKuffmanList(kuffman_list_model.get(index).path+"/")
                    }
                }
            }
        }

    }
}
