/****************************************************************************
** Meta object code from reading C++ file 'ToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../view/ui/ToolBar.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSToolBarENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSToolBarENDCLASS = QtMocHelpers::stringData(
    "ToolBar",
    "newProject",
    "",
    "openProject",
    "saveProject",
    "undoAction",
    "redoAction",
    "spiceAnalysis",
    "probeAction",
    "newUnipolarAction",
    "openUnipolarAction",
    "openThevininAction",
    "openNortonAction",
    "newTNAction"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSToolBarENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x06,    1 /* Public */,
       3,    0,   87,    2, 0x06,    2 /* Public */,
       4,    0,   88,    2, 0x06,    3 /* Public */,
       5,    0,   89,    2, 0x06,    4 /* Public */,
       6,    0,   90,    2, 0x06,    5 /* Public */,
       7,    0,   91,    2, 0x06,    6 /* Public */,
       8,    0,   92,    2, 0x06,    7 /* Public */,
       9,    0,   93,    2, 0x06,    8 /* Public */,
      10,    0,   94,    2, 0x06,    9 /* Public */,
      11,    0,   95,    2, 0x06,   10 /* Public */,
      12,    0,   96,    2, 0x06,   11 /* Public */,
      13,    0,   97,    2, 0x06,   12 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ToolBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSToolBarENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSToolBarENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSToolBarENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ToolBar, std::true_type>,
        // method 'newProject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openProject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveProject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'undoAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'redoAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'spiceAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'probeAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newUnipolarAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openUnipolarAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openThevininAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openNortonAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newTNAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ToolBar *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newProject(); break;
        case 1: _t->openProject(); break;
        case 2: _t->saveProject(); break;
        case 3: _t->undoAction(); break;
        case 4: _t->redoAction(); break;
        case 5: _t->spiceAnalysis(); break;
        case 6: _t->probeAction(); break;
        case 7: _t->newUnipolarAction(); break;
        case 8: _t->openUnipolarAction(); break;
        case 9: _t->openThevininAction(); break;
        case 10: _t->openNortonAction(); break;
        case 11: _t->newTNAction(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::newProject; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::openProject; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::saveProject; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::undoAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::redoAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::spiceAnalysis; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::probeAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::newUnipolarAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::openUnipolarAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::openThevininAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::openNortonAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (ToolBar::*)();
            if (_t _q_method = &ToolBar::newTNAction; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *ToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSToolBarENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ToolBar::newProject()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ToolBar::openProject()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ToolBar::saveProject()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ToolBar::undoAction()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ToolBar::redoAction()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ToolBar::spiceAnalysis()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ToolBar::probeAction()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ToolBar::newUnipolarAction()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ToolBar::openUnipolarAction()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ToolBar::openThevininAction()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ToolBar::openNortonAction()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ToolBar::newTNAction()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}
QT_WARNING_POP
