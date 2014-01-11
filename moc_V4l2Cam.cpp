/****************************************************************************
** Meta object code from reading C++ file 'V4l2Cam.h'
**
** Created: Sat Jan 11 19:09:09 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "V4l2Cam.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'V4l2Cam.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Test[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
      17,    5,    5,    5, 0x08,
      28,    5,    5,    5, 0x08,
      41,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Test[] = {
    "Test\0\0oneFrame()\0testTime()\0testThread()\0"
    "switchVideo(QString)\0"
};

void Test::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Test *_t = static_cast<Test *>(_o);
        switch (_id) {
        case 0: _t->oneFrame(); break;
        case 1: _t->testTime(); break;
        case 2: _t->testThread(); break;
        case 3: _t->switchVideo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Test::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Test::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Test,
      qt_meta_data_Test, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Test::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Test::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Test::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Test))
        return static_cast<void*>(const_cast< Test*>(this));
    return QObject::qt_metacast(_clname);
}

int Test::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Test::oneFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_V4l2Cam[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,    8,    8,    8, 0x08,
      42,    8,    8,    8, 0x08,
      67,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_V4l2Cam[] = {
    "V4l2Cam\0\0switchSignal(QString)\0"
    "cam_show()\0paintEvent(QPaintEvent*)\0"
    "switchVideo()\0"
};

void V4l2Cam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        V4l2Cam *_t = static_cast<V4l2Cam *>(_o);
        switch (_id) {
        case 0: _t->switchSignal((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->cam_show(); break;
        case 2: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 3: _t->switchVideo(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData V4l2Cam::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject V4l2Cam::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_V4l2Cam,
      qt_meta_data_V4l2Cam, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &V4l2Cam::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *V4l2Cam::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *V4l2Cam::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_V4l2Cam))
        return static_cast<void*>(const_cast< V4l2Cam*>(this));
    return QWidget::qt_metacast(_clname);
}

int V4l2Cam::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void V4l2Cam::switchSignal(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
