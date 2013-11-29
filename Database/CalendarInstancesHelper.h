#ifndef CALENDARINSTANCESHELPER_H
#define CALENDARINSTANCESHELPER_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>

class CalendarInstancesHelper
{
public:
  CalendarInstancesHelper();

  void performInstanceExpansion(qint64 start, qint64 end,
                                const QSqlRecord& record);
};

#endif // CALENDARINSTANCESHELPER_H
