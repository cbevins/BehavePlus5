/****************************************************************************
** WizardDialog meta object code from reading C++ file 'wizarddialog.h'
**
** Created: Thu Apr 29 11:04:21 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../wizarddialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *WizardDialog::className() const
{
    return "WizardDialog";
}

QMetaObject *WizardDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_WizardDialog( "WizardDialog", &WizardDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString WizardDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "WizardDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString WizardDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "WizardDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* WizardDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"itemDoubleClicked", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "column", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"rightButtonClicked", 3, param_slot_2 };
    static const QUMethod slot_3 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected },
	{ "itemDoubleClicked(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "rightButtonClicked(QListViewItem*,const QPoint&,int)", &slot_2, QMetaData::Protected },
	{ "store()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"WizardDialog", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_WizardDialog.setMetaObject( metaObj );
    return metaObj;
}

void* WizardDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "WizardDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool WizardDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 1: itemDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: rightButtonClicked((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 3: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool WizardDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool WizardDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool WizardDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
