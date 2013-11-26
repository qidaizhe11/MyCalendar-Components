#include "CalendarContract.h"

// Calendars

const QString CalendarColumns::NAME = "name";
const QString CalendarColumns::CALENDAR_COLOR = "calendar_color";
const QString CalendarColumns::CALENDAR_DISPLAYNAME = "calendar_displayName";
const QString CalendarColumns::VISIBLE = "visible";

// Events

const QString EventColumns::CALENDAR_ID = "calendar_id";
const QString EventColumns::TITLE = "title";
const QString EventColumns::DESCRIPTION = "description";
const QString EventColumns::EVENT_LOCATION = "eventLocation";
const QString EventColumns::DT_START = "dtstart";
const QString EventColumns::DT_END = "dtend";
const QString EventColumns::DURATION = "duration";
const QString EventColumns::EVENT_TIMEZONE = "eventTimezone";
const QString EventColumns::EVENT_END_TIMEZONE = "eventEndTimezone";
const QString EventColumns::ALL_DAY = "allday";
const QString EventColumns::AVAILABILITY = "availability";
const QString EventColumns::ACCESS_LEVEL = "accessLevel";
const QString EventColumns::RRULE = "rrule";
const QString EventColumns::RDATE = "rdate";
const QString EventColumns::ORGANIZER = "organizer";

const int EventColumns::ACCESS_DEFAULT = 0;
const int EventColumns::ACCESS_CONFIDENTIAL = 1;
const int EventColumns::ACCESS_PRIVATE = 2;
const int EventColumns::ACCESS_PUBLIC = 3;

const int EventColumns::AVAILABILITY_BUSY = 0;
const int EventColumns::AVAILABILITY_FREE = 1;
const int EventColumns::AVAILABILITY_TENTATIVE = 2;

// Instances

const QString Instances::BEGIN = "begin";
const QString Instances::END = "end";
const QString Instances::EVENT_ID = "event_id";
const QString Instances::START_DAY = "startDay";
const QString Instances::END_DAY = "endDay";
const QString Instances::START_MINUTE = "startMinute";
const QString Instances::END_MINUTE = "endMinute";

// Reminders

const QString ReminderColumns::EVENT_ID = "event_id";
const QString ReminderColumns::MINUTES = "minutes";
const QString ReminderColumns::METHOD = "method";

const int ReminderColumns::METHOD_DEFAULT = 0;
const int ReminderColumns::METHOD_ALERT = 1;
const int ReminderColumns::METHOD_EMAIL = 2;
const int ReminderColumns::METHOD_SMS = 3;
const int ReminderColumns::METHOD_ALARM = 4;
