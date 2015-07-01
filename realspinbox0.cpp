//------------------------------------------------------------------------------
/*! \file realspinbox.cpp
 *  \author Copyright (C) 2002-2004 by Collin D. Bevins
 *
 *  \brief RealSpinBox class methods.
 */

// Custom include files
#include "realspinbox.h"

// Qt include files
#include <qspinbox.h>
#include <qvalidator.h>

//------------------------------------------------------------------------------
/*! \brief RealSpinBox constructor.
 */

RealSpinBox::RealSpinBox( double min, double max, double step, double start,
        int decimals, QWidget *parent, const char *name ) :
    QSpinBox( parent, name ),
    m_minReal( min ),
    m_maxReal( max ),
    m_stepReal( step ),
    m_startReal( start ),
    m_factor( 1.0 ),
    m_round( 0.1 ),
    m_decimals( decimals ),
    m_validator( 0 )
{
    // Create and use a validator
    m_validator = new QDoubleValidator( m_minReal, m_maxReal, m_decimals,
        this, "realSpinBoxValidator" );
    Q_CHECK_PTR( m_validator );
    setValidator( m_validator );
    // Determine real-to-integer mapping factor and rounding value
    for ( int i=1; i<=m_decimals; i++ )
    {
        m_factor *= 10.;
        m_round *= 0.1;
    }
    // Set QSpinBox integers
    setMinValue( int( m_factor * m_minReal + m_round ) );
    setMaxValue( int( m_factor * m_maxReal + m_round ) );
    setLineStep( int( m_factor * m_stepReal + m_round ) );
    setValue( int( m_factor * m_startReal + m_round ) );
    return;
}

//------------------------------------------------------------------------------
/*! \brief RealSpinBox destructor.
 */

RealSpinBox::~RealSpinBox( void )
{
    delete m_validator; m_validator = 0;
}

//------------------------------------------------------------------------------
/*! \brief Reimplemented virtual function to map the current QSpinBox value
 *  to the text displayed in the entry field.
 */

QString RealSpinBox::mapValueToText( int value )
{
    double real = (double) value / m_factor + m_round;
    return( QString( "%1" ).arg( real, 0, 'f', m_decimals ) );
}

//------------------------------------------------------------------------------
/*! \brief Reimplemented virtual function to map the current QSpinBox entry
 *  field to an integer value.
 */

int RealSpinBox::mapTextToValue( bool *ok )
{
    double real = cleanText().toFloat();
    *ok = true;
    return( int( real * m_factor + m_round ) );
}

//------------------------------------------------------------------------------
/*! \brief Public access to the current real value.
 *  to the text displayed in the entry field.
 */

double RealSpinBox::realValue( void )
{
    return( cleanText().toFloat() );
}

//------------------------------------------------------------------------------
//  End of realspinbox.h
//------------------------------------------------------------------------------

