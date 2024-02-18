#include "inioptions.h"

IniOptions::IniOptions(Settings *settings) : MapSettings(settings) { }

void IniOptions::resetToDefault()
{
    setLogDebug(false);
    setLogConsole(false);

    setNoServiceControl(false);
    setCheckPasswordOnUninstall(false);

    setQuotaDayAlerted(0);
    setQuotaMonthAlerted(0);
    setQuotaDayMb(0);
    setQuotaMonthMb(0);
    setQuotaBlockInternet(false);

    setMonthStart(DEFAULT_MONTH_START);
    setTrafHourKeepDays(DEFAULT_TRAF_HOUR_KEEP_DAYS);
    setTrafDayKeepDays(DEFAULT_TRAF_DAY_KEEP_DAYS);
    setTrafMonthKeepMonths(DEFAULT_TRAF_MONTH_KEEP_MONTHS);

    setAllowedIpKeepCount(DEFAULT_LOG_IP_KEEP_COUNT);
    setBlockedIpKeepCount(DEFAULT_LOG_IP_KEEP_COUNT);

    setProgPurgeOnMounted(false);

    setGraphWindowAlwaysOnTop(true);
    setGraphWindowFrameless(false);
    setGraphWindowClickThrough(false);
    setGraphWindowHideOnHover(false);
    setGraphWindowOpacity(graphWindowOpacityDefault());
    setGraphWindowHoverOpacity(graphWindowHoverOpacityDefault());
    setGraphWindowMaxSeconds(graphWindowMaxSecondsDefault());
    setGraphWindowColor(graphWindowColorDefault());
    setGraphWindowColorIn(graphWindowColorInDefault());
    setGraphWindowColorOut(graphWindowColorOutDefault());
    setGraphWindowAxisColor(graphWindowAxisColorDefault());
    setGraphWindowTickLabelColor(graphWindowTickLabelColorDefault());
    setGraphWindowLabelColor(graphWindowLabelColorDefault());
    setGraphWindowGridColor(graphWindowGridColorDefault());
}
