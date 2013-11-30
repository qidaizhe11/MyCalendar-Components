#ifndef CALENDARINSTANCESHELPER_H
#define CALENDARINSTANCESHELPER_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>
#include <QList>
#include <libical/ical.h>

class CalendarInstancesHelper
{
public:
  CalendarInstancesHelper();

  void performInstanceExpansion(qint64 begin, qint64 end,
                                const QSqlRecord& record);

private:
  void dateExpand(qint64 dtstart, const icalrecurrencetype& recur,
                  qint64 range_start, qint64 range_end,
                  QList<qint64>* expandedDates);
  int calculateByXXCount(const short* by_XX_array, int by_XX_size);
  int day2TimeDay(icalrecurrencetype_weekday ical_weekday);
};

#endif // CALENDARINSTANCESHELPER_H
