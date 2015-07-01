/****************************************************************************
** PageMenuDialog meta object code from reading C++ file 'pagemenudialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../pagemenudialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *PageMenuDialog::className() const
{
    return "PageMenuDialog";
}

QMetaObject *PageMenuDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_PageMenuDialog( "PageMenuDialog", &PageMenuDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString PageMenuDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PageMenuDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString PageMenuDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "PageMenuDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* PageMenuDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"itemDoubleClicked", 1, param_slot_0 };
    static const QUMethod slot_1 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "itemDoubleClicked(QListViewItem*)", &slot_0, QMetaData::Protected },
	{ "store()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"PageMenuDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_PageMenuDialog.setMetaObject( metaObj );
    return metaObj;
}

void* PageMenuDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "PageMenuDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool PageMenuDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: itemDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool PageMenuDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool PageMenuDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool PageMenuDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
