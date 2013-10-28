import QtQuick 2.1

Item {
  id: root
  property real value: 1
  Rectangle {
    id: frame
    width: parent.width
    height: parent.height
    color: "white"
  }
  Rectangle {
    id: emptySlider
    width: frame.width - 4
    height: frame.height - 4
    color: "grey"
    anchors.centerIn: frame
    MouseArea {
      anchors.fill: parent
      onPositionChanged: {
        root.value = Math.max(0, Math.min(1, 1 - (mouse.y - y) / height))
      }
    }
  }
  Rectangle {
    id: fullSlider
    width: emptySlider.width
    height: emptySlider.height * parent.value
    color: "steelblue"
    anchors.horizontalCenter: emptySlider.horizontalCenter
    anchors.bottom: emptySlider.bottom
  }
}
