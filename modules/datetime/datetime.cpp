#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include <libdui/dseparatorhorizontal.h>
#include <libdui/dimagebutton.h>

#include "datetime.h"
#include "timewidget.h"
#include "moduleheader.h"
#include "timezonewidget.h"
#include "dbus/dbustimedate.h"

DUI_USE_NAMESPACE

Datetime::Datetime() :
    QObject(),
    m_frame(new QFrame),
    m_dbusInter(m_frame),
    m_timezoneListWidget(new QWidget)
{
    // get timezone info list
    m_zoneInfoList = new QList<ZoneInfo>;
    QStringList zoneList = m_dbusInter.GetZoneList();
    for (const QString & zone : zoneList)
        m_zoneInfoList->append(m_dbusInter.GetZoneInfo(zone).argumentAt<0>());

    // sort by utc offset ascend, if utc offset is equal, sort by city.
    std::sort(m_zoneInfoList->begin(), m_zoneInfoList->end(), [this] (const ZoneInfo & z1, const ZoneInfo & z2) -> bool {
        if (z1.m_utcOffset == z2.m_utcOffset)
            return z1.m_zoneCity < z2.m_zoneCity;
        return z1.m_utcOffset < z2.m_utcOffset;
    });

    ModuleHeader *header = new ModuleHeader(tr("Date and Time"), false);

    m_dateCtrlWidget = new DateControlWidget;

    DHeaderLine *dateBaseLine = new DHeaderLine;
    dateBaseLine->setTitle(tr("Date"));
    dateBaseLine->setContent(m_dateCtrlWidget);

    m_timezoneCtrlWidget = new TimezoneCtrlWidget;

    m_timezoneHeaderLine = new DHeaderLine;
    m_timezoneHeaderLine->setTitle(tr("TimeZone"));
    m_timezoneHeaderLine->setContent(m_timezoneCtrlWidget);

    m_autoSyncSwitcher = new DSwitchButton;
    m_autoSyncSwitcher->setChecked(m_dbusInter.nTP());
    DHeaderLine *cyncBaseLine = new DHeaderLine;
    cyncBaseLine->setTitle(tr("Sync Automaticly"));
    cyncBaseLine->setContent(m_autoSyncSwitcher);

    m_clockFormatSwitcher = new DSwitchButton;
    m_clockFormatSwitcher->setChecked(m_dbusInter.use24HourFormat());
    DHeaderLine *clockFormat = new DHeaderLine;
    clockFormat->setTitle(tr("Use 24-hour clock"));
    clockFormat->setContent(m_clockFormatSwitcher);

    m_calendar = new DCalendar(m_frame);
    m_calendar->setMinimumHeight(350);

    TimeWidget *timeWidget = new TimeWidget;
    timeWidget->setIs24HourFormat(m_dbusInter.use24HourFormat());

    showSelectedTimezoneList();

    QVBoxLayout *centeralLayout = new QVBoxLayout;
    centeralLayout->addWidget(header);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(timeWidget);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(cyncBaseLine);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(clockFormat);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(m_timezoneHeaderLine);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(m_timezoneListWidget);
    //centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(dateBaseLine);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addWidget(m_calendar);
    centeralLayout->addWidget(new DSeparatorHorizontal);
    centeralLayout->addStretch(1);
    centeralLayout->setSpacing(0);
    centeralLayout->setMargin(0);

    m_frame->setLayout(centeralLayout);
    m_dateCtrlWidget->setVisible(!m_dbusInter.nTP());

    connect(m_timezoneCtrlWidget, &TimezoneCtrlWidget::removeAccept, this, &Datetime::showSelectedTimezoneList);
    connect(m_timezoneCtrlWidget, &TimezoneCtrlWidget::removeTimezone, this, &Datetime::toRemoveTimezoneMode);
    connect(m_timezoneCtrlWidget, &TimezoneCtrlWidget::addTimezone, this, &Datetime::showTimezoneList);
    connect(m_clockFormatSwitcher, &DSwitchButton::checkedChanged, &m_dbusInter, &DBusTimedate::setUse24HourFormat);
    connect(m_clockFormatSwitcher, &DSwitchButton::checkedChanged, timeWidget, &TimeWidget::setIs24HourFormat);
    connect(m_autoSyncSwitcher, &DSwitchButton::checkedChanged, this, &Datetime::switchAutoSync);
    connect(&m_dbusInter, &DBusTimedate::NTPChanged, [this] () -> void {m_dateCtrlWidget->setVisible(!m_dbusInter.nTP());});
    connect(&m_dbusInter, &DBusTimedate::TimezoneChanged, this, &Datetime::showSelectedTimezoneList);
    connect(timeWidget, &TimeWidget::applyTime, [this] (const QDateTime & time) -> void {
                m_dbusInter.SetTime(time.currentMSecsSinceEpoch(), true);
    });
    connect(m_dateCtrlWidget, &DateControlWidget::applyDate, [this] () -> void {
                const QDate date = m_calendar->getSelectDate();
                const QTime time = QTime::currentTime();

                m_dbusInter.SetDate(date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second(), time.msec());
                m_calendar->resetCurrentDate(date);
    });

    qDebug() << getZoneCityListByOffset(m_dbusInter.GetZoneInfo(m_dbusInter.timezone()).argumentAt<0>().m_utcOffset);
}

