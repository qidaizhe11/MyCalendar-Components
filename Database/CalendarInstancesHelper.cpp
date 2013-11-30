
#include "CalendarInstancesHelper.h"
#include <QtSql>
//#include <libical/ical.h>
#include "CalendarContract.h"

CalendarInstancesHelper::CalendarInstancesHelper()
{

//  struct icalrecurrencetype recur = icalrecurrencetype_from_string(
//        "FREQ=WEEKLY;COUNT=35;INTERVAL=2;BYDAY=TU,WE,TH");
//  struct icalrecurrencetype recur = icalrecurrencetype_from_string(
//        "FREQ=WEEKLY;COUNT=8;BYDAY=MO,TU,WE,TH,FR");

  char* str = "FREQ=MONTHLY;BYDAY=2SU";
//  char* str = "FREQ=WEEKLY;COUNT=8;BYDAY=SU";
  icalrecurrencetype recur = icalrecurrencetype_from_string(str);

  int count = recur.count;
}

//
// public
//

void CalendarInstancesHelper::performInstanceExpansion(qint64 begin, qint64 end,
                                                       const QSqlRecord &record)
{
  qint64 dtstart_millis = record.value(Events::DT_START).toLongLong();
  int event_id = record.value(Events::_ID).toInt();
  int calendar_id = record.value(Events::CALENDAR_ID).toInt();
  QString duration_str = record.value(Events::DURATION).toString();

  QString rrule_str = record.value(Events::RRULE).toString();
  QString rdate_str = record.value(Events::RDATE).toString();

  struct icaldurationtype duration;

  if (!duration_str.isEmpty()) {
    QByteArray byte_array = duration_str.toLatin1();
    const char* c_dur = byte_array.data();
    duration = icaldurationtype_from_string(c_dur);
  }

  if (!rrule_str.isEmpty()) {
    QByteArray byte_array = rrule_str.toLatin1();
    const char* c_rrule = byte_array.data();
    icalrecurrencetype recurrence = icalrecurrencetype_from_string(c_rrule);

    if (duration_str.isEmpty()) {
      // TODO: 把它搞成 全天 事件
    }

    QList<qint64> dates;

    dateExpand(dtstart_millis, recurrence, begin, end, &dates);

    qint64 duration_millis = icaldurationtype_as_int(duration);

    foreach (qint64 date, dates) {
      qint64 dtend_millis = date + duration_millis;
      // TODO: insert or replace in Instances table.
    }
  }
}

//
// private
//

void CalendarInstancesHelper::dateExpand(qint64 dtstart,
                                         const icalrecurrencetype &recur,
                                         qint64 range_start, qint64 range_end,
                                         QList<qint64> *expandedDates)
{
  QDateTime iterator;
  QDateTime until;
  QDateTime generated;

  int freqAmount = recur.interval;
  int freq = recur.freq;
  if (freq == ICAL_WEEKLY_RECURRENCE) {
    freqAmount = 7 * recur.interval;
    if (freqAmount <= 0) {
      freqAmount = 7;
    }
  }

  if (freq == ICAL_NO_RECURRENCE) {
    // TODO: 这里应该是要处理下的
  }

  int byMonthCount = calculateByXXCount(recur.by_month, ICAL_BY_MONTH_SIZE);
  bool usebymonth = freq > ICAL_MONTHLY_RECURRENCE && byMonthCount > 0;
  int byMonthDayCount = calculateByXXCount(recur.by_month_day,
                                           ICAL_BY_MONTHDAY_SIZE);
  int byDayCount = calculateByXXCount(recur.by_day, ICAL_BY_DAY_SIZE);
  bool useDays = freq > ICAL_DAILY_RECURRENCE &&
      (byDayCount>0 || byMonthDayCount > 0);
  // TODO: 完善usebyhour, usebyminute, usebysecond

  iterator = QDateTime::fromMSecsSinceEpoch(dtstart);
  if (ICAL_MONTHLY_RECURRENCE) {
    if (useDays) {
      // if ..., set the iterator day field to 1 because sometimes
      // we'll skip months if it's greater than 28.
      iterator.setDate(
            QDate(iterator.date().year(), iterator.date().month(), 1));
    }
  }

  qint64 untilDateValue;
  if (!icaltime_is_null_time(recur.until)) {
    untilDateValue = static_cast<qint64>( icaltime_as_timet(recur.until) );
  } else {
    untilDateValue = std::numeric_limits<qint64>::max();
  }

  bool eventEnded = false;
  int failsafe = 0; // Avoid infinite loops

  while (!eventEnded) {
    int monthIndex = 0;

    int iteratorYear = iterator.date().year();
    int iteratorMonth = iterator.date().month();
    int iteratorDay = iterator.date().day();

    generated = iterator;

    do {
      int month = usebymonth ? recur.by_month[monthIndex] : iteratorMonth;
      month--;

      int dayIndex = 1;
      int lastDayToExamine = 0;

      if (useDays) {
        if (freq == ICAL_WEEKLY_RECURRENCE) {
          int weekStartAdj = (iterator.date().dayOfWeek() -
                              day2TimeDay(recur.week_start) + 7) % 7;
          dayIndex = iterator.date().day() - weekStartAdj;
          lastDayToExamine = dayIndex + 6;
        }
        // TODO: begin here.
      }
    } while (!eventEnded);
  }
}

int CalendarInstancesHelper::calculateByXXCount(const short* by_XX_array,
                                                int by_XX_size)
{
  int count = 0;
  for (int i = 0; i < by_XX_size && by_XX_array[i] != ICAL_RECURRENCE_ARRAY_MAX;
       ++i) {
    ++count;
  }
  return count;
}

int CalendarInstancesHelper::day2TimeDay(
    icalrecurrencetype_weekday ical_weekday)
{
  switch (ical_weekday) {
  case ICAL_SUNDAY_WEEKDAY:
    return 0;
  case ICAL_MONDAY_WEEKDAY:
    return 1;
  case ICAL_TUESDAY_WEEKDAY:
    return 2;
  case ICAL_WEDNESDAY_WEEKDAY:
    return 3;
  case ICAL_THURSDAY_WEEKDAY:
    return 4;
  case ICAL_FRIDAY_WEEKDAY:
    return 5;
  case ICAL_SATURDAY_WEEKDAY:
    return 6;
  default:
    throw QString("bad type: ICAL_NO_WEEKDAY");
  }
}
