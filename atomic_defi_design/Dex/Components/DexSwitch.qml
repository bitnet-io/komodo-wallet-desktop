//! Qt Imports.
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.15

//! Projects Imports.
import "../Constants"
import App 1.0
import Dex.Themes 1.0 as Dex

Switch
{
    id: control

    property alias label: _label
    property alias switchButtonWidth: _indicator.width
    property alias switchButtonHeight: _indicator.height
    property alias switchButtonRadius: _indicator.radius
    property alias mouseArea: _mouseArea
    property color textColor: Dex.CurrentTheme.foregroundColor
    property int labelWidth: label.text == '' ? 0 : 120

    width: labelWidth + 60
    height: 30

    font.family: DexTypo.fontFamily
    indicator: DexRectangle
    {
        id: _indicator
        width: 52
        height: 28
        radius: 13
        anchors.verticalCenter: control.verticalCenter

        gradient: Gradient
        {
            orientation: Gradient.Horizontal
            GradientStop
            {
                position: 0
                color: Dex.CurrentTheme.switchGradientStartColor
            }
            GradientStop
            {
                position: 0.9311
                color: Dex.CurrentTheme.switchGradientEndColor
            }
        }

        Rectangle
        {
            visible: !control.checked
            width: parent.width - 4
            height: parent.height - 4
            radius: parent.radius
            x: 2
            y: 2

            gradient: Gradient
            {
                orientation: Gradient.Horizontal
                GradientStop
                {
                    position: 0;
                    color: control.checked ? Dex.CurrentTheme.switchGradientStartColor : Dex.CurrentTheme.switchGradientStartColor2
                }
                GradientStop
                {
                    position: 0.9311;
                    color: control.checked ? Dex.CurrentTheme.switchGradientEndColor : Dex.CurrentTheme.switchGradientEndColor2
                }
            }
        }

        Rectangle
        {
            x: control.checked ? parent.width - width - 4 : 4
            y: 3
            width: parent.width / 2 - 2
            height: parent.height - 6
            radius: parent.radius + 2

            gradient: Gradient
            {
                orientation: Gradient.Horizontal
                GradientStop
                {
                    position: 0;
                    color: control.checked ? Dex.CurrentTheme.switchGradientStartColor2 : Dex.CurrentTheme.switchGradientStartColor
                }
                GradientStop
                {
                    position: 0.9311;
                    color: control.checked ? Dex.CurrentTheme.switchGradientEndColor2 : Dex.CurrentTheme.switchGradientEndColor
                }
            }
        }
    }

    Item
    {
        Layout.preferredWidth: labelWidth
        Layout.preferredHeight: _indicator.height
        Layout.alignment: Qt.AlignVCenter
        anchors.verticalCenter: control.verticalCenter

        visible: {
            console.log(_label.text)
            _label.text != ''
        }

        DexLabel
        {
            id: _label
            Layout.fillHeight: true

            font: control.font
            color: control.textColor
            leftPadding: _indicator.width + control.spacing 
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            wrapMode: Label.Wrap

        }
    }

    DexMouseArea
    {
        id: _mouseArea
        anchors.fill: control
        acceptedButtons: Qt.NoButton
    }
}