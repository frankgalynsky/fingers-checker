/* Fingers checker
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "devicechecker.h"
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>


DeviceChecker::DeviceChecker(const shared_ptr<IConfigs> &cfg, const shared_ptr<INotify> &notify, const shared_ptr<ILog> &log)
{
    _cfg = cfg;
    _notify = notify;
    _log = log;
}

void DeviceChecker::checkDeviceLife()
{
    bool retVal = false;
    const auto &wc = _cfg->getWebCfg();
    const auto &time = boost::posix_time::second_clock::local_time();

    for (unsigned i = 0; i < DEV_COUNT; i++) {
        retVal = _notify->checkDevice(wc.devices[i]);
        if (!retVal) {
            try {
                _notify->sendTelegram("New%20Issue%0AStation:%20" + wc.devices[i] + "%0ADate:%20" +
                                      boost::lexical_cast<string>(time) + "%0AType:%20ERROR%0AIssue:%20Connection failed.");
            }
            catch(const string &err) {
                _log->local(err, LOG_WARNING);
            }
        }
    }

    /*
     * Next time cycle
     */
    _timer->expires_at(_timer->expires_at() + boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&DeviceChecker::checkDeviceLife, this));
}

void DeviceChecker::start()
{
    _timer = make_shared<deadline_timer>(_io, boost::posix_time::seconds(_interval));
    _timer->async_wait(boost::bind(&DeviceChecker::checkDeviceLife, this));
    _io.run();
}