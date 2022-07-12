/*
* VLC-Qt QML Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

import QtQuick 2.1
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0
import MDKPlayer 1.0

Window {
    width: 640
    height: 480
    color: "black"
    visible: true

    property string m_curPlayFile: ""

    Rectangle
    {
        id: _toolPanel
        width: parent.width
        height: 30
        color: "transparent"

        Row
        {
            anchors.fill: parent
            spacing: 8
            Button
            {
                text: "select file"
                height: 24
                onClicked: {
                    fileDialog.open()
                }
            }
        }
    }

    FileDialog
    {
        id: fileDialog
        visible: false
        title: "please select video file"
        selectFolder: false
        selectMultiple: false
        nameFilters: [ "video files (*.mp4 *.m4v *.wmv *.mov *.asf *.flv *.f4v *.rmvb *.rm *.3gp *.vob *.avi *.div *.dv *.divx)",
            "All files (*)" ]
        onAccepted: {
            m_curPlayFile = fileDialog.fileUrl
            console.log("You chose: " + m_curPlayFile)

            if (m_curPlayFile === "")
            {
                return
            }

            player.source = m_curPlayFile
            player.play()
        }
    }

    MDKPlayer {
        id: player
        width: parent.width
        height: parent.height - _toolPanel
        x: 0
        y: _toolPanel.y + _toolPanel.height
    }

    function gfxApi() {
        switch (GraphicsInfo.api) {
        case GraphicsInfo.Direct3D11:
            return "Direct3D11";
        case GraphicsInfo.Vulkan:
            return "Vulkan";
        case GraphicsInfo.Metal:
            return "Metal";
        case GraphicsInfo.OpenGL:
            return "OpenGL";
        case GraphicsInfo.Null:
            return "Null";
        case GraphicsInfo.Direct3D11Rhi:
            return "Direct3D11 Rhi";
        case GraphicsInfo.VulkanRhi:
            return "Vulkan Rhi";
        case GraphicsInfo.MetalRhi:
            return "Metal Rhi";
        case GraphicsInfo.OpenGLRhi:
            return "OpenGL Rhi";
        case GraphicsInfo.Software:
            return "Software";
        case GraphicsInfo.NullRhi:
            return "Null Rhi";
        case GraphicsInfo.Unknown:
            return "Unknown";
        }
        return "What?";
    }
}
