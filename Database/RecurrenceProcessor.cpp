
#include "RecurrenceProcessor.h"

RecurrenceProcessor::RecurrenceProcessor()
  : byDayCount(0), byMonthDayCount(0)
{
  icalrecurrencetype_clear(&m_recur);
//  m_days_first_occured = true;
//  m_bymonthday_last_occurs = new QList<QDate>();
//  m_byday_last_occurs = new QList<QDate>();
}

void RecurrenceProcessor::expand(qint64 dtstart,
                                 const icalrecurrencetype &recur,
                                 qint64 range_start, qint64 range_end,
                                 QList<qint64> *expandedDates)
{
  QDateTime iterator;
  QDateTime until;
  QDateTime generated;

  m_recur = recur;

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
  byMonthDayCount = calculateByXXCount(recur.by_month_day,
                                           ICAL_BY_MONTHDAY_SIZE);
  byDayCount = calculateByXXCount(recur.by_day, ICAL_BY_DAY_SIZE);
  bool useDays = freq > ICAL_DAILY_RECURRENCE &&
      (byDayCount>0 || byMonthDayCount > 0);

  for (int i = 0; i < byMonthDayCount; ++i) {
    m_bymonthday_last_occurs.append(QDate());
  }
  for (int i = 0; i < byDayCount; ++i) {
    m_byday_last_occurs.append(QDate());
  }

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

    do { // month
      int month = usebymonth ? recur.by_month[monthIndex] : iteratorMonth;
      month--;

      int dayIndex = 0;
//      int lastDayToExamine = 0;

      if (useDays) {
//        if (freq == ICAL_WEEKLY_RECURRENCE) {
//          int weekStartAdj = (iterator.date().dayOfWeek() -
//                              day2TimeDay(recur.week_start) + 7) % 7;
//          dayIndex = iterator.date().day() - weekStartAdj;
//          lastDayToExamine = dayIndex + 6;
//        }
        // TODO: begin here.
        QList<int> my_by_day_count;
        QDate current_iterator = QDate(iteratorYear, month, 1);
      }

      do { // day

      } while (!eventEnded);
    } while (!eventEnded && usebymonth && monthIndex < byMonthCount);
  }
}

void RecurrenceProcessor::generateDaysList(const QDate &current_month,
                                           QList<int> *dayslist)
{
//  int dayIndex = 0;
  int days_in_current_month = current_month.daysInMonth();

  // BYMONTHDAY
  if (byMonthDayCount > 0 && m_recur.freq > ICAL_WEEKLY_RECURRENCE) {
    int dayIndex = 0;
    int interval = m_recur.interval;

    if (interval > 1) {
      while (dayIndex < byMonthDayCount) {
        int day = m_recur.by_month_day[dayIndex];

        if (day <= days_in_current_month) {
          if (m_bymonthday_last_occurs.at(dayIndex).isNull()) {
            dayslist->append(day);
            m_bymonthday_last_occurs.append(
                  QDate(current_month.year(), current_month.month(), day));
          } else {
            QDate last_occured_date = m_bymonthday_last_occurs.at(dayIndex);
            last_occured_date = last_occured_date.addMonths(interval);
            if (last_occured_date.month() == current_month.month()) {
              dayslist->append(day);
              m_bymonthday_last_occurs.at(dayIndex) = last_occured_date;
            }
          }
        }
      } // end while
    } else { // interval = 1
      while (dayIndex < byMonthDayCount) {
        int day = m_recur.by_month_day[dayIndex];
        if (day <= days_in_current_month) {
          dayslist->append(dayj);
        }
      }
    }
  } // end BYMONTHDAY

  // BYDAY
  if (byDayCount > 0) {

  }
}

int RecurrenceProcessor::calculateByXXCount(const short* by_XX_array,
                                                int by_XX_size)
{
  int count = 0;
  for (int i = 0; i < by_XX_size && by_XX_array[i] != ICAL_RECURRENCE_ARRAY_MAX;
       ++i) {
    ++count;
  }
  return count;
}

int RecurrenceProcessor::day2TimeDay(
    const icalrecurrencetype_weekday& ical_weekday)
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
