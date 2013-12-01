#ifndef CALENDARINSTANCESHELPER_H
#define CALENDARINSTANCESHELPER_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>
#include <QList>
//#include <QVector>
//#include <libical/ical.h>

class CalendarInstancesHelper
{
public:
  CalendarInstancesHelper();

  void performInstanceExpansion(qint64 begin, qint64 end,
                                const QSqlRecord& record);

private:

};

#endif // CALENDARINSTANCESHELPER_H
