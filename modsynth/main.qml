import QtQuick 2.1

Rectangle {
  width: 800
  height: 600
  Slider {
    width: 20
    height: 100
    value: modsynth.filterCutoff
    onValueChanged: modsynth.filterCutoff = value
  }
  Text {
    text: "value: " + Math.round(100 * modsynth.filterCutoff)
  }
}
