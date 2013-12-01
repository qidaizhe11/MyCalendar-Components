#ifndef RECURRENCEPROCESSOR_H
#define RECURRENCEPROCESSOR_H

#include <QString>
#include <QDateTime>
//#include <QVector>
#include <QList>
#include <libical/ical.h>

class RecurrenceProcessor
{
public:
  RecurrenceProcessor();

  void expand(qint64 dtstart, const icalrecurrencetype& recur,
              qint64 range_start, qint64 range_end,
              QList<qint64>* expandedDates);

private:
  void generateDaysList(const QDate& current_month,
//                        const icalrecurrencetype& recur,
//                        int byDayCount, int byMonthDayCount,
                        QList<int>* dayslist);

  int calculateByXXCount(const short* by_XX_array, int by_XX_size);
  int day2TimeDay(const icalrecurrencetype_weekday& ical_weekday);

  icalrecurrencetype m_recur;
  int byDayCount, byMonthDayCount;

//  bool m_days_first_occured;

  // 这两个数组的索引与icalrecurrencetype中
  // bymonthday,byday数组的index完全一致
  QList<QDate> m_bymonthday_last_occurs;
  QList<QDate> m_byday_last_occurs;
};

#endif // RECURRENCEPROCESSOR_H
