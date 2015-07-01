/****************************************************************************
** ModulesDialog meta object code from reading C++ file 'modulesdialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../modulesdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ModulesDialog::className() const
{
    return "ModulesDialog";
}

QMetaObject *ModulesDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ModulesDialog( "ModulesDialog", &ModulesDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ModulesDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ModulesDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ModulesDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ModulesDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ModulesDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"slotOptions", 1, param_slot_0 };
    static const QUMethod slot_1 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotOptions(int)", &slot_0, QMetaData::Public },
	{ "store()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ModulesDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ModulesDialog.setMetaObject( metaObj );
    return metaObj;
}

void* ModulesDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ModulesDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool ModulesDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotOptions((int)static_QUType_int.get(_o+1)); break;
    case 1: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ModulesDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ModulesDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool ModulesDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
