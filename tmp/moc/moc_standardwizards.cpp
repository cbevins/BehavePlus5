/****************************************************************************
** StandardWizard meta object code from reading C++ file 'standardwizards.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../standardwizards.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *StandardWizard::className() const
{
    return "StandardWizard";
}

QMetaObject *StandardWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_StandardWizard( "StandardWizard", &StandardWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString StandardWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StandardWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString StandardWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "StandardWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* StandardWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = WizardDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"StandardWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_StandardWizard.setMetaObject( metaObj );
    return metaObj;
}

void* StandardWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "StandardWizard" ) )
	return this;
    return WizardDialog::qt_cast( clname );
}

bool StandardWizard::qt_invoke( int _id, QUObject* _o )
{
    return WizardDialog::qt_invoke(_id,_o);
}

bool StandardWizard::qt_emit( int _id, QUObject* _o )
{
    return WizardDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool StandardWizard::qt_property( int id, int f, QVariant* v)
{
    return WizardDialog::qt_property( id, f, v);
}

bool StandardWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *BarkThicknessWizard::className() const
{
    return "BarkThicknessWizard";
}

QMetaObject *BarkThicknessWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BarkThicknessWizard( "BarkThicknessWizard", &BarkThicknessWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BarkThicknessWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BarkThicknessWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BarkThicknessWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BarkThicknessWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BarkThicknessWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"BarkThicknessWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BarkThicknessWizard.setMetaObject( metaObj );
    return metaObj;
}

void* BarkThicknessWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BarkThicknessWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool BarkThicknessWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool BarkThicknessWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BarkThicknessWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool BarkThicknessWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CanopyBulkDensityWizard::className() const
{
    return "CanopyBulkDensityWizard";
}

QMetaObject *CanopyBulkDensityWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CanopyBulkDensityWizard( "CanopyBulkDensityWizard", &CanopyBulkDensityWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CanopyBulkDensityWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CanopyBulkDensityWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CanopyBulkDensityWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CanopyBulkDensityWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CanopyBulkDensityWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CanopyBulkDensityWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CanopyBulkDensityWizard.setMetaObject( metaObj );
    return metaObj;
}

void* CanopyBulkDensityWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CanopyBulkDensityWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool CanopyBulkDensityWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool CanopyBulkDensityWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CanopyBulkDensityWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool CanopyBulkDensityWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CompassNorthWizard::className() const
{
    return "CompassNorthWizard";
}

QMetaObject *CompassNorthWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CompassNorthWizard( "CompassNorthWizard", &CompassNorthWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CompassNorthWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CompassNorthWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CompassNorthWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CompassNorthWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CompassNorthWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CompassNorthWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CompassNorthWizard.setMetaObject( metaObj );
    return metaObj;
}

void* CompassNorthWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CompassNorthWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool CompassNorthWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool CompassNorthWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CompassNorthWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool CompassNorthWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CompassUpslopeWizard::className() const
{
    return "CompassUpslopeWizard";
}

QMetaObject *CompassUpslopeWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CompassUpslopeWizard( "CompassUpslopeWizard", &CompassUpslopeWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CompassUpslopeWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CompassUpslopeWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CompassUpslopeWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CompassUpslopeWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CompassUpslopeWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CompassUpslopeWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CompassUpslopeWizard.setMetaObject( metaObj );
    return metaObj;
}

void* CompassUpslopeWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CompassUpslopeWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool CompassUpslopeWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool CompassUpslopeWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CompassUpslopeWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool CompassUpslopeWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *CrownRatioWizard::className() const
{
    return "CrownRatioWizard";
}

QMetaObject *CrownRatioWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CrownRatioWizard( "CrownRatioWizard", &CrownRatioWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CrownRatioWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CrownRatioWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CrownRatioWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CrownRatioWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CrownRatioWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"CrownRatioWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CrownRatioWizard.setMetaObject( metaObj );
    return metaObj;
}

void* CrownRatioWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CrownRatioWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool CrownRatioWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool CrownRatioWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CrownRatioWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool CrownRatioWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *FireLineIntWizard::className() const
{
    return "FireLineIntWizard";
}

QMetaObject *FireLineIntWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FireLineIntWizard( "FireLineIntWizard", &FireLineIntWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FireLineIntWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FireLineIntWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FireLineIntWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FireLineIntWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FireLineIntWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"FireLineIntWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FireLineIntWizard.setMetaObject( metaObj );
    return metaObj;
}

void* FireLineIntWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FireLineIntWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool FireLineIntWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool FireLineIntWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FireLineIntWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool FireLineIntWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *LiveMoistureWizard::className() const
{
    return "LiveMoistureWizard";
}

QMetaObject *LiveMoistureWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LiveMoistureWizard( "LiveMoistureWizard", &LiveMoistureWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LiveMoistureWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LiveMoistureWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LiveMoistureWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LiveMoistureWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LiveMoistureWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"LiveMoistureWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LiveMoistureWizard.setMetaObject( metaObj );
    return metaObj;
}

void* LiveMoistureWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LiveMoistureWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool LiveMoistureWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool LiveMoistureWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool LiveMoistureWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool LiveMoistureWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *LiveHerbMoistureWizard::className() const
{
    return "LiveHerbMoistureWizard";
}

QMetaObject *LiveHerbMoistureWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LiveHerbMoistureWizard( "LiveHerbMoistureWizard", &LiveHerbMoistureWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LiveHerbMoistureWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LiveHerbMoistureWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LiveHerbMoistureWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LiveHerbMoistureWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LiveHerbMoistureWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"LiveHerbMoistureWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LiveHerbMoistureWizard.setMetaObject( metaObj );
    return metaObj;
}

void* LiveHerbMoistureWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LiveHerbMoistureWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool LiveHerbMoistureWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool LiveHerbMoistureWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool LiveHerbMoistureWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool LiveHerbMoistureWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *LiveWoodMoistureWizard::className() const
{
    return "LiveWoodMoistureWizard";
}

QMetaObject *LiveWoodMoistureWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LiveWoodMoistureWizard( "LiveWoodMoistureWizard", &LiveWoodMoistureWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LiveWoodMoistureWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LiveWoodMoistureWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LiveWoodMoistureWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LiveWoodMoistureWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LiveWoodMoistureWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"LiveWoodMoistureWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LiveWoodMoistureWizard.setMetaObject( metaObj );
    return metaObj;
}

void* LiveWoodMoistureWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LiveWoodMoistureWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool LiveWoodMoistureWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool LiveWoodMoistureWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool LiveWoodMoistureWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool LiveWoodMoistureWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *MapFractionWizard::className() const
{
    return "MapFractionWizard";
}

QMetaObject *MapFractionWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MapFractionWizard( "MapFractionWizard", &MapFractionWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MapFractionWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MapFractionWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MapFractionWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MapFractionWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MapFractionWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = WizardDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MapFractionWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MapFractionWizard.setMetaObject( metaObj );
    return metaObj;
}

void* MapFractionWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MapFractionWizard" ) )
	return this;
    return WizardDialog::qt_cast( clname );
}

bool MapFractionWizard::qt_invoke( int _id, QUObject* _o )
{
    return WizardDialog::qt_invoke(_id,_o);
}

bool MapFractionWizard::qt_emit( int _id, QUObject* _o )
{
    return WizardDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MapFractionWizard::qt_property( int id, int f, QVariant* v)
{
    return WizardDialog::qt_property( id, f, v);
}

bool MapFractionWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SafetyZoneEquipmentAreaWizard::className() const
{
    return "SafetyZoneEquipmentAreaWizard";
}

QMetaObject *SafetyZoneEquipmentAreaWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SafetyZoneEquipmentAreaWizard( "SafetyZoneEquipmentAreaWizard", &SafetyZoneEquipmentAreaWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SafetyZoneEquipmentAreaWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SafetyZoneEquipmentAreaWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SafetyZoneEquipmentAreaWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SafetyZoneEquipmentAreaWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SafetyZoneEquipmentAreaWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"SafetyZoneEquipmentAreaWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SafetyZoneEquipmentAreaWizard.setMetaObject( metaObj );
    return metaObj;
}

void* SafetyZoneEquipmentAreaWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SafetyZoneEquipmentAreaWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool SafetyZoneEquipmentAreaWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool SafetyZoneEquipmentAreaWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SafetyZoneEquipmentAreaWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool SafetyZoneEquipmentAreaWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SafetyZonePersonnelAreaWizard::className() const
{
    return "SafetyZonePersonnelAreaWizard";
}

QMetaObject *SafetyZonePersonnelAreaWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SafetyZonePersonnelAreaWizard( "SafetyZonePersonnelAreaWizard", &SafetyZonePersonnelAreaWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SafetyZonePersonnelAreaWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SafetyZonePersonnelAreaWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SafetyZonePersonnelAreaWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SafetyZonePersonnelAreaWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SafetyZonePersonnelAreaWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"SafetyZonePersonnelAreaWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SafetyZonePersonnelAreaWizard.setMetaObject( metaObj );
    return metaObj;
}

void* SafetyZonePersonnelAreaWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SafetyZonePersonnelAreaWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool SafetyZonePersonnelAreaWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool SafetyZonePersonnelAreaWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SafetyZonePersonnelAreaWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool SafetyZonePersonnelAreaWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SlopeDegreesWizard::className() const
{
    return "SlopeDegreesWizard";
}

QMetaObject *SlopeDegreesWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SlopeDegreesWizard( "SlopeDegreesWizard", &SlopeDegreesWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SlopeDegreesWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SlopeDegreesWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SlopeDegreesWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SlopeDegreesWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SlopeDegreesWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"SlopeDegreesWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SlopeDegreesWizard.setMetaObject( metaObj );
    return metaObj;
}

void* SlopeDegreesWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SlopeDegreesWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool SlopeDegreesWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool SlopeDegreesWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SlopeDegreesWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool SlopeDegreesWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SlopeFractionWizard::className() const
{
    return "SlopeFractionWizard";
}

QMetaObject *SlopeFractionWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SlopeFractionWizard( "SlopeFractionWizard", &SlopeFractionWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SlopeFractionWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SlopeFractionWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SlopeFractionWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SlopeFractionWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SlopeFractionWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"SlopeFractionWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SlopeFractionWizard.setMetaObject( metaObj );
    return metaObj;
}

void* SlopeFractionWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SlopeFractionWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool SlopeFractionWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool SlopeFractionWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SlopeFractionWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool SlopeFractionWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *WindAdjWizard::className() const
{
    return "WindAdjWizard";
}

QMetaObject *WindAdjWizard::metaObj = 0;
static QMetaObjectCleanUp cleanUp_WindAdjWizard( "WindAdjWizard", &WindAdjWizard::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString WindAdjWizard::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "WindAdjWizard", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString WindAdjWizard::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "WindAdjWizard", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* WindAdjWizard::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = StandardWizard::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"WindAdjWizard", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_WindAdjWizard.setMetaObject( metaObj );
    return metaObj;
}

void* WindAdjWizard::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "WindAdjWizard" ) )
	return this;
    return StandardWizard::qt_cast( clname );
}

bool WindAdjWizard::qt_invoke( int _id, QUObject* _o )
{
    return StandardWizard::qt_invoke(_id,_o);
}

bool WindAdjWizard::qt_emit( int _id, QUObject* _o )
{
    return StandardWizard::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool WindAdjWizard::qt_property( int id, int f, QVariant* v)
{
    return StandardWizard::qt_property( id, f, v);
}

bool WindAdjWizard::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
