/****************************************************************************
** FdfmcDialog meta object code from reading C++ file 'fdfmcdialog.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../fdfmcdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FdfmcDialog::className() const
{
    return "FdfmcDialog";
}

QMetaObject *FdfmcDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FdfmcDialog( "FdfmcDialog", &FdfmcDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FdfmcDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FdfmcDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FdfmcDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FdfmcDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FdfmcDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUMethod slot_0 = {"clear", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "descritpion", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"descriptionChanged", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"update", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "clear()", &slot_0, QMetaData::Protected },
	{ "descriptionChanged(const QString&)", &slot_1, QMetaData::Protected },
	{ "update(int)", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FdfmcDialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FdfmcDialog.setMetaObject( metaObj );
    return metaObj;
}

void* FdfmcDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FdfmcDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool FdfmcDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: clear(); break;
    case 1: descriptionChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: update((int)static_QUType_int.get(_o+1)); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FdfmcDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FdfmcDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool FdfmcDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
