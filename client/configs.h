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

#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include <memory>

using namespace std;

typedef struct {
    string username;
    string passwd;
    string devices[3];
    string printers[3];
} WebCfg;

typedef struct {
    string ip;
    unsigned port;
} ServerCfg;

typedef struct {
    unsigned interval;
} CheckerCfg;

typedef struct {
    string ip;
    string user;
    string passwd;
    string base;
} DatabaseCfg;


class IConfigs
{
public:
    virtual const WebCfg& getWebCfg(void) const = 0;
    virtual const CheckerCfg& getCheckerCfg(void) const = 0;
    virtual const ServerCfg& getServerCfg(void) const = 0;
    virtual const DatabaseCfg& getDatabaseCfg(void) const = 0;
    virtual void load(const string &filename) = 0;
};


class Configs: public IConfigs
{
private:
    WebCfg wc;
    ServerCfg sc;
    CheckerCfg cc;
    DatabaseCfg dbc;

public:
    inline const DatabaseCfg& getDatabaseCfg(void) const {
        return dbc;
    }

    inline const CheckerCfg& getCheckerCfg(void) const {
        return cc;
    }

    inline const WebCfg& getWebCfg(void) const {
        return wc;
    }

    inline const ServerCfg& getServerCfg(void) const {
        return sc;
    }

    /*
     * Loading configs from json file
     */
    void load(const string &filename);
};


#endif