import QtQuick 2.1
import QtQuick.Controls 1.0
import org.kde.pim.calendar 1.0

Item {
    width: 80; height: 60

    Rectangle {
        id: todayRect
        color: "darkgray"

        opacity: (isToday(dayNumber + "/" + monthNumber + "/" + yearNumber)) ?
                     0.8 : 0
        Behavior on opacity { NumberAnimation {} }
    }

    Label {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 16
        text: dayNumber
        opacity: (isPreviousMonth || isNextMonth) ? 0.5 : 1.0
    }

    Component.onCompleted: {
        var t = isToday(dayNumber + "/" + monthNumber + "/" + yearNumber)
        if (t) {
            root.date = model;
        }
        console.log(dayNumber + " " + monthNumber + " " + yearNumber)
    }
}
