/****************************************************************************
** SlopeToolDialog meta object code from reading C++ file 'slopetooldialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../slopetooldialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SlopeToolDialog::className() const
{
    return "SlopeToolDialog";
}

QMetaObject *SlopeToolDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SlopeToolDialog( "SlopeToolDialog", &SlopeToolDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SlopeToolDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SlopeToolDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SlopeToolDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SlopeToolDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SlopeToolDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUMethod slot_0 = {"calculateButtonClicked", 0, 0 };
    static const QUMethod slot_1 = {"clear", 0, 0 };
    static const QUMethod slot_2 = {"clearOutputs", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_3 = {"mrfDoubleClicked", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"unitsActivated", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "calculateButtonClicked()", &slot_0, QMetaData::Protected },
	{ "clear()", &slot_1, QMetaData::Protected },
	{ "clearOutputs()", &slot_2, QMetaData::Protected },
	{ "mrfDoubleClicked(QListViewItem*)", &slot_3, QMetaData::Protected },
	{ "unitsActivated(int)", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SlopeToolDialog", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SlopeToolDialog.setMetaObject( metaObj );
    return metaObj;
}

void* SlopeToolDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SlopeToolDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool SlopeToolDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: calculateButtonClicked(); break;
    case 1: clear(); break;
    case 2: clearOutputs(); break;
    case 3: mrfDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 4: unitsActivated((int)static_QUType_int.get(_o+1)); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SlopeToolDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SlopeToolDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool SlopeToolDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
