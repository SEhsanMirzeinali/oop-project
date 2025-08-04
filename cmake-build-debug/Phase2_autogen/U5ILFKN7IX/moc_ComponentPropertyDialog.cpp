/****************************************************************************
** Meta object code from reading C++ file 'ComponentPropertyDialog.h'
**
<<<<<<< HEAD
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
=======
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../view/ui/ComponentPropertyDialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ComponentPropertyDialog.h' doesn't include <QObject>."
<<<<<<< HEAD
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
=======
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
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
<<<<<<< HEAD

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSComponentPropertyDialogENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSComponentPropertyDialogENDCLASS = QtMocHelpers::stringData(
    "ComponentPropertyDialog"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSComponentPropertyDialogENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject ComponentPropertyDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSComponentPropertyDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSComponentPropertyDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSComponentPropertyDialogENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ComponentPropertyDialog, std::true_type>
    >,
=======
struct qt_meta_tag_ZN23ComponentPropertyDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto ComponentPropertyDialog::qt_create_metaobjectdata<qt_meta_tag_ZN23ComponentPropertyDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ComponentPropertyDialog"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ComponentPropertyDialog, qt_meta_tag_ZN23ComponentPropertyDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ComponentPropertyDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23ComponentPropertyDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23ComponentPropertyDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN23ComponentPropertyDialogE_t>.metaTypes,
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
    nullptr
} };

void ComponentPropertyDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
<<<<<<< HEAD
    (void)_o;
    (void)_id;
    (void)_c;
=======
    auto *_t = static_cast<ComponentPropertyDialog *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
    (void)_a;
}

const QMetaObject *ComponentPropertyDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComponentPropertyDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
<<<<<<< HEAD
    if (!strcmp(_clname, qt_meta_stringdata_CLASSComponentPropertyDialogENDCLASS.stringdata0))
=======
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN23ComponentPropertyDialogE_t>.strings))
>>>>>>> e075db8 (Initial commit to Mmfotuhi branch)
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ComponentPropertyDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
