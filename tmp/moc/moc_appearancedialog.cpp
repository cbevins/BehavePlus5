/****************************************************************************
** AppearanceDialog meta object code from reading C++ file 'appearancedialog.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../appearancedialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AppearanceDialog::className() const
{
    return "AppearanceDialog";
}

QMetaObject *AppearanceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AppearanceDialog( "AppearanceDialog", &AppearanceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AppearanceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppearanceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AppearanceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppearanceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AppearanceDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = PropertyTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"store", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"tabsValueChanged", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "store()", &slot_0, QMetaData::Protected },
	{ "tabsValueChanged(int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AppearanceDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AppearanceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AppearanceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AppearanceDialog" ) )
	return this;
    return PropertyTabDialog::qt_cast( clname );
}

bool AppearanceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: store(); break;
    case 1: tabsValueChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return PropertyTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AppearanceDialog::qt_emit( int _id, QUObject* _o )
{
    return PropertyTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AppearanceDialog::qt_property( int id, int f, QVariant* v)
{
    return PropertyTabDialog::qt_property( id, f, v);
}

bool AppearanceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
