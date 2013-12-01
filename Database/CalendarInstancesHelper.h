#ifndef CALENDARINSTANCESHELPER_H
#define CALENDARINSTANCESHELPER_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>
#include <QList>
//#include <libical/ical.h>

class CalendarInstancesHelper
{
public:
  CalendarInstancesHelper();
  ~CalendarInstancesHelper() {}

  void performInstanceExpansion(qint64 begin, qint64 end,
                                const QSqlRecord& record);

  void instancesReplace(QList<Instance*> instances_list);

private:

};

class Instance
{
public:
  Instance() {}
  ~Instance() {}

  int m_event_id;
  int m_begin;
  int m_end;
};

#endif // CALENDARINSTANCESHELPER_H
