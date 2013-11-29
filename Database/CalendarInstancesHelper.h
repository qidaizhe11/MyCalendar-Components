#ifndef CALENDARINSTANCESHELPER_H
#define CALENDARINSTANCESHELPER_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>
#include <QList>
#include <libical/icalrecur.h>

class CalendarInstancesHelper
{
public:
  CalendarInstancesHelper();

  void performInstanceExpansion(qint64 begin, qint64 end,
                                const QSqlRecord& record);

private:
  QList<qint64> recurrenceDateExpand(qint64 event_time,
                                 const icalrecurrencetype& recur,
                                 qint64 begin, qint64 end);
};

#endif // CALENDARINSTANCESHELPER_H
