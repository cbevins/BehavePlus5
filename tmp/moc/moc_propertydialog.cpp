/****************************************************************************
** PropertyTabDialog meta object code from reading C++ file 'propertydialog.h'
**
** Created: Wed Jul 13 12:48:37 2011
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../propertydialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *PropertyTabDialog::className() const
{
    return "PropertyTabDialog";
}

QMetaObject *PropertyTabDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PropertyTabDialog( "PropertyTabDialog", &PropertyTabDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PropertyTabDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PropertyTabDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PropertyTabDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PropertyTabDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PropertyTabDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppTabDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "tab", &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_0 = {"tabChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "tabChanged(QWidget*)", &slot_0, QMetaData::Protected },
	{ "store()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PropertyTabDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PropertyTabDialog.setMetaObject( metaObj );
    return metaObj;
}

void* PropertyTabDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PropertyTabDialog" ) )
	return this;
    return AppTabDialog::qt_cast( clname );
}

bool PropertyTabDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: tabChanged((QWidget*)static_QUType_ptr.get(_o+1)); break;
    case 1: store(); break;
    default:
	return AppTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PropertyTabDialog::qt_emit( int _id, QUObject* _o )
{
    return AppTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PropertyTabDialog::qt_property( int id, int f, QVariant* v)
{
    return AppTabDialog::qt_property( id, f, v);
}

bool PropertyTabDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
