
#include "RecurrenceProcessor.h"

RecurrenceProcessor::RecurrenceProcessor()
  : byDayCount(0), byMonthDayCount(0)
{
  icalrecurrencetype_clear(&m_recur);
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
  QDate first_day = QDate(current_month.year(), current_month.month(), 1);

  // TODO: 这里统统没考虑规则为负的情形
  // (计算日期从每月的最后倒着往前数）
  //类似BYMONTHDAY=-3,BYDAY=-2MO,... 当前版本，遇此情形可能会崩溃！

  // TODO: interval > 1的处理依然不正确，
  // 对于类似BYMONTHDAY=31,29的情形，last_occur的追踪是错误的！

  // BYMONTHDAY
  if (byMonthDayCount > 0 && m_recur.freq > ICAL_WEEKLY_RECURRENCE) {
    int dayIndex = 0;
    int interval = m_recur.interval;

    while (dayIndex < byMonthDayCount) {
      int day = m_recur.by_month_day[dayIndex];
      bool first_occured = m_bymonthday_last_occurs.at(dayIndex).isNull();

      if (day <= days_in_current_month) {
        if (interval == 1 || first_occured) {
          dayslist->append(day);
          if (interval > 1) {
            m_bymonthday_last_occurs.append(
                  QDate(current_month.year(), current_month.month(), day));
          }
        } else { // interval > 1 && first_occured = false
          QDate last_occured_date = m_bymonthday_last_occurs.at(dayIndex);
          last_occured_date = last_occured_date.addMonths(interval);
          if (last_occured_date.month() == current_month.month()) {
            dayslist->append(day);
            m_bymonthday_last_occurs.at(dayIndex) = last_occured_date;
          }
        } // end else
      }
      dayIndex++;
    } // end while


  } // end BYMONTHDAY

  // BYDAY
  if (byDayCount > 0) {
    // TODO: 这里暂时忽略了WKST的影响，有待以后完善，额，2.0版以后再说吧

    int dayIndex = 0;
    int firstday_of_week = first_day.dayOfWeek();
    int interval = m_recur.interval;

    while (dayIndex < byDayCount) {
      int ical_by_day_day = m_recur.by_day[dayIndex];
      int day_number = ical_by_day_day / 8;
      int day_of_week = icalByDay2WeekDay(ical_by_day_day);

      int day = 0;
      if (day_of_week >= firstday_of_week) {
        day = day_of_week - firstday_of_week + 1 + 7*day_number;
      } else {
        day = day_of_week + 8 - firstday_of_week + 7*day_number;
      }
      bool is_first_occured = m_byday_last_occurs.at(dayIndex).isNull();

      if (day <= days_in_current_month) {
        if (interval == 1 || is_first_occured) {
          // weekly event
          if (m_recur.freq == ICAL_WEEKLY_RECURRENCE) {
            while (day <= days_in_current_month) {
              dayslist->append(day);
              if (interval > 1) {
                m_byday_last_occurs.at(dayIndex) = day;
              }
              day += 7;
            }
          } else { // monthly event
            if (day <= days_in_current_month) {
              dayslist->append(day);
              if (interval > 1) {
                m_byday_last_occurs.at(dayIndex) = day;
              }
            }
          }
        } else { // interval > 1 && is_first_occured = false
          // weekly event
          if (m_recur.freq == ICAL_WEEKLY_RECURRENCE) {
            last_occured_date = last_occured_date.addDays(interval * 7);
            if (last_occured_date.month() == current_month.month()) {
              while (day <= days_in_current_month) {
                dayslist->append(day);
                m_byday_last_occurs.at(dayIndex) = day;
                day += interval*7;
              }
            }
          } else { // monthly event
            QDate last_occured_date = m_byday_last_occurs.at(dayIndex);
            last_occured_date = last_occured_date.addMonths(interval);
            if (last_occured_date.month() == current_month.month()) {
              if (day <= days_in_current_month) {
                dayslist->append(day);
                m_byday_last_occurs.at(dayIndex) = day;
              }
            }
          }
        } // end else
      } // end if

      dayIndex++;
    } // end while
  } // end BYDAY
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

//
// @return  1 = Monday, ..., 7 = Sunday.
// (Qt中QDate的dayOfWeek, 1 = Monday, 7 = Sunday)
//
int RecurrenceProcessor::icalByDay2WeekDay(short ical_by_day_day) {
  ical_by_day_day = static_cast<int>(ical_by_day_day);
  ical_by_day_day %= 8;

  if (ical_by_day_day == 1) { // Sunday
    return 7;
  } else {
    return ical_by_day_day - 1;
  }
}

// 用于解析WKST
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

//    if (interval > 1) {
//      while (dayIndex < byMonthDayCount) {
//        int day = m_recur.by_month_day[dayIndex];

//        if (day <= days_in_current_month) {
//          if (m_bymonthday_last_occurs.at(dayIndex).isNull()) {
//            dayslist->append(day);
//            m_bymonthday_last_occurs.append(
//                  QDate(current_month.year(), current_month.month(), day));
//          } else {
//            QDate last_occured_date = m_bymonthday_last_occurs.at(dayIndex);
//            last_occured_date = last_occured_date.addMonths(interval);
//            if (last_occured_date.month() == current_month.month()) {
//              dayslist->append(day);
//              m_bymonthday_last_occurs.at(dayIndex) = last_occured_date;
//            }
//          }
//        }
//        dayIndex++;
//      } // end while
//    } else { // interval = 1
//      while (dayIndex < byMonthDayCount) {
//        int day = m_recur.by_month_day[dayIndex];
//        if (day <= days_in_current_month) {
//          dayslist->append(day);
//        }
//        dayIndex++;
//      }
//    }


//    if (interval > 1) {
//      while (dayIndex < byDayCount) {
//        int ical_by_day_day = m_recur.by_day[dayIndex];
//        int day_number = ical_by_day_day / 8;
//        int day_of_week = icalByDay2WeekDay(ical_by_day_day);

//        int day = 0;
//        if (day_of_week >= firstday_of_week) {
//          day = day_of_week - firstday_of_week + 1 + 7*day_number;
//        } else {
//          day = day_of_week + 8 - firstday_of_week + 7*day_number;
//        }

//        if (m_byday_last_occurs.at(dayIndex).isNull()) {
//          // weekly event
//          // ...
//          // monthly event
//          // ...
//        } else {
//          // monthly event
//          QDate last_occured_date = m_byday_last_occurs.at(dayIndex);
//          last_occured_date = last_occured_date.addMonths(interval);
//          if (last_occured_date.month() == current_month.month()) {
//            if (day <= days_in_current_month) {
//              dayslist->append(day);
//              m_byday_last_occurs.at(dayIndex) = day;
//            }
//          }

//          // weekly event
//          last_occured_date = last_occured_date.addDays(interval * 7);
//          if (last_occured_date.month() == current_month.month()) {
//            while (day <= days_in_current_month) {
//              dayslist->append(day);
//              m_byday_last_occurs.at(dayIndex) = day;
//              day += interval*7;
//            }
//          }
//        }
//      }
//    } else { // interval = 1
//      while (dayIndex < byDayCount) {
//        int ical_by_day_day = m_recur.by_day[dayIndex];
//        int day_number = ical_by_day_day / 8;
//        int day_of_week = icalByDay2WeekDay(ical_by_day_day);

//        int day = 0;
//        if (day_of_week >= firstday_of_week) {
//          day = day_of_week - firstday_of_week + 1 + 7*day_number;
//        } else {
//          day = day_of_week + 8 - firstday_of_week + 7*day_number;
//        }

//        // weekly event
//        if (m_recur.freq == ICAL_WEEKLY_RECURRENCE) {
//          while (day <= days_in_current_month) {
//            dayslist->append(day);
//            day += 7;
//          }
//        } else { // monthly event
//          if (day <= days_in_current_month) {
//            dayslist->append(day);
//          }
//        }

//        dayIndex++;
//      }
//    }
