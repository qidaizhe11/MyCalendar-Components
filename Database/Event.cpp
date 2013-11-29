
#include "Event.h"
#include <QtSql>
#include "CalendarContract.h"

Event::Event()
{
}

void Event::loadEvents(const QDate &start_day, int days, QList<Event> *events)
{
  QDate end_day = start_day + days - 1;
  qint64 start = QDateTime(start_day).toMSecsSinceEpoch();
  qint64 end = QDateTime(end_day, QTime(23, 59)).toMSecsSinceEpoch();

  QString SQL_SELECT_REPEAT_EVENTS = QString(
        "select * from " + Tables::EVENTS +
        " where " +
        "(" + Events::DT_END + " is null)" +
        " and " + "( " +
        Events::DT_START + " <= " + end + " and " +
        + "(" + Events::LAST_DATE + " is null or " +
        Events::LAST_DATE + " >= " + start + ")"
        + " )");

//  select * from events
//  where
//  (dtend is null)
//  and
//  ( dtstart <= ? and (lastDate is null or lastDate > ?) )

  QSqlDatabase db = QSqlDatabase::database();
  if (db.isValid()) {
    if (db.isOpen()) {
      QSqlQuery query(db);
      bool rc = query.exec(SQL_SELECT_REPEAT_EVENTS);
      if (rc) {
        while (query.next()) {
          QSqlRecord record = query.record();
        }
      }
    }
  }
}
