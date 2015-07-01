/****************************************************************************
** BpDocument meta object code from reading C++ file 'bpdocument.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../bpdocument.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BpDocument::className() const
{
    return "BpDocument";
}

QMetaObject *BpDocument::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BpDocument( "BpDocument", &BpDocument::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BpDocument::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BpDocument", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BpDocument::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BpDocument", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BpDocument::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = Document::staticMetaObject();
    static const QUMethod slot_0 = {"regenerateWorksheet", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"contextMenuActivated", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"guideClicked", 1, param_slot_2 };
    static const QUMethod slot_3 = {"fuelClicked", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"maintenanceMenuActivated", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "points", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"rescale", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "regenerateWorksheet()", &slot_0, QMetaData::Public },
	{ "contextMenuActivated(int)", &slot_1, QMetaData::Protected },
	{ "guideClicked(int)", &slot_2, QMetaData::Protected },
	{ "fuelClicked()", &slot_3, QMetaData::Protected },
	{ "maintenanceMenuActivated(int)", &slot_4, QMetaData::Protected },
	{ "rescale(int)", &slot_5, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"BpDocument", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BpDocument.setMetaObject( metaObj );
    return metaObj;
}

void* BpDocument::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BpDocument" ) )
	return this;
    return Document::qt_cast( clname );
}

bool BpDocument::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: regenerateWorksheet(); break;
    case 1: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 2: guideClicked((int)static_QUType_int.get(_o+1)); break;
    case 3: fuelClicked(); break;
    case 4: maintenanceMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 5: rescale((int)static_QUType_int.get(_o+1)); break;
    default:
	return Document::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BpDocument::qt_emit( int _id, QUObject* _o )
{
    return Document::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BpDocument::qt_property( int id, int f, QVariant* v)
{
    return Document::qt_property( id, f, v);
}

bool BpDocument::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
