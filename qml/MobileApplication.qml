import QtQuick
import QtQuick.Controls
import QtQuick.Window

ApplicationWindow {
    id: appWindow
    minimumWidth: 480
    minimumHeight: 960

    flags: Qt.Window
    color: "white"
    visible: true

    Component.onCompleted: notificationManager.requestNotificationPermissions()

    Connections {
        target: Qt.application
        function onStateChanged() {
            switch (Qt.application.state) {
                case Qt.ApplicationSuspended:
                    //console.log("Qt.ApplicationSuspended")
                    break
                case Qt.ApplicationHidden:
                    //console.log("Qt.ApplicationHidden")
                    break
                case Qt.ApplicationInactive:
                    //console.log("Qt.ApplicationInactive")
                    break

                case Qt.ApplicationActive:
                    //console.log("Qt.ApplicationActive")
                    break
            }
        }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 52
        color: "#f1f3f5"

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.verticalCenter: parent.verticalCenter

            text: "AndroidServiceDemo"
            font.pixelSize: 20
        }
    }

    Column {
        anchors.fill: parent
        anchors.topMargin: 52+16
        anchors.margins: 16

        Text {
            text: "notification permission?" + notificationManager.permissionOS
        }

        Button {
            text: "ask notification permission"
            onClicked: notificationManager.requestNotificationPermissions()
        }

        Button {
            text: "restart service"
            onClicked: notificationManager.restartService()
        }
    }
}
