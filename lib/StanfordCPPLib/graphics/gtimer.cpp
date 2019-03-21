/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 * 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unneeded include statements
 */

#define INTERNAL_INCLUDE 1
#include <gtimer.h>
#define INTERNAL_INCLUDE 1
#include <error.h>
#define INTERNAL_INCLUDE 1
#include <gthread.h>
#define INTERNAL_INCLUDE 1
#include <gwindow.h>
#define INTERNAL_INCLUDE 1
#include <require.h>
#undef INTERNAL_INCLUDE

/* Implementation of the GTimer class */

GTimer::GTimer(double milliseconds)
        : _ms(0),
          _id(-1) {
    setDelay(milliseconds);
}

double GTimer::getDelay() const {
    return _ms;
}

bool GTimer::isStarted() const {
    return _id >= 0;
}

void GTimer::restart() {
    stop();
    start();
}

void GTimer::setDelay(double ms) {
    require::positive(ms, "GTimer::setDelay", "delay (ms)");
    _ms = ms;
    if (isStarted()) {
        restart();
    }
}

void GTimer::start() {
    _Internal_QMainWindow* lastWindow = static_cast<_Internal_QMainWindow*>(GWindow::getLastWindow());
    if (!lastWindow) {
        error("GTimer::start: You must create at least one GWindow before starting a GTimer.");
        return;
    }
    GThread::runOnQtGuiThreadAsync([this, lastWindow]() {
        _id = lastWindow->timerStart(_ms);
    });
}

void GTimer::stop() {
    if (isStarted()) {
        _Internal_QMainWindow* lastWindow = static_cast<_Internal_QMainWindow*>(GWindow::getLastWindow());
        if (!lastWindow) {
            error("GTimer::constructor: You must create at least one GWindow before stopping a GTimer.");
            return;
        }
        GThread::runOnQtGuiThreadAsync([this, lastWindow]() {
            lastWindow->timerStop(_id);
            _id = -1;
        });
    }
}
