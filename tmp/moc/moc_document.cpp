/****************************************************************************
** Document meta object code from reading C++ file 'document.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../document.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Document::className() const
{
    return "Document";
}

QMetaObject *Document::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Document( "Document", &Document::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Document::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Document", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Document::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Document", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Document::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "points", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"rescale", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected },
	{ "rescale(int)", &slot_1, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "msg", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"message", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "message(const QString&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Document", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Document.setMetaObject( metaObj );
    return metaObj;
}

void* Document::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Document" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

// SIGNAL message
void Document::message( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool Document::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 1: rescale((int)static_QUType_int.get(_o+1)); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Document::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: message((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QMainWindow::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Document::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool Document::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
