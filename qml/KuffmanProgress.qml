import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Material 0.3

Dialog {

    hasActions: false
    title: "Please wait..."

    width: 300
    height: 100
    Connections{
        target: kuffman_connection
        onOpenKuffmanProgress:{
            open();
        }
        onProgressKuffmanProgress:{
            kuffman_progress_progressbar.value = progress/100;
        }
        onCloseKuffmanProgress:{
            close();
        }
    }

    ProgressBar {
        id: kuffman_progress_progressbar
        width: parent.width
        color: theme.primaryColor
    }
}
