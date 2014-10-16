/****************************************************************************
** Meta object code from reading C++ file 'ViewPort.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../ViewPort.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ViewPort.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ViewPort_t {
    QByteArrayData data[18];
    char stringdata[224];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ViewPort_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ViewPort_t qt_meta_stringdata_ViewPort = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 14),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 5),
QT_MOC_LITERAL(4, 31, 16),
QT_MOC_LITERAL(5, 48, 10),
QT_MOC_LITERAL(6, 59, 10),
QT_MOC_LITERAL(7, 70, 10),
QT_MOC_LITERAL(8, 81, 10),
QT_MOC_LITERAL(9, 92, 10),
QT_MOC_LITERAL(10, 103, 10),
QT_MOC_LITERAL(11, 114, 18),
QT_MOC_LITERAL(12, 133, 3),
QT_MOC_LITERAL(13, 137, 18),
QT_MOC_LITERAL(14, 156, 13),
QT_MOC_LITERAL(15, 170, 23),
QT_MOC_LITERAL(16, 194, 17),
QT_MOC_LITERAL(17, 212, 11)
    },
    "ViewPort\0texSizeChanged\0\0value\0"
    "openDisplayModel\0choosePosX\0chooseNegX\0"
    "choosePosY\0chooseNegY\0choosePosZ\0"
    "chooseNegZ\0updateMicrosurface\0val\0"
    "updateReflectivity\0updateFresnel\0"
    "updateDisplayModelScale\0updateSampleCount\0"
    "chooseColor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ViewPort[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x0a /* Public */,
       4,    0,   87,    2, 0x0a /* Public */,
       5,    0,   88,    2, 0x0a /* Public */,
       6,    0,   89,    2, 0x0a /* Public */,
       7,    0,   90,    2, 0x0a /* Public */,
       8,    0,   91,    2, 0x0a /* Public */,
       9,    0,   92,    2, 0x0a /* Public */,
      10,    0,   93,    2, 0x0a /* Public */,
      11,    1,   94,    2, 0x0a /* Public */,
      13,    1,   97,    2, 0x0a /* Public */,
      14,    1,  100,    2, 0x0a /* Public */,
      15,    1,  103,    2, 0x0a /* Public */,
      16,    1,  106,    2, 0x0a /* Public */,
      17,    0,  109,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

       0        // eod
};

void ViewPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ViewPort *_t = static_cast<ViewPort *>(_o);
        switch (_id) {
        case 0: _t->texSizeChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->openDisplayModel(); break;
        case 2: _t->choosePosX(); break;
        case 3: _t->chooseNegX(); break;
        case 4: _t->choosePosY(); break;
        case 5: _t->chooseNegY(); break;
        case 6: _t->choosePosZ(); break;
        case 7: _t->chooseNegZ(); break;
        case 8: _t->updateMicrosurface((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->updateReflectivity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->updateFresnel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->updateDisplayModelScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->updateSampleCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->chooseColor(); break;
        default: ;
        }
    }
}

const QMetaObject ViewPort::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_ViewPort.data,
      qt_meta_data_ViewPort,  qt_static_metacall, 0, 0}
};


const QMetaObject *ViewPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ViewPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ViewPort.stringdata))
        return static_cast<void*>(const_cast< ViewPort*>(this));
    if (!strcmp(_clname, "QGLFunctions"))
        return static_cast< QGLFunctions*>(const_cast< ViewPort*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int ViewPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
