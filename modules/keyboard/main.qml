import DBus.Com.Deepin.Daemon.ExtDevManager 1.0
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import "../widgets/"

Item {
    id: keyboardModule
    anchors.fill: parent

    property int contentLeftMargin: 22
    property int contentHeight: 60

    property var dconstants: DConstants {}

    ExtDevManager {
       id: "extDevManagerID"
    } 
    Keyboard {
       id: "keyboardID"
    } 
    Mouse {
       id: "mouseID"
    } 
    TouchPad {
       id: "touchPadID"
    } 

    Column {
        anchors.top: parent.top
        width: parent.width

        DssTitle {
            text:dsTr("Keyboard")
        }

        DSepratorHorizontal {}

        DBaseHeader {
            title.sourceComponent: DssH2 {
                text: dsTr("Repeat")
            }
        }

        DSepratorHorizontal {}

        DBaseHeader {
            color: dconstants.contentBgColor
            height: contentHeight
            leftMargin: contentLeftMargin
            title.sourceComponent: DLabel {
                text: dsTr("Repeat Delay")
                font.pixelSize: 12
            }
            button.sourceComponent: SliderRect {
                leftLabel: dsTr("Long")
                rightLabel: dsTr("Short")

                value: (2000 - keyboardID.repeatDelay)/1900
                onValueChanged: {
                    if (keyboardID.repeatDelay != (2000 - 1900 * value)){
                        keyboardID.repeatDelay = (2000 - 1900 * value)
                    }
                }
            }
        }

        DSepratorHorizontal {}

        DBaseHeader {
            color: dconstants.contentBgColor
            height: contentHeight
            leftMargin: contentLeftMargin
            title.sourceComponent: DLabel {
                text: dsTr("Repeat Interval")
                font.pixelSize: 12
            }

            button.sourceComponent: SliderRect {
                leftLabel: dsTr("Slow")
                rightLabel: dsTr("Fast")

                value: (2000 - keyboardID.repeatSpeed)/1980
                onValueChanged: {
                    if (keyboardID.repeatSpeed != (2000 - 1980 * value)){
                        keyboardID.repeatSpeed = 2000 - 1980 * value
                    }
                }
            }

        }

        DSepratorHorizontal {}

        DBaseHeader {
            color: dconstants.contentBgColor
            leftMargin: contentLeftMargin
            title.sourceComponent: DLabel {
                text: dsTr("Test Repeat Interval")
                font.pixelSize: 13
            }
            button.sourceComponent: Rectangle {
                clip: true
                color: "transparent"
                border.width: 1
                border.color: "#888"

                width: testRepeatIntervalInput.width + 4
                height: testRepeatIntervalInput.height + 4

                TextInput {
                    id: testRepeatIntervalInput
                    anchors.centerIn: parent
                    width: 130
                    font.pixelSize: 12
                    color: dconstants.fgColor
                    focus: true
                    selectByMouse: true
                }
            }
        }

        DSepratorHorizontal {}

        DBaseHeader {
            title.sourceComponent: DssH2 {
                text: dsTr("Cusor Blink")
            }
        }

        DSepratorHorizontal {}

        DBaseHeader {
            color: dconstants.contentBgColor
            height: contentHeight
            leftMargin: contentLeftMargin
            title.sourceComponent: Rectangle {
                width: 1
                height: 14
                color: "white"

                Timer {
                    running: true
                    interval: keyboardID.cursorBlink/2
                    repeat: true
                    onTriggered: parent.visible = !parent.visible
                }
            }
            button.sourceComponent: SliderRect{
                id: cursorBlinkSlider
                leftLabel: dsTr("Slow")
                rightLabel: dsTr("Fast")

                value: (2500 - keyboardID.cursorBlink)/2400
                onValueChanged: {
                    if (keyboardID.cursorBlink != (2500 - 2400 * value)){
                        keyboardID.cursorBlink = 2500 - 2400 * value
                    }
                }

            }
        }

        DSepratorHorizontal {}

        DBaseExpand {
            id: keyboardLayoutSetting
            header.sourceComponent: DDownArrowHeader {
                text: dsTr("Keyboard Layout")
                darkText: dsTr("US")
                onClicked: {
                    keyboardLayoutSetting.expanded = !keyboardLayoutSetting.expanded
                }
            }

            content.sourceComponent: Column {
                Text {text:"test"}
                Text {text:"test"}
                Text {text:"test"}
                Text {text:"test"}
                Text {text:"test"}
                Text {text:"test"}
                Text {text:"test"}
            }
        }
    }
}
