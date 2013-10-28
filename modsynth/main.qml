import QtQuick 2.0

Rectangle {
  width: 800
  height: 600
  color: "black"
  Slider {
    width: 20
    height: 100
    value: modsynth.filterCutoff
    onValueChanged: modsynth.filterCutoff = value
  }
}
