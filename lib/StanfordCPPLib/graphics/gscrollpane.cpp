/*
 * File: gscrollpane.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/09/01
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include <gscrollpane.h>
#define INTERNAL_INCLUDE 1
#include <glayout.h>
#define INTERNAL_INCLUDE 1
#include <gthread.h>
#define INTERNAL_INCLUDE 1
#include <require.h>
#undef INTERNAL_INCLUDE

GScrollPane::GScrollPane(GInteractor* interactor, QWidget* parent)
        : _iqscrollarea(nullptr),
          _interactor(interactor),
          _horizontalScrollBarPolicy(GScrollPane::SCROLLBAR_AS_NEEDED),
          _verticalScrollBarPolicy(GScrollPane::SCROLLBAR_AS_NEEDED) {
    require::nonNull(interactor, "GScrollPane::constructor", "interactor");
    GThread::runOnQtGuiThread([this, interactor, parent]() {
        _iqscrollarea = new _Internal_QScrollArea(this, getInternalParent(parent));
        _iqscrollarea->setWidget(interactor->getWidget());
    });
    setInteractorStretch(true);
    setVisible(false);   // all widgets are not shown until added to a window
}

GScrollPane::~GScrollPane() {
    // TODO: delete _iqscrollarea;
    _iqscrollarea = nullptr;
}

GScrollPane::ScrollBarPolicy GScrollPane::getHorizontalScrollBarPolicy() const {
    return _horizontalScrollBarPolicy;
}

GInteractor* GScrollPane::getInteractor() const {
    return _interactor;
}

_Internal_QWidget* GScrollPane::getInternalWidget() const {
    return _iqscrollarea;
}

std::string GScrollPane::getType() const {
    return "GScrollPane";
}

GScrollPane::ScrollBarPolicy GScrollPane::getVerticalScrollBarPolicy() const {
    return _verticalScrollBarPolicy;
}

QWidget* GScrollPane::getWidget() const {
    return static_cast<QWidget*>(_iqscrollarea);
}

bool GScrollPane::isInteractorStretch() const {
    return _iqscrollarea->widgetResizable();
}

void GScrollPane::setHorizontalScrollBarPolicy(ScrollBarPolicy policy) {
    GThread::runOnQtGuiThread([this, policy]() {
        Qt::ScrollBarPolicy qtScrollBarPolicy = toQtScrollBarPolicy(policy);
        _iqscrollarea->setHorizontalScrollBarPolicy(qtScrollBarPolicy);
    });
}

void GScrollPane::setInteractorStretch(bool stretch) {
    GThread::runOnQtGuiThread([this, stretch]() {
        _iqscrollarea->setWidgetResizable(stretch);
        GLayout::forceUpdate(_iqscrollarea);
    });
}

void GScrollPane::setScrollBarPolicy(ScrollBarPolicy policy) {
    setHorizontalScrollBarPolicy(policy);
    setVerticalScrollBarPolicy(policy);
}

void GScrollPane::setVerticalScrollBarPolicy(ScrollBarPolicy policy) {
    GThread::runOnQtGuiThread([this, policy]() {
        Qt::ScrollBarPolicy qtScrollBarPolicy = toQtScrollBarPolicy(policy);
        _iqscrollarea->setVerticalScrollBarPolicy(qtScrollBarPolicy);
    });
}

Qt::ScrollBarPolicy GScrollPane::toQtScrollBarPolicy(ScrollBarPolicy policy) {
    switch (policy) {
        case GScrollPane::SCROLLBAR_ALWAYS:
            return Qt::ScrollBarAlwaysOn;
        case GScrollPane::SCROLLBAR_NEVER:
            return Qt::ScrollBarAlwaysOff;
        case GScrollPane::SCROLLBAR_AS_NEEDED:
        default:
            return Qt::ScrollBarAsNeeded;
    }
}


_Internal_QScrollArea::_Internal_QScrollArea(GScrollPane* gscrollpane, QWidget* parent)
        : QScrollArea(parent)
          /*_gscrollpane(gscrollpane)*/ {
    require::nonNull(gscrollpane, "_Internal_QScrollArea::constructor");
    setObjectName(QString::fromStdString("_Internal_QScrollArea_" + std::to_string(gscrollpane->getID())));
}

QSize _Internal_QScrollArea::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QScrollArea::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gscrollpane.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES
