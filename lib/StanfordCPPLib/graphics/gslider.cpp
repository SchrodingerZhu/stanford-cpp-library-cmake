/*
 * File: GSlider.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gslider.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include <gslider.h>
#define INTERNAL_INCLUDE 1
#include <gthread.h>
#define INTERNAL_INCLUDE 1
#include <require.h>
#define INTERNAL_INCLUDE 1
#include <gwindow.h>
#undef INTERNAL_INCLUDE

const int GSlider::DEFAULT_MIN_VALUE = 0;
const int GSlider::DEFAULT_MAX_VALUE = 100;
const int GSlider::DEFAULT_INITIAL_VALUE = 50;

GSlider::GSlider(int min, int max, int value, QWidget* parent) {
    require::require(min <= max, "GSlider::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this,
                                          Qt::Horizontal,
                                          getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::GSlider(Orientation orientation, int min, int max, int value, QWidget* parent) {
    require::require(min <= max, "GSlider::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, orientation, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this,
                                          orientation == HORIZONTAL ? Qt::Horizontal : Qt::Vertical,
                                          getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::~GSlider() {
    // TODO: delete _iqslider;
    _iqslider = nullptr;
}

_Internal_QWidget* GSlider::getInternalWidget() const {
    return _iqslider;
}

int GSlider::getMajorTickSpacing() const {
    return _iqslider->tickInterval();
}

int GSlider::getMax() const {
    return _iqslider->maximum();
}

int GSlider::getMin() const {
    return _iqslider->minimum();
}

int GSlider::getMinorTickSpacing() const {
    return _iqslider->tickInterval();
}

GSlider::Orientation GSlider::getOrientation() const {
    return _iqslider->orientation() == Qt::Horizontal ? HORIZONTAL : VERTICAL;
}

bool GSlider::getPaintLabels() const {
    // TODO
    return false;
}

bool GSlider::getPaintTicks() const {
    // TODO
    return true;
}

bool GSlider::getSnapToTicks() const {
    // TODO
    return true;
}

std::string GSlider::getType() const {
    return "GSlider";
}

int GSlider::getValue() const {
    return _iqslider->value();
}

QWidget* GSlider::getWidget() const {
    return static_cast<QWidget*>(_iqslider);
}

void GSlider::removeActionListener() {
    removeEventListener("change");
}

void GSlider::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GSlider::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GSlider::setMajorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setMax(int max) {
    int min = getMin();
    require::require(min <= max, "GSlider::setMax", "max (" + std::to_string(max) + ") cannot be less than min (" + std::to_string(min) + ")");
    GThread::runOnQtGuiThread([this, max]() {
        _iqslider->setMaximum(max);
    });
}

void GSlider::setMin(int min) {
    int max = getMax();
    require::require(min <= max, "GSlider::setMin", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min]() {
        _iqslider->setMinimum(min);
    });
}

void GSlider::setMinorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setPaintLabels(bool /* value */) {
    // not supported
}

void GSlider::setPaintTicks(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickPosition(value ? QSlider::TicksBothSides : QSlider::NoTicks);
    });
}

void GSlider::setRange(int min, int max) {
    require::require(min <= max, "GSlider::setRange", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min, max]() {
        _iqslider->setRange(min, max);
    });
}

void GSlider::setSnapToTicks(bool /* value */) {
    // TODO
}

void GSlider::setState(int min, int max, int value) {
    require::require(min <= max, "GSlider::setState", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::setState", "value");
    GThread::runOnQtGuiThread([this, min, max, value]() {
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
}

void GSlider::setValue(int value) {
    require::inRange(value, getMin(), getMax(), "GSlider::setValue", "value");
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setValue(value);
    });
}


_Internal_QSlider::_Internal_QSlider(GSlider* gslider, Qt::Orientation orientation, QWidget* parent)
        : QSlider(orientation, parent),
          _gslider(gslider) {
    require::nonNull(gslider, "_Internal_QSlider::constructor");
    setObjectName(QString::fromStdString("_Internal_QSlider_" + std::to_string(gslider->getID())));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleChange(int)));
}

void _Internal_QSlider::handleChange(int /* value */) {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gslider);
    changeEvent.setActionCommand(_gslider->getActionCommand());
    _gslider->fireEvent(changeEvent);
}

QSize _Internal_QSlider::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSlider::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gslider.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES
