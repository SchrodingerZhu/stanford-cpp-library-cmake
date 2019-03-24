/*
 * File: gspacer.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/10/06
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include <gspacer.h>
#define INTERNAL_INCLUDE 1
#include <gthread.h>
#define INTERNAL_INCLUDE 1
#include <require.h>
#undef INTERNAL_INCLUDE

GSpacer::GSpacer(double width, double height, QWidget* parent)
        : _iqspacer(nullptr) {
    require::nonNegative2D(width, height, "GSpacer::constructor", "width", "height");
    GThread::runOnQtGuiThread([this, width, height, parent]() {
        _iqspacer = new _Internal_QSpacer(this, width, height, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSpacer::~GSpacer() {
    // TODO: delete _iqscrollarea;
    _iqspacer = nullptr;
}

_Internal_QWidget* GSpacer::getInternalWidget() const {
    return _iqspacer;
}

std::string GSpacer::getType() const {
    return "GSpacer";
}

QWidget* GSpacer::getWidget() const {
    return static_cast<QWidget*>(_iqspacer);
}


_Internal_QSpacer::_Internal_QSpacer(GSpacer* gspacer, double width, double height, QWidget* parent)
        : QWidget(parent) {
    require::nonNull(gspacer, "_Internal_QSpacer::constructor");
    setObjectName(QString::fromStdString("_Internal_QSpacer_" + std::to_string(gspacer->getID())));
    setFixedSize(static_cast<int>(width), static_cast<int>(height));
}

QSize _Internal_QSpacer::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gspacer.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES
