/****************************************************************************
** Meta object code from reading C++ file 'backend.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../backend.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
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
struct qt_meta_tag_ZN16DashboardBackendE_t {};
} // unnamed namespace

template <> constexpr inline auto DashboardBackend::qt_create_metaobjectdata<qt_meta_tag_ZN16DashboardBackendE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DashboardBackend",
        "currentUserChanged",
        "",
        "vesselTempSetpointChanged",
        "pumpPressureSetpointChanged",
        "pumpRpmSetpointChanged",
        "chillerTempSetpointChanged",
        "dryAirFlowSetpointChanged",
        "sensorDataChanged",
        "runningChanged",
        "emergencyStoppedChanged",
        "runTimeChanged",
        "batchIdChanged",
        "recipeChanged",
        "pumpStateChanged",
        "loginFailed",
        "reason",
        "runSaved",
        "filepath",
        "simulateSensorData",
        "updateRunTime",
        "finishFlushing",
        "login",
        "username",
        "password",
        "logout",
        "startRun",
        "finishRun",
        "emergencyStop",
        "saveRun",
        "pumpOn",
        "pumpOff",
        "flushPump",
        "currentUser",
        "vesselTempSetpoint",
        "pumpPressureSetpoint",
        "pumpRpmSetpoint",
        "chillerTempSetpoint",
        "dryAirFlowSetpoint",
        "vesselTemp",
        "pumpPressure",
        "pumpRpm",
        "chillerTemp",
        "dryAirFlow",
        "running",
        "emergencyStopped",
        "runTimeString",
        "runElapsedSeconds",
        "batchId",
        "recipe",
        "recipeList",
        "pumpRunning",
        "flushing",
        "vesselTempHistory",
        "QVariantList",
        "pumpPressureHistory",
        "pumpRpmHistory",
        "chillerTempHistory",
        "dryAirFlowHistory",
        "timeHistory"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentUserChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'vesselTempSetpointChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pumpPressureSetpointChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pumpRpmSetpointChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'chillerTempSetpointChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dryAirFlowSetpointChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sensorDataChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runningChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'emergencyStoppedChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runTimeChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'batchIdChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'recipeChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pumpStateChanged'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loginFailed'
        QtMocHelpers::SignalData<void(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 },
        }}),
        // Signal 'runSaved'
        QtMocHelpers::SignalData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Slot 'simulateSensorData'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateRunTime'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'finishFlushing'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'login'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 23 }, { QMetaType::QString, 24 },
        }}),
        // Method 'logout'
        QtMocHelpers::MethodData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startRun'
        QtMocHelpers::MethodData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'finishRun'
        QtMocHelpers::MethodData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'emergencyStop'
        QtMocHelpers::MethodData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'saveRun'
        QtMocHelpers::MethodData<bool()>(29, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'pumpOn'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pumpOff'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'flushPump'
        QtMocHelpers::MethodData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentUser'
        QtMocHelpers::PropertyData<QString>(33, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'vesselTempSetpoint'
        QtMocHelpers::PropertyData<double>(34, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'pumpPressureSetpoint'
        QtMocHelpers::PropertyData<double>(35, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'pumpRpmSetpoint'
        QtMocHelpers::PropertyData<double>(36, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'chillerTempSetpoint'
        QtMocHelpers::PropertyData<double>(37, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'dryAirFlowSetpoint'
        QtMocHelpers::PropertyData<double>(38, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'vesselTemp'
        QtMocHelpers::PropertyData<double>(39, QMetaType::Double, QMC::DefaultPropertyFlags, 6),
        // property 'pumpPressure'
        QtMocHelpers::PropertyData<double>(40, QMetaType::Double, QMC::DefaultPropertyFlags, 6),
        // property 'pumpRpm'
        QtMocHelpers::PropertyData<double>(41, QMetaType::Double, QMC::DefaultPropertyFlags, 6),
        // property 'chillerTemp'
        QtMocHelpers::PropertyData<double>(42, QMetaType::Double, QMC::DefaultPropertyFlags, 6),
        // property 'dryAirFlow'
        QtMocHelpers::PropertyData<double>(43, QMetaType::Double, QMC::DefaultPropertyFlags, 6),
        // property 'running'
        QtMocHelpers::PropertyData<bool>(44, QMetaType::Bool, QMC::DefaultPropertyFlags, 7),
        // property 'emergencyStopped'
        QtMocHelpers::PropertyData<bool>(45, QMetaType::Bool, QMC::DefaultPropertyFlags, 8),
        // property 'runTimeString'
        QtMocHelpers::PropertyData<QString>(46, QMetaType::QString, QMC::DefaultPropertyFlags, 9),
        // property 'runElapsedSeconds'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags, 9),
        // property 'batchId'
        QtMocHelpers::PropertyData<QString>(48, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 10),
        // property 'recipe'
        QtMocHelpers::PropertyData<QString>(49, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 11),
        // property 'recipeList'
        QtMocHelpers::PropertyData<QStringList>(50, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'pumpRunning'
        QtMocHelpers::PropertyData<bool>(51, QMetaType::Bool, QMC::DefaultPropertyFlags, 12),
        // property 'flushing'
        QtMocHelpers::PropertyData<bool>(52, QMetaType::Bool, QMC::DefaultPropertyFlags, 12),
        // property 'vesselTempHistory'
        QtMocHelpers::PropertyData<QVariantList>(53, 0x80000000 | 54, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'pumpPressureHistory'
        QtMocHelpers::PropertyData<QVariantList>(55, 0x80000000 | 54, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'pumpRpmHistory'
        QtMocHelpers::PropertyData<QVariantList>(56, 0x80000000 | 54, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'chillerTempHistory'
        QtMocHelpers::PropertyData<QVariantList>(57, 0x80000000 | 54, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'dryAirFlowHistory'
        QtMocHelpers::PropertyData<QVariantList>(58, 0x80000000 | 54, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
        // property 'timeHistory'
        QtMocHelpers::PropertyData<QVariantList>(59, 0x80000000 | 54, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DashboardBackend, qt_meta_tag_ZN16DashboardBackendE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DashboardBackend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DashboardBackendE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DashboardBackendE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16DashboardBackendE_t>.metaTypes,
    nullptr
} };

void DashboardBackend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DashboardBackend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentUserChanged(); break;
        case 1: _t->vesselTempSetpointChanged(); break;
        case 2: _t->pumpPressureSetpointChanged(); break;
        case 3: _t->pumpRpmSetpointChanged(); break;
        case 4: _t->chillerTempSetpointChanged(); break;
        case 5: _t->dryAirFlowSetpointChanged(); break;
        case 6: _t->sensorDataChanged(); break;
        case 7: _t->runningChanged(); break;
        case 8: _t->emergencyStoppedChanged(); break;
        case 9: _t->runTimeChanged(); break;
        case 10: _t->batchIdChanged(); break;
        case 11: _t->recipeChanged(); break;
        case 12: _t->pumpStateChanged(); break;
        case 13: _t->loginFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->runSaved((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->simulateSensorData(); break;
        case 16: _t->updateRunTime(); break;
        case 17: _t->finishFlushing(); break;
        case 18: { bool _r = _t->login((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 19: _t->logout(); break;
        case 20: _t->startRun(); break;
        case 21: _t->finishRun(); break;
        case 22: _t->emergencyStop(); break;
        case 23: { bool _r = _t->saveRun();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->pumpOn(); break;
        case 25: _t->pumpOff(); break;
        case 26: _t->flushPump(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::currentUserChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::vesselTempSetpointChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::pumpPressureSetpointChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::pumpRpmSetpointChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::chillerTempSetpointChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::dryAirFlowSetpointChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::sensorDataChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::runningChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::emergencyStoppedChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::runTimeChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::batchIdChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::recipeChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)()>(_a, &DashboardBackend::pumpStateChanged, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)(const QString & )>(_a, &DashboardBackend::loginFailed, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (DashboardBackend::*)(const QString & )>(_a, &DashboardBackend::runSaved, 14))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->currentUser(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->vesselTempSetpoint(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->pumpPressureSetpoint(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->pumpRpmSetpoint(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->chillerTempSetpoint(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->dryAirFlowSetpoint(); break;
        case 6: *reinterpret_cast<double*>(_v) = _t->vesselTemp(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->pumpPressure(); break;
        case 8: *reinterpret_cast<double*>(_v) = _t->pumpRpm(); break;
        case 9: *reinterpret_cast<double*>(_v) = _t->chillerTemp(); break;
        case 10: *reinterpret_cast<double*>(_v) = _t->dryAirFlow(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->running(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->emergencyStopped(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->runTimeString(); break;
        case 14: *reinterpret_cast<int*>(_v) = _t->runElapsedSeconds(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->batchId(); break;
        case 16: *reinterpret_cast<QString*>(_v) = _t->recipe(); break;
        case 17: *reinterpret_cast<QStringList*>(_v) = _t->recipeList(); break;
        case 18: *reinterpret_cast<bool*>(_v) = _t->pumpRunning(); break;
        case 19: *reinterpret_cast<bool*>(_v) = _t->flushing(); break;
        case 20: *reinterpret_cast<QVariantList*>(_v) = _t->vesselTempHistory(); break;
        case 21: *reinterpret_cast<QVariantList*>(_v) = _t->pumpPressureHistory(); break;
        case 22: *reinterpret_cast<QVariantList*>(_v) = _t->pumpRpmHistory(); break;
        case 23: *reinterpret_cast<QVariantList*>(_v) = _t->chillerTempHistory(); break;
        case 24: *reinterpret_cast<QVariantList*>(_v) = _t->dryAirFlowHistory(); break;
        case 25: *reinterpret_cast<QVariantList*>(_v) = _t->timeHistory(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setVesselTempSetpoint(*reinterpret_cast<double*>(_v)); break;
        case 2: _t->setPumpPressureSetpoint(*reinterpret_cast<double*>(_v)); break;
        case 3: _t->setPumpRpmSetpoint(*reinterpret_cast<double*>(_v)); break;
        case 4: _t->setChillerTempSetpoint(*reinterpret_cast<double*>(_v)); break;
        case 5: _t->setDryAirFlowSetpoint(*reinterpret_cast<double*>(_v)); break;
        case 15: _t->setBatchId(*reinterpret_cast<QString*>(_v)); break;
        case 16: _t->setRecipe(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *DashboardBackend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DashboardBackend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16DashboardBackendE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DashboardBackend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 27;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void DashboardBackend::currentUserChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DashboardBackend::vesselTempSetpointChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DashboardBackend::pumpPressureSetpointChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DashboardBackend::pumpRpmSetpointChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DashboardBackend::chillerTempSetpointChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void DashboardBackend::dryAirFlowSetpointChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void DashboardBackend::sensorDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void DashboardBackend::runningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void DashboardBackend::emergencyStoppedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void DashboardBackend::runTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void DashboardBackend::batchIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void DashboardBackend::recipeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void DashboardBackend::pumpStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void DashboardBackend::loginFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void DashboardBackend::runSaved(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1);
}
QT_WARNING_POP
