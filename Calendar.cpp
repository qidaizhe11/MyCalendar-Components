
#include "Calendar.h"

Calendar::Calendar(QObject *parent) :
  QObject(parent),
  m_types(Holiday | Event | Todo | Journal),
  m_dayList(),
  m_weekList(),
  m_weeks(),
  m_startDay(Qt::Monday),
  m_errorMessage()
{
  m_model = new DaysModel(this);
  m_model->setSourceData(&m_dayList);
}

QDate Calendar::startDate() const
{
  return m_startDate;
}

void Calendar::setStartDate(const QDate &dateTime)
{
  if (m_startDate == dateTime) {
    return;
  }
  m_startDate = dateTime;
  updateData();
  emit startDateChanged();
}

int Calendar::types() const
{
  return m_types;
}

void Calendar::setTypes(int types)
{
  if (m_types == types)
    return;
  emit typesChanged();
}

int Calendar::weeks() const
{
  return m_weeks;
}

void Calendar::setWeeks(int weeks)
{
  if (m_weeks != weeks) {
    m_weeks = weeks;
    updateData();
    emit weeksChanged();
  }
}

int Calendar::startDay() const
{
  return m_startDay;
}

void Calendar::setStartDay(int day)
{
  if (day > 7 || day < 1)
    return;

  if (m_startDay != day) {
    m_startDay = day;
    emit startDayChanged();
  }
}

QString Calendar::errorMessage() const
{
  return m_errorMessage;
}

int Calendar::year() const
{
  return m_startDate.year();
}

QAbstractListModel* Calendar::model() const
{
  return m_model;
}

QList<int> Calendar::weeksModel() const
{
  return m_weekList;
}