Datetime::~Datetime()
{
    qDebug() << "~Datetime";

    m_frame->setParent(nullptr);
    m_frame->deleteLater();
}

QFrame* Datetime::getContent()
{
    return m_frame;
}

void Datetime::switchAutoSync(const bool autoSync)
{
    m_dbusInter.SetNTP(autoSync);
}

const QString Datetime::getUTCOffset(int offset)
{
    const QString offsetHour = QString::number(abs(offset) / 3600);
    const QString offsetMinute = QString::number((abs(offset) % 3600) / 60);

    return std::move(QString("UTC%1%2:%3").arg(offset >= 0 ? '+' : '-')
                                          .arg(offsetHour, 2, '0')
                                          .arg(offsetMinute, 2, '0'));
}

const QString Datetime::getZoneCityListByOffset(int zoneOffset)
{
    QStringList list;
    for (const ZoneInfo & zone : *m_zoneInfoList)
        if (zone.m_utcOffset == zoneOffset)
            list.append(zone.m_zoneCity);
        else if (zone.m_utcOffset > zoneOffset)
            break;

    return std::move(list.join(", "));
}

const ZoneInfo &Datetime::getZoneInfoByName(const QString &zoneName) const
{
    for (const ZoneInfo & zone : *m_zoneInfoList)
        if (zone.m_zoneName == zoneName)
            return zone;

    qWarning() << zoneName << "not in Timezone list!!!";

    // for default
    return m_zoneInfoList->first();
}

void Datetime::clearTimezoneList()
{
    // destruct old items in timezone listwidget.
    qDeleteAll(m_timezoneListWidget->children());
    QLayout *oldLayout = m_timezoneListWidget->layout();
    if (oldLayout)
    {
        oldLayout->setParent(nullptr);
        oldLayout->deleteLater();
    }

    m_timezoneListWidget->setLayout(nullptr);
}

void Datetime::showSelectedTimezoneList()
{
    //qDebug() << __LINE__ << __func__;
    clearTimezoneList();

    const QString userZone = m_dbusInter.timezone();
    QStringList zoneList = m_dbusInter.userTimezones();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    int zoneNums = 0;

    for (const QString & zone : zoneList)
    {
        // pass default Timezone
        if (zone == "Etc/UTC")
            continue;

        ++zoneNums;

        const ZoneInfo & zoneInfo = getZoneInfoByName(zone);
        qDebug() << zone << zoneInfo;

        TimezoneWidget *zoneWidget = new TimezoneWidget(&zoneInfo);
        zoneWidget->setZoneCities(getZoneCityListByOffset(zoneInfo.m_utcOffset));
        zoneWidget->setZoneUTCOffset(getUTCOffset(zoneInfo.m_utcOffset));
        zoneWidget->setSelected(userZone == zoneInfo.m_zoneName);

        connect(zoneWidget, &TimezoneWidget::clicked, [this, zoneWidget] () -> void {toggleTimeZone(zoneWidget);});

        mainLayout->addWidget(zoneWidget);
        mainLayout->addWidget(new DSeparatorHorizontal);
    }
    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    m_timezoneListWidget->setLayout(mainLayout);
    m_timezoneListWidget->setFixedHeight(50 * zoneNums);
}

void Datetime::showTimezoneList()
{
    clearTimezoneList();
/*
    QStringList zoneList = m_dbusInter.GetZoneList();
    QStringList userZoneList = m_dbusInter.userTimezones();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    int zoneNums = 0;

    for (const QString & zone : zoneList)
    {

    }*/
}

void Datetime::toRemoveTimezoneMode()
{
    // update zone list.
    showSelectedTimezoneList();
    const QObjectList list = m_timezoneListWidget->children();

    for (QObject * object : list)
    {
        TimezoneWidget *widget = qobject_cast<TimezoneWidget *>(object);
        if (!widget)
            continue;

        widget->removeMode();
        widget->disconnect();

        connect(widget, &TimezoneWidget::clicked, [this, widget] () -> void {removeTimeZone(widget);});
    }
}

void Datetime::toggleTimeZone(TimezoneWidget *zone)
{
    if (!zone)
        return;

    qDebug() << "toggle zone: " << zone->zoneName();
    m_dbusInter.SetTimezone(zone->zoneName());
}

void Datetime::removeTimeZone(TimezoneWidget *zone)
{
    if (!zone)
        return;

    qDebug() << "remove zone: " << zone->zoneName();
    m_dbusInter.DeleteUserTimezone(zone->zoneName());
    toRemoveTimezoneMode();
}
