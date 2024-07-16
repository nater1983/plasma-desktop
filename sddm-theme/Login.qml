import org.kde.breeze.components

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2

import org.kde.plasma.components 3.0 as PlasmaComponents3
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kirigami 2.20 as Kirigami

SessionManagementScreen {
    id: root
    property string lastUserName: ""
    required property int sessionIndex

    usernameField: userNameInput
    passwordField: passwordBox
    requestLoginCallback: (username, password) => {
        root.setNotificationMessage();
        Qt.callLater(sddm.login, username, password, root.sessionIndex);
        return [sddm.loginFailed, sddm.loginSucceeded]
    }

    Connections {
        target: sddm
        function onLoginFailed() {
            passwordBox.selectAll()
            passwordBox.forceActiveFocus(Qt.TabFocusReason)
        }
    }

    contentItem: ColumnLayout {
        baselineOffset: height
        spacing: root.spacing
        PlasmaComponents3.TextField {
            id: userNameInput
            Layout.fillWidth: true

            text: root.lastUserName
            visible: !root.showUserList
            focus: visible && !root.lastUserName //if there's a username prompt it gets focus first, otherwise password does
            placeholderText: i18nd("plasma-desktop-sddm-theme", "Username")

            onAccepted: passwordBox.forceActiveFocus(Qt.TabFocusReason)
            onVisibleChanged: {
                if (!visible) {
                    root.lastUserName = ""
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            PlasmaExtras.PasswordField {
                id: passwordBox
                Layout.fillWidth: true

                placeholderText: i18nd("plasma-desktop-sddm-theme", "Password")
                focus: !root.usernameField.visible || lastUserName

                // Disable reveal password action because SDDM does not have the breeze icon set loaded
                rightActions: []

                onAccepted: requestLogin()

                visible: root.usernameField.visible || userList.currentItem.needsPassword

                Keys.onEscapePressed: {
                    mainStack.currentItem.forceActiveFocus();
                }

                //if empty and left or right is pressed change selection in user switch
                //this cannot be in keys.onLeftPressed as then it doesn't reach the password box
                Keys.onPressed: event => {
                    if (event.key === Qt.Key_Left && !text) {
                        userList.decrementCurrentIndex();
                        event.accepted = true
                    }
                    if (event.key === Qt.Key_Right && !text) {
                        userList.incrementCurrentIndex();
                        event.accepted = true
                    }
                }
            }

            PlasmaComponents3.Button {
                id: loginButton
                Accessible.name: i18nd("plasma-desktop-sddm-theme", "Log In")
                Layout.preferredHeight: passwordBox.implicitHeight
                Layout.preferredWidth: text.length === 0 ? loginButton.Layout.preferredHeight : -1

                icon.name: text.length === 0 ? (root.LayoutMirroring.enabled ? "go-previous" : "go-next") : ""

                text: root.usernameField.visible || userList.currentItem.needsPassword ? "" : i18n("Log In")
                onClicked: requestLogin()
                Keys.onEnterPressed: clicked()
                Keys.onReturnPressed: clicked()
            }
        }
    }
}
