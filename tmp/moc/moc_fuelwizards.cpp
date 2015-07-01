/****************************************************************************
** FuelWizard meta object code from reading C++ file 'fuelwizards.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../fuelwizards.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FuelWizard::className() const
{
    return "FuelWizard";
}

QMetaObject *FuelWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelWizard( "FuelWizard", &FuelWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = WizardDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelWizard" ) )
	return this;
    return WizardDialog::qt_cast( clname );
}

bool FuelWizard::qt_invoke( int _id, QUObject* _o )
{
    return WizardDialog::qt_invoke(_id,_o);
}

bool FuelWizard::qt_emit( int _id, QUObject* _o )
{
    return WizardDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelWizard::qt_property( int id, int f, QVariant* v)
{
    return WizardDialog::qt_property( id, f, v);
}

bool FuelWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelBedDepthWizard::className() const
{
    return "FuelBedDepthWizard";
}

QMetaObject *FuelBedDepthWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelBedDepthWizard( "FuelBedDepthWizard", &FuelBedDepthWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelBedDepthWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelBedDepthWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelBedDepthWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelBedDepthWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelBedDepthWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelBedDepthWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelBedDepthWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelBedDepthWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelBedDepthWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelBedDepthWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelBedDepthWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelBedDepthWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelBedDepthWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelBedMextDeadWizard::className() const
{
    return "FuelBedMextDeadWizard";
}

QMetaObject *FuelBedMextDeadWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelBedMextDeadWizard( "FuelBedMextDeadWizard", &FuelBedMextDeadWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelBedMextDeadWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelBedMextDeadWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelBedMextDeadWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelBedMextDeadWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelBedMextDeadWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelBedMextDeadWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelBedMextDeadWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelBedMextDeadWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelBedMextDeadWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelBedMextDeadWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelBedMextDeadWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelBedMextDeadWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelBedMextDeadWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelHeatWizard::className() const
{
    return "FuelHeatWizard";
}

QMetaObject *FuelHeatWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelHeatWizard( "FuelHeatWizard", &FuelHeatWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelHeatWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelHeatWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelHeatWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelHeatWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelHeatWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelHeatWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelHeatWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelHeatWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelHeatWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelHeatWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelHeatWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelHeatWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelHeatWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelLoadDead1Wizard::className() const
{
    return "FuelLoadDead1Wizard";
}

QMetaObject *FuelLoadDead1Wizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelLoadDead1Wizard( "FuelLoadDead1Wizard", &FuelLoadDead1Wizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelLoadDead1Wizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadDead1Wizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelLoadDead1Wizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadDead1Wizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelLoadDead1Wizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelLoadDead1Wizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelLoadDead1Wizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelLoadDead1Wizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelLoadDead1Wizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelLoadDead1Wizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelLoadDead1Wizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelLoadDead1Wizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelLoadDead1Wizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelLoadDead10Wizard::className() const
{
    return "FuelLoadDead10Wizard";
}

QMetaObject *FuelLoadDead10Wizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelLoadDead10Wizard( "FuelLoadDead10Wizard", &FuelLoadDead10Wizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelLoadDead10Wizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadDead10Wizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelLoadDead10Wizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadDead10Wizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelLoadDead10Wizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelLoadDead10Wizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelLoadDead10Wizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelLoadDead10Wizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelLoadDead10Wizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelLoadDead10Wizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelLoadDead10Wizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelLoadDead10Wizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelLoadDead10Wizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelLoadDead100Wizard::className() const
{
    return "FuelLoadDead100Wizard";
}

QMetaObject *FuelLoadDead100Wizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelLoadDead100Wizard( "FuelLoadDead100Wizard", &FuelLoadDead100Wizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelLoadDead100Wizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadDead100Wizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelLoadDead100Wizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadDead100Wizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelLoadDead100Wizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelLoadDead100Wizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelLoadDead100Wizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelLoadDead100Wizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelLoadDead100Wizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelLoadDead100Wizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelLoadDead100Wizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelLoadDead100Wizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelLoadDead100Wizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelLoadLiveWizard::className() const
{
    return "FuelLoadLiveWizard";
}

QMetaObject *FuelLoadLiveWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelLoadLiveWizard( "FuelLoadLiveWizard", &FuelLoadLiveWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelLoadLiveWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadLiveWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelLoadLiveWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelLoadLiveWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelLoadLiveWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelLoadLiveWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelLoadLiveWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelLoadLiveWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelLoadLiveWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelLoadLiveWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelLoadLiveWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelLoadLiveWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelLoadLiveWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelSavrDead1Wizard::className() const
{
    return "FuelSavrDead1Wizard";
}

QMetaObject *FuelSavrDead1Wizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelSavrDead1Wizard( "FuelSavrDead1Wizard", &FuelSavrDead1Wizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelSavrDead1Wizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelSavrDead1Wizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelSavrDead1Wizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelSavrDead1Wizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelSavrDead1Wizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelSavrDead1Wizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelSavrDead1Wizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelSavrDead1Wizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelSavrDead1Wizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelSavrDead1Wizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelSavrDead1Wizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelSavrDead1Wizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelSavrDead1Wizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FuelSavrLiveWizard::className() const
{
    return "FuelSavrLiveWizard";
}

QMetaObject *FuelSavrLiveWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FuelSavrLiveWizard( "FuelSavrLiveWizard", &FuelSavrLiveWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FuelSavrLiveWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelSavrLiveWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FuelSavrLiveWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FuelSavrLiveWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FuelSavrLiveWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FuelSavrLiveWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FuelSavrLiveWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FuelSavrLiveWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FuelSavrLiveWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool FuelSavrLiveWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool FuelSavrLiveWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FuelSavrLiveWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool FuelSavrLiveWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *HeatPerUnitAreaWizard::className() const
{
    return "HeatPerUnitAreaWizard";
}

QMetaObject *HeatPerUnitAreaWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HeatPerUnitAreaWizard( "HeatPerUnitAreaWizard", &HeatPerUnitAreaWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HeatPerUnitAreaWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HeatPerUnitAreaWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HeatPerUnitAreaWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HeatPerUnitAreaWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HeatPerUnitAreaWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"HeatPerUnitAreaWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HeatPerUnitAreaWizard.setMetaObject( metaObj );
    return metaObj;
}

void* HeatPerUnitAreaWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HeatPerUnitAreaWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool HeatPerUnitAreaWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool HeatPerUnitAreaWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HeatPerUnitAreaWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool HeatPerUnitAreaWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PalmettoAgeWizard::className() const
{
    return "PalmettoAgeWizard";
}

QMetaObject *PalmettoAgeWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PalmettoAgeWizard( "PalmettoAgeWizard", &PalmettoAgeWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PalmettoAgeWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PalmettoAgeWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PalmettoAgeWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PalmettoAgeWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PalmettoAgeWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"PalmettoAgeWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PalmettoAgeWizard.setMetaObject( metaObj );
    return metaObj;
}

void* PalmettoAgeWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PalmettoAgeWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool PalmettoAgeWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool PalmettoAgeWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PalmettoAgeWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool PalmettoAgeWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PalmettoCoverWizard::className() const
{
    return "PalmettoCoverWizard";
}

QMetaObject *PalmettoCoverWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PalmettoCoverWizard( "PalmettoCoverWizard", &PalmettoCoverWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PalmettoCoverWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PalmettoCoverWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PalmettoCoverWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PalmettoCoverWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PalmettoCoverWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"PalmettoCoverWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PalmettoCoverWizard.setMetaObject( metaObj );
    return metaObj;
}

void* PalmettoCoverWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PalmettoCoverWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool PalmettoCoverWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool PalmettoCoverWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PalmettoCoverWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool PalmettoCoverWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *PalmettoOverstoryBasalAreaWizard::className() const
{
    return "PalmettoOverstoryBasalAreaWizard";
}

QMetaObject *PalmettoOverstoryBasalAreaWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PalmettoOverstoryBasalAreaWizard( "PalmettoOverstoryBasalAreaWizard", &PalmettoOverstoryBasalAreaWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PalmettoOverstoryBasalAreaWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PalmettoOverstoryBasalAreaWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PalmettoOverstoryBasalAreaWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PalmettoOverstoryBasalAreaWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PalmettoOverstoryBasalAreaWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = FuelWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"PalmettoOverstoryBasalAreaWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PalmettoOverstoryBasalAreaWizard.setMetaObject( metaObj );
    return metaObj;
}

void* PalmettoOverstoryBasalAreaWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PalmettoOverstoryBasalAreaWizard" ) )
	return this;
    return FuelWizard::qt_cast( clname );
}

bool PalmettoOverstoryBasalAreaWizard::qt_invoke( int _id, QUObject* _o )
{
    return FuelWizard::qt_invoke(_id,_o);
}

bool PalmettoOverstoryBasalAreaWizard::qt_emit( int _id, QUObject* _o )
{
    return FuelWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PalmettoOverstoryBasalAreaWizard::qt_property( int id, int f, QVariant* v)
{
    return FuelWizard::qt_property( id, f, v);
}

bool PalmettoOverstoryBasalAreaWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
