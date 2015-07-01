/****************************************************************************
** AttachDialog meta object code from reading C++ file 'attachdialog.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../attachdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AttachDialog::className() const
{
    return "AttachDialog";
}

QMetaObject *AttachDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AttachDialog( "AttachDialog", &AttachDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AttachDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AttachDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AttachDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AttachDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AttachDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "column", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"rightButtonClicked", 3, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Private },
	{ "rightButtonClicked(QListViewItem*,const QPoint&,int)", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"AttachDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AttachDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AttachDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AttachDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool AttachDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 1: rightButtonClicked((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AttachDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AttachDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool AttachDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
