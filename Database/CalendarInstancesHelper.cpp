
#include "CalendarInstancesHelper.h"
#include <QtSql>
#include <libical/ical.h>
#include "CalendarContract.h"
#include "RecurrenceProcessor.h"

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
  RecurrenceProcessor* rp = new RecurrenceProcessor();

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

    rp->expand(dtstart_millis, recurrence, begin, end, &dates);

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


