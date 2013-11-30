#include <QtGui/QGuiApplication>
#include <QQmlComponent>
#include "qtquick2applicationviewer.h"
#include "Calendar.h"
#include <QAbstractItemModel>
#include <QAbstractListModel>

#include "Database/CalendarInstancesHelper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    CalendarInstancesHelper* calendar_instances_helper =
        new CalendarInstancesHelper();

//    Q_ASSERT(uri == QLatin1String("org.kde.pim.calendar"));
    qmlRegisterType<Calendar>("org.kde.pim.calendar", 1, 0, "Calendar");
    qmlRegisterType<QAbstractItemModel>();
    qmlRegisterType<QAbstractListModel>();

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/CalendarComponents/MonthView.qml"));
    viewer.showExpanded();

    return app.exec();
}
