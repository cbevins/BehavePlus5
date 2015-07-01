/****************************************************************************
** GuideDialog meta object code from reading C++ file 'guidedialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../guidedialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GuideDialog::className() const
{
    return "GuideDialog";
}

QMetaObject *GuideDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GuideDialog( "GuideDialog", &GuideDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GuideDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GuideDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GuideDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GuideDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GuideDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUMethod slot_0 = {"clear", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"contextMenuActivated", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_2 = {"itemDoubleClicked", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "e", &static_QUType_ptr, "QKeyEvent", QUParameter::In }
    };
    static const QUMethod slot_3 = {"keyPressEvent", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "column", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"rightButtonClicked", 3, param_slot_4 };
    static const QUMethod slot_5 = {"store", 0, 0 };
    static const QUMethod slot_6 = {"wizard", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "clear()", &slot_0, QMetaData::Protected },
	{ "contextMenuActivated(int)", &slot_1, QMetaData::Protected },
	{ "itemDoubleClicked(QListViewItem*)", &slot_2, QMetaData::Protected },
	{ "keyPressEvent(QKeyEvent*)", &slot_3, QMetaData::Protected },
	{ "rightButtonClicked(QListViewItem*,const QPoint&,int)", &slot_4, QMetaData::Protected },
	{ "store()", &slot_5, QMetaData::Protected },
	{ "wizard()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GuideDialog", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GuideDialog.setMetaObject( metaObj );
    return metaObj;
}

void* GuideDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GuideDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool GuideDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: clear(); break;
    case 1: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 2: itemDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 3: keyPressEvent((QKeyEvent*)static_QUType_ptr.get(_o+1)); break;
    case 4: rightButtonClicked((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 5: store(); break;
    case 6: wizard(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GuideDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GuideDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool GuideDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
