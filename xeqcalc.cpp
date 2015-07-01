//------------------------------------------------------------------------------
/*! \file xeqcalc.cpp
 *  \version BehavePlus5
 *  \author Copyright (C) 2002-2011 by Collin D. Bevins.  All rights reserved.
 *
 *  \brief EqTree calculator interface.
 *
 *  This file contains the implementation interface between the generic EqTree
 *  and the specific set of variables and functions.  The idea is to keep all
 *  function and variable details in just 2 files, xeqtree.xml and xeqcalc.cpp.
 *
 *  All variables must be
 *  -# specified in the xeqtree.xml file via
 *          <variable name="vVariableName" type=""... />
 *  -# declared in xeqcalc.h via
 *          EqVar *vVariableName;
 *  -# initialized in EqCalc::EqCalc() constructor via
 *          vVariableName = m_eqTree->getVarPtr( "vVariableName" );
 *
 *  All functions must be
 *  -# specified in xeqtree.xml file via
 *          <function name="fFunctionName" input="" output=""... />
 *  -# declared in xeqcalc.h via
 *          void FunctionName( void );
 *  -# defined as a function in this file via
 *          void EqCalc::FunctionName( void ) { }
 *  -# initialized in EqCalc::EqCalc() constructor via
 *      m_eqTree->setEqFunAddress( "FunctionName", &EqCalc::FunctioName );
 */

// Custom include files
#include "appmessage.h"
#include "appsiunits.h"
#include "apptranslator.h"
#include "bpdocument.h"
#include "cdtlib.h"
#include "contain.h"
#include "fuelmodel.h"
#include "moisscenario.h"
#include "parser.h"
#include "property.h"
#include "xeqcalc.h"
#include "xeqvar.h"
#include "xeqtree.h"
#include "xfblib.h"

// Qt include files
#include <qstring.h>

// Standard include files
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const int MaxParts = 8;
static char Margin[] = { "        " };

//------------------------------------------------------------------------------
/*! \brief Convenience routine to get a pointer to the FuelModel
 *  of the current vSurfaceFuelBedModel (if not doing two fuel model weighting)
 *  or of the current vSurfaceFuelBedModel1 or vSurfaceFuelBedModel2
 *  (if doing two fuel model weighting).
 *
 *  \param id Indicates which fuel model to get:
 *      - 0 fetches a pointer to the current vSurfaceFuelBedModel FuelModel
 *      - 1 fetches a pointer to the current vSurfaceFuelBedModel1 FuelModel
 *      - 2 fetches a pointer to the current vSurfaceFuelBedModel2 FuelModel
 *
 *  \return Pointer to the current items FuelModel.
 */

FuelModel *EqCalc::currentFuelModel( int id )
{
    // Get the requested variable's address
    EqVar *varPtr;
    if ( id == 0 )
    {
        varPtr = vSurfaceFuelBedModel;
    }
    else if ( id == 1 )
    {
        varPtr = vSurfaceFuelBedModel1;
    }
    else if ( id == 2 )
    {
        varPtr = vSurfaceFuelBedModel2;
    }
    // Get the FuelModel class pointer for the current fuel model name
    QString name = varPtr->activeItemName();
    FuelModel *fm = m_eqTree->m_fuelModelList->fuelModelByModelName( name );
    if ( ! fm )
    // This code block should never be executed!
    {
        QString text("");
        translate( text, "EqCalc:FuelModelNotFound", varPtr->activeItemName() );
        bomb( text );
    }
    return( fm );
}

//------------------------------------------------------------------------------
/*! \brief ContainFF is Contain a'la Fried & Fried for multiple resources.
 *
 *  Dependent Variables (Outputs)
 *      vContainAttackBack (ch)
 *      vContainAttackHead (ch)
 *      vContainAttackPerimeter (ch)
 *      vContainAttackSize (ac)
 *      vContainCost (none)
 *      vContainLine (ch)
 *      vContainPoints
 *      vContainReportBack (ch)
 *      vContainReportHead (ch)
 *      vContainResourcesUsed (count)
 *      vContainSize (ac)
 *      vContainStatus
 *      vContainTime (min since report)
 *      vContainXMax (ch)
 *      vContainXMin (ch)
 *      vContainYMax (ch)
 *
 *  Independent Variables (Inputs)
 *      vContainAttackDist  (ch)
 *      vContainAttackTactic (Head or Rear)
 *      vContainReportRatio (ch/ch)
 *      vContainReportSize (ac)
 *      vContainReportSpread (ch/h)
 *      vContainResourceArrival (min since fire report)
 *      vContainResourceBaseCost
 *      vContainResourceDuration (min)
 *      vContainResourceHourCost
 *      vContainResourceName
 *      vContainResourceProd (ch/h)
 */

void EqCalc::ContainFF( void )
{
    // Crosswalk from Contain status codes to BehavePlus status codes.
    // 0 == Contained
    // 1 == Withdrawn
    // 2 == Escaped
    static int Status[8] =
    {
        2, // 0=Unreported  Fire started but not yet reported (init() not called)
        2, // 1=Reported    Fire reported but not yet attacked (init() called)
        2, // 2=Attacked    Fire attacked but not yet resolved
        0, // 3=Contained   Fire contained by attacking forces
        1, // 4=Overrun     Attacking resources are overrun
        2, // 5=Exhausted   Attacking resources are exhausted
        2, // 6=Overflow    Simulation max step overflow
        2, // 7=DistLimit   Fire spread distance limit exceeded
    };

    // We'll need to get some properties
    PropertyDict *prop = m_eqTree->m_propDict;

    // Access current input values
    double attackDist = vContainAttackDist->m_nativeValue;
    int    tactic     = vContainAttackTactic->activeItemDataIndex();
    double lwRatio    = vContainReportRatio->m_nativeValue;
    double reportSize = vContainReportSize->m_nativeValue;
    double reportRate = vContainReportSpread->m_nativeValue;
    double distLimit  = vContainLimitDist->m_nativeValue;
    if ( prop->boolean( "containConfLimitDistOff" ) )
    {
        distLimit = 1000000.;
    }

    // Build the containment resources array
    ContainForce *force = new ContainForce();
    checkmem( __FILE__, __LINE__, force, "ContainForce force", 1 );
    Parser parserArr( " \t,\"", "", "" );
    parserArr.parse( vContainResourceArrival->m_store );
    Parser parserDur( " \t,\"", "", "" );
    parserDur.parse( vContainResourceDuration->m_store );
    Parser parserName( " \t,\"", "", "" );
    parserName.parse( vContainResourceName->m_store );
    Parser parserProd( " \t,\"", "", "" );
    parserProd.parse( vContainResourceProd->m_store );

    Parser parserBase( " \t,\"", "", "" );
    Parser parserHour( " \t,\"", "", "" );
    bool doCost = vContainCost->m_isUserOutput;
    if ( doCost )
    {
        parserBase.parse( vContainResourceBaseCost->m_store );
        parserHour.parse( vContainResourceHourCost->m_store );
    }
    double arr, dur, prod, tmp;
    double base = 0.;
    double hour = 0.;
    QString name;
    // Loop for each resource
    for ( int i=0; i<vContainResourceName->m_tokens; i++ )
    {
        // Resource arrival time
        tmp = parserArr.token( i ).toDouble();
        appSiUnits()->convert( tmp,
            vContainResourceArrival->m_displayUnits.latin1(),
            vContainResourceArrival->m_nativeUnits.latin1(), &arr );
        //fprintf( stderr, "Resource Arrival %f %s --> %f %s\n",
        //  tmp, vContainResourceArrival->m_displayUnits.latin1(),
        //    arr, vContainResourceArrival->m_nativeUnits.latin1() );

        // Resource duration
        tmp = parserDur.token( i ).toDouble();
        appSiUnits()->convert( tmp,
            vContainResourceDuration->m_displayUnits.latin1(),
            vContainResourceDuration->m_nativeUnits.latin1(), &dur );
        //fprintf( stderr, "Resource Duration %f %s --> %f %s\n",
        //  tmp, vContainResourceDuration->m_displayUnits.latin1(),
        //    dur, vContainResourceDuration->m_nativeUnits.latin1() );

        // Resource name
        name = parserName.token( i );

        // Resource productivity
        tmp = parserProd.token( i ).toDouble();
        appSiUnits()->convert( tmp,
            vContainResourceProd->m_displayUnits.latin1(),
            vContainResourceProd->m_nativeUnits.latin1(), &prod );
        //fprintf( stderr, "Resource Productivity %f %s --> %f %s\n",
        //  tmp, vContainResourceProd->m_displayUnits.latin1(),
        //    prod, vContainResourceProd->m_nativeUnits.latin1() );

        // Resource cost
        if ( doCost )
        {
            base = parserBase.token( i ).toDouble();
            hour = parserHour.token( i ).toDouble();
        }
        // Add the resource to the resource array
        force->addResource( arr, prod, dur, LeftFlank, name.latin1(),
            base, hour );
    }

    // Calculate results
    int maxSteps = prop->integer( "containConfMaxSteps" );
    int minSteps = prop->integer( "containConfMinSteps" );
    bool retry   = prop->boolean( "containConfRetry" );
    ContainSim *sim = new ContainSim( reportSize, reportRate, lwRatio,
        force, (ContainTactic) tactic, attackDist, distLimit,
        retry, minSteps, maxSteps );
    checkmem( __FILE__, __LINE__, sim, "ContainSim sim", 1 );
    sim->run();
    int    status = Status[ sim->m_left->m_status ];
    double finalSize = sim->m_finalSize;
    // If no contained or limit exceeded...
    if ( status != 0 && status != 3 )
    {
        // Reset final size to -1 acres
        finalSize = -1;
        // If fire line was built, then Withdrawn, otherwise Escaped
        status = ( sim->m_finalLine > 0.0 ) ? 1 : 2;
    }

    // Determine perimeter and size at initial attack
    bool useFirstResourceArrivalTimeAsInitialAttack = true;
    double length = sim->m_left->m_attackBack + sim->m_left->m_attackHead;
    if ( useFirstResourceArrivalTimeAsInitialAttack )
    {
        length = sim->m_left->m_initialAttackBack
               + sim->m_left->m_initialAttackHead;
    }
    double width  = FBL_SurfaceFireWidth( length, lwRatio );
    double perim  = FBL_SurfaceFirePerimeter( length, width );
    double size   = FBL_SurfaceFireArea( length, width );
    size *= 0.1;    // ac

    // Store results
    if ( useFirstResourceArrivalTimeAsInitialAttack )
    {
        vContainAttackBack->update( sim->m_left->m_initialAttackBack );
        vContainAttackHead->update( sim->m_left->m_initialAttackHead );
    }
    else
    {
        vContainAttackBack->update( sim->m_left->m_attackBack );
        vContainAttackHead->update( sim->m_left->m_attackHead );
    }
    vContainAttackPerimeter->update( perim );
    vContainAttackSize->update( size );
    vContainCost->update( sim->m_finalCost );
    vContainLine->update( sim->m_finalLine );
    vContainPoints->update( sim->m_left->m_step + 1 );
    vContainReportBack->update( sim->m_left->m_reportBack );
    vContainReportHead->update( sim->m_left->m_reportHead );
    vContainResourcesUsed->update( sim->m_used );
    vContainSize->update( finalSize );
    vContainStatus->updateItem( status );
    vContainTime->update( sim->m_finalTime );
    vContainXMax->update( sim->m_xMax );
    vContainXMin->update( sim->m_xMin );
    vContainYMax->update( sim->m_yMax );

    // Log results
    if( m_log )
    {
        int outputs = 14 + sim->m_left->m_step + 1;
        fprintf( m_log, "%sbegin proc ContainFF() 5 %d\n", Margin, outputs );
        fprintf( m_log, "%s  i vContainReportSpread %g %d %s\n", Margin,
            vContainReportSpread->m_displayValue,
            vContainReportSpread->m_displayDecimals,
            vContainReportSpread->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vContainReportSize %g %d %s\n", Margin,
            vContainReportSize->m_displayValue,
            vContainReportSize->m_displayDecimals,
            vContainReportSize->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vContainReportRatio %g %d %s\n", Margin,
            vContainReportRatio->m_displayValue,
            vContainReportRatio->m_displayDecimals,
            vContainReportRatio->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vContainAttackTactic %d %d %s\n", Margin,
            vContainAttackTactic->activeItemDataIndex(),
            0,
            vContainAttackTactic->activeItemName().latin1() );
        fprintf( m_log, "%s  i vContainAttackDist %g %d %s\n", Margin,
            vContainAttackDist->m_displayValue,
            vContainAttackDist->m_displayDecimals,
            vContainAttackDist->m_displayUnits.latin1() );

        fprintf( m_log, "%s  o vContainAttackSize %g %d %s\n", Margin,
            vContainAttackSize->m_displayValue,
            vContainAttackSize->m_displayDecimals,
            vContainAttackSize->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainAttackPerimeter %g %d %s\n", Margin,
            vContainAttackPerimeter->m_displayValue,
            vContainAttackPerimeter->m_displayDecimals,
            vContainAttackPerimeter->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainStatus %d %d %s\n", Margin,
            vContainStatus->activeItemDataIndex(),
            0,
            vContainStatus->activeItemName().latin1() );
        fprintf( m_log, "%s  o vContainTime %g %d %s\n", Margin,
            vContainTime->m_displayValue,
            vContainTime->m_displayDecimals,
            vContainTime->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainSize %g %d %s\n", Margin,
            vContainSize->m_displayValue,
            vContainSize->m_displayDecimals,
            vContainSize->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainLine %g %d %s\n", Margin,
            vContainLine->m_displayValue,
            vContainLine->m_displayDecimals,
            vContainLine->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainResourcesUsed %g %d ratio\n", Margin,
            vContainResourcesUsed->m_displayValue,
            vContainResourcesUsed->m_displayDecimals );
        fprintf( m_log, "%s  o vContainCost %g %d %s\n", Margin,
            vContainCost->m_displayValue,
            vContainCost->m_displayDecimals,
            vContainCost->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainAttackBack %g %d %s\n", Margin,
            vContainAttackBack->m_displayValue,
            vContainAttackBack->m_displayDecimals,
            vContainAttackBack->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainAttackHead %g %d %s\n", Margin,
            vContainAttackHead->m_displayValue,
            vContainAttackHead->m_displayDecimals,
            vContainAttackHead->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportBack %g %d %s\n", Margin,
            vContainReportBack->m_displayValue,
            vContainReportBack->m_displayDecimals,
            vContainReportBack->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportHead %g %d %s\n", Margin,
            vContainReportHead->m_displayValue,
            vContainReportHead->m_displayDecimals,
            vContainReportHead->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainXMin %g %d %s\n", Margin,
            vContainXMin->m_displayValue,
            vContainXMin->m_displayDecimals,
            vContainXMin->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainXMax %g %d %s\n", Margin,
            vContainXMax->m_displayValue,
            vContainXMax->m_displayDecimals,
            vContainXMax->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainYMax %g %d %s\n", Margin,
            vContainYMax->m_displayValue,
            vContainYMax->m_displayDecimals,
            vContainYMax->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainPoints %g %d ratio\n", Margin,
            vContainPoints->m_displayValue,
            vContainPoints->m_displayDecimals );
        // The coordinates need to be converted from chains to display units
        double factor, offset;
        appSiUnits()->conversionFactorOffset( vContainXMax->m_nativeUnits,
            vContainXMax->m_displayUnits, &factor, &offset );
        for ( int pt = 0;
              pt <= sim->m_left->m_step;
              pt++ )
        {
            fprintf( m_log, " %s      %f %f\n", Margin,
                offset + factor * sim->m_x[pt],
                offset + factor * sim->m_y[pt] );
        }
    }
    // Free resources
    delete force;   force = 0;
    delete sim;     sim = 0;
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainFFReportRatio
 *
 *  Dependent Variables (Outputs)
 *      vContainReportRatio (ch/ch)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengthToWidth (ft/ft)
 */

void EqCalc::ContainFFReportRatio( void )
{
    // Access current input values
    double ratio = vSurfaceFireLengthToWidth->m_nativeValue;
    // Store results
    vContainReportRatio->update( ratio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc ContainFFReportRatio() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireArea %g %s\n", Margin,
            vSurfaceFireLengthToWidth->m_nativeValue,
            vSurfaceFireLengthToWidth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportRatio %g %s\n", Margin,
            vContainReportRatio->m_nativeValue,
            vContainReportRatio->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainFFReportSize
 *
 *  Dependent Variables (Outputs)
 *      vContainReportSize (ac)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireArea (ac)
 */

void EqCalc::ContainFFReportSize( void )
{
    // Access current input values
    double acres = vSurfaceFireArea->m_nativeValue;
    // Store results
    vContainReportSize->update( acres );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc ContainFFReportSize() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireArea %g %s\n", Margin,
            vSurfaceFireArea->m_nativeValue,
            vSurfaceFireArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportSize %g %s\n", Margin,
            vContainReportSize->m_nativeValue,
            vContainReportSize->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainFFReportSpread - simply transforms vSurfaceFireSpreadAtHead
 *	from its native units (ft/min) into the F&F Contain native units (ch/hr)
 *
 *  Dependent Variables (Outputs)
 *      vContainReportSpread (ch/h)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireSpreadAtHead (ft/min)
 */

void EqCalc::ContainFFReportSpread( void )
{
    // Access current input values
    double fpm = vSurfaceFireSpreadAtHead->m_nativeValue;
    // Store results
    vContainReportSpread->update( fpm * 60. / 66. );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc ContainFFReportSpread() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportSpread %g %s\n", Margin,
            vContainReportSpread->m_nativeValue,
            vContainReportSpread->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief ContainFFSingle is Contain a'la Fried & Freid for single resources
 *
 *  Dependent Variables (Outputs)
 *      vContainAttackBack (ch)
 *      vContainAttackHead (ch)
 *      vContainAttackPerimeter (ch)
 *      vContainAttackSize (ac)
 *      vContainCost (none)
 *      vContainLine (ch)
 *      vContainPoints
 *      vContainReportBack (ch)
 *      vContainReportHead (ch)
 *      vContainResourcesUsed (count)
 *      vContainSize (ac)
 *      vContainStatus
 *      vContainTime (min since report)
 *      vContainXMax (ch)
 *      vContainXMin (ch)
 *      vContainYMax (ch)
 *
 *  Independent Variables (Inputs)
 *      vContainAttackDist  (ch)
 *      vContainAttackTactic (Head or Rear)
 *      vContainReportRatio (ch/ch)
 *      vContainReportSize (ac)
 *      vContainReportSpread (ch/h)
 *      vContainResourceArrival (min since fire report)
 *      vContainResourceBaseCost
 *      vContainResourceDuration (min)
 *      vContainResourceHourCost
 *      vContainResourceName
 *      vContainResourceProd (ch/h)
 */

void EqCalc::ContainFFSingle( void )
{
    // Crosswalk from Contain status codes to BehavePlus status codes.
    // 0 == Contained
    // 1 == Withdrawn
    // 2 == Escaped
    static int Status[8] =
    {
        2, // 0=Unreported  Fire started but not yet reported (init() not called)
        2, // 1=Reported    Fire reported but not yet attacked (init() called)
        2, // 2=Attacked    Fire attacked but not yet resolved
        0, // 3=Contained   Fire contained by attacking forces
        1, // 4=Overrun     Attacking resources are overrun
        2, // 5=Exhausted   Attacking resources are exhausted
        2, // 6=Overflow    Simulation max step overflow
        2, // 7=DistLimit   Fire spread distance limit exceeded
    };

    // We'll need to get some properties
    PropertyDict *prop = m_eqTree->m_propDict;

    // Access current input values
    double attackDist = vContainAttackDist->m_nativeValue;
    int    tactic     = vContainAttackTactic->activeItemDataIndex();
    double lwRatio    = vContainReportRatio->m_nativeValue;
    double reportSize = vContainReportSize->m_nativeValue;
    double reportRate = vContainReportSpread->m_nativeValue;
    double distLimit  = vContainLimitDist->m_nativeValue;
    if ( prop->boolean( "containConfLimitDistOff" ) )
    {
        distLimit = 1000000.;
    }

    // Resources
    QString name      = vContainResourceName->m_store;
    double arrival    = vContainResourceArrival->m_nativeValue;
    double duration   = vContainResourceDuration->m_nativeValue;
    double prod       = vContainResourceProd->m_nativeValue;
    double baseCost   = 0.;
    double hourCost   = 0.;
    bool   doCost     = vContainCost->m_isUserOutput;
    if ( doCost )
    {
        baseCost   = vContainResourceBaseCost->m_nativeValue;
        hourCost   = vContainResourceHourCost->m_nativeValue;
    }

    // Build the containment resources array
    ContainForce *force = new ContainForce();
    checkmem( __FILE__, __LINE__, force, "ContainForce force", 1 );
    force->addResource( arrival, prod, duration, LeftFlank, name.latin1(),
            baseCost, hourCost );

    // Calculate results
    int maxSteps = prop->integer( "containConfMaxSteps" );
    int minSteps = prop->integer( "containConfMinSteps" );
    bool retry   = prop->boolean( "containConfRetry" );
    ContainSim *sim = new ContainSim( reportSize, reportRate, lwRatio,
        force, (ContainTactic) tactic, attackDist, distLimit,
        retry, minSteps, maxSteps );
    checkmem( __FILE__, __LINE__, sim, "ContainSim sim", 1 );
    sim->run();
    int    status = Status[ sim->m_left->m_status ];
    double finalSize = sim->m_finalSize;
    // If not contained...
    if ( status != 0 && status != 3 )
    {
        // Reset final size to -1 acres
        finalSize = -1;
        // If fire line was built, then Withdrawn, otherwise Escaped
        status = ( sim->m_finalLine > 0.0 ) ? 1 : 2;
    }

    // Determine perimeter and size at initial attack
    double length = sim->m_left->m_attackBack + sim->m_left->m_attackHead;
    double width  = FBL_SurfaceFireWidth( length, lwRatio );
    double perim  = FBL_SurfaceFirePerimeter( length, width );
    double size   = FBL_SurfaceFireArea( length, width );  // ch2
    size *= 0.1;    // ac

    // Store results
    vContainAttackBack->update( sim->m_left->m_attackBack );
    vContainAttackHead->update( sim->m_left->m_attackHead );
    vContainAttackPerimeter->update( perim );
    vContainAttackSize->update( size );
    vContainCost->update( sim->m_finalCost );
    vContainLine->update( sim->m_finalLine );
    vContainPoints->update( sim->m_left->m_step + 1 );
    vContainReportBack->update( sim->m_left->m_reportBack );
    vContainReportHead->update( sim->m_left->m_reportHead );
    vContainResourcesUsed->update( sim->m_used );
    vContainSize->update( finalSize );
    vContainStatus->updateItem( status );
    vContainTime->update( sim->m_finalTime );
    vContainXMax->update( sim->m_xMax );
    vContainXMin->update( sim->m_xMin );
    vContainYMax->update( sim->m_yMax );

    // Log results
    if( m_log )
    {
        int outputs = 14 + sim->m_left->m_step + 1;
        fprintf( m_log, "%sbegin proc ContainFF() 5 %d\n", Margin, outputs );
        fprintf( m_log, "%s  i vContainReportSpread %g %d %s\n", Margin,
            vContainReportSpread->m_displayValue,
            vContainReportSpread->m_displayDecimals,
            vContainReportSpread->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vContainReportSize %g %d %s\n", Margin,
            vContainReportSize->m_displayValue,
            vContainReportSize->m_displayDecimals,
            vContainReportSize->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vContainReportRatio %g %d %s\n", Margin,
            vContainReportRatio->m_displayValue,
            vContainReportRatio->m_displayDecimals,
            vContainReportRatio->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vContainAttackTactic %d %d %s\n", Margin,
            vContainAttackTactic->activeItemDataIndex(),
            0,
            vContainAttackTactic->activeItemName().latin1() );
        fprintf( m_log, "%s  i vContainAttackDist %g %d %s\n", Margin,
            vContainAttackDist->m_displayValue,
            vContainAttackDist->m_displayDecimals,
            vContainAttackDist->m_displayUnits.latin1() );

        fprintf( m_log, "%s  o vContainAttackSize %g %d %s\n", Margin,
            vContainAttackSize->m_displayValue,
            vContainAttackSize->m_displayDecimals,
            vContainAttackSize->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainAttackPerimeter %g %d %s\n", Margin,
            vContainAttackPerimeter->m_displayValue,
            vContainAttackPerimeter->m_displayDecimals,
            vContainAttackPerimeter->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainStatus %d %d %s\n", Margin,
            vContainStatus->activeItemDataIndex(),
            0,
            vContainStatus->activeItemName().latin1() );
        fprintf( m_log, "%s  o vContainTime %g %d %s\n", Margin,
            vContainTime->m_displayValue,
            vContainTime->m_displayDecimals,
            vContainTime->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainSize %g %d %s\n", Margin,
            vContainSize->m_displayValue,
            vContainSize->m_displayDecimals,
            vContainSize->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainLine %g %d %s\n", Margin,
            vContainLine->m_displayValue,
            vContainLine->m_displayDecimals,
            vContainLine->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainResourcesUsed %g %d ratio\n", Margin,
            vContainResourcesUsed->m_displayValue,
            vContainResourcesUsed->m_displayDecimals );
        fprintf( m_log, "%s  o vContainCost %g %d %s\n", Margin,
            vContainCost->m_displayValue,
            vContainCost->m_displayDecimals,
            vContainCost->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainAttackBack %g %d %s\n", Margin,
            vContainAttackBack->m_displayValue,
            vContainAttackBack->m_displayDecimals,
            vContainAttackBack->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainAttackHead %g %d %s\n", Margin,
            vContainAttackHead->m_displayValue,
            vContainAttackHead->m_displayDecimals,
            vContainAttackHead->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportBack %g %d %s\n", Margin,
            vContainReportBack->m_displayValue,
            vContainReportBack->m_displayDecimals,
            vContainReportBack->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainReportHead %g %d %s\n", Margin,
            vContainReportHead->m_displayValue,
            vContainReportHead->m_displayDecimals,
            vContainReportHead->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainXMin %g %d %s\n", Margin,
            vContainXMin->m_displayValue,
            vContainXMin->m_displayDecimals,
            vContainXMin->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainXMax %g %d %s\n", Margin,
            vContainXMax->m_displayValue,
            vContainXMax->m_displayDecimals,
            vContainXMax->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainYMax %g %d %s\n", Margin,
            vContainYMax->m_displayValue,
            vContainYMax->m_displayDecimals,
            vContainYMax->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vContainPoints %g %d ratio\n", Margin,
            vContainPoints->m_displayValue,
            vContainPoints->m_displayDecimals );
        // The coordinates need to be converted from chains to display units
        double factor, offset;
        appSiUnits()->conversionFactorOffset( vContainXMax->m_nativeUnits,
            vContainXMax->m_displayUnits, &factor, &offset );
        for ( int pt = 0;
              pt <= sim->m_left->m_step;
              pt++ )
        {
            fprintf( m_log, " %s      %f %f\n", Margin,
                offset + factor * sim->m_x[pt],
                offset + factor * sim->m_y[pt] );
        }
    }
    // Free resources
    delete force;   force = 0;
    delete sim;     sim = 0;
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireActiveCrown
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireActiveCrown
 *
 *  Independent Variables (Inputs)
 *      vCrownFireActiveRatio (dl)
 */

void EqCalc::CrownFireActiveCrown( void )
{
    // Access current input values
    double ratio = vCrownFireActiveRatio->m_nativeValue;
    // Calculate results
    int   status = ( ratio < 1.0 ) ? 0 : 1;
    // Store results
    vCrownFireActiveCrown->updateItem( status );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireActiveCrown() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireActiveRatio %g %s\n", Margin,
            vCrownFireActiveRatio->m_nativeValue,
            vCrownFireActiveRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireActiveCrown %g %s\n", Margin,
            vCrownFireActiveCrown->m_nativeValue,
            vCrownFireActiveCrown->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireActiveRatio
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireActiveRatio (dl)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireSpreadRate (ft/min)
 *      vCrownFireCritCrownSpreadRate (ft/min)
 */

void EqCalc::CrownFireActiveRatio( void )
{
    // Access current input values
    double ros  = vCrownFireSpreadRate->m_nativeValue;
    double cros = vCrownFireCritCrownSpreadRate->m_nativeValue;
    // Calculate results
    double ratio = FBL_CrownFireActiveRatio( ros, cros );
    // Store results
    vCrownFireActiveRatio->update( ratio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireActiveRatio() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireSpreadRate %g %s\n", Margin,
            vCrownFireSpreadRate->m_nativeValue,
            vCrownFireSpreadRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireCritCrownSpreadRate %g %s\n", Margin,
            vCrownFireCritCrownSpreadRate->m_nativeValue,
            vCrownFireCritCrownSpreadRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireActiveRatio %g %s\n", Margin,
            vCrownFireActiveRatio->m_nativeValue,
            vCrownFireActiveRatio->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrowFireArea
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireArea (ft)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireSpreadDist (ft)
 *      vCrownFireLengthToWidth (ft/ft)
 */

void EqCalc::CrownFireArea( void )
{
    // Access current input values
    double dist  = vCrownFireSpreadDist->m_nativeValue;
    double ratio = vCrownFireLengthToWidth->m_nativeValue;
    // Calculate results
    double area = FBL_CrownFireArea( dist, ratio );
    // Store results
    vCrownFireArea->update( area );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireArea() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireSpreadDist %g %s\n", Margin,
            vCrownFireSpreadDist->m_nativeValue,
            vCrownFireSpreadDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireLengthToWidth %g %s\n", Margin,
            vSurfaceFireLengthToWidth->m_nativeValue,
            vSurfaceFireLengthToWidth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireArea %g %s\n", Margin,
            vCrownFireArea->m_nativeValue,
            vCrownFireArea->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireCritCrownSpreadRate
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireCritCrownSpreadRate (ft/min)
 *
 *  Independent Variables (Inputs)
 *      vTreeCanopyBulkDens (lb/ft3)
 */

void EqCalc::CrownFireCritCrownSpreadRate( void )
{
    // Access current input values
    double rhob = vTreeCanopyBulkDens->m_nativeValue;
    // Calculate results
    double cros = FBL_CrownFireCriticalCrownFireSpreadRate( rhob );
    // Store results
    vCrownFireCritCrownSpreadRate->update( cros );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireCritCrownSpreadRate() 1 1\n", Margin );
        fprintf( m_log, "%s  i vTreeCanopyBulkDens %g %s\n", Margin,
            vTreeCanopyBulkDens->m_nativeValue,
            vTreeCanopyBulkDens->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireCritCrownSpreadRate %g %s\n", Margin,
            vCrownFireCritCrownSpreadRate->m_nativeValue,
            vCrownFireCritCrownSpreadRate->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireCritSurfFireInt
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireCritSurfFireInt (Btu/ft/s)
 *
 *  Independent Variables (Inputs)
 *      vTreeCrownBaseHt (ft)
 *      vTreeFoliarMois (lb water/lb fuel)
 */

void EqCalc::CrownFireCritSurfFireInt( void )
{
    // Access current input values
    double fmc = vTreeFoliarMois->m_nativeValue;
    double cbh = vTreeCrownBaseHt->m_nativeValue;
    // Calculate results
    double cfli = FBL_CrownFireCriticalSurfaceFireIntensity( fmc, cbh );
    // Store results
    vCrownFireCritSurfFireInt->update( cfli );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireCritSurfFireInt() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeCrownBaseHt %g %s\n", Margin,
            vTreeCrownBaseHt->m_nativeValue,
            vTreeCrownBaseHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeFoliarMois %g %s\n", Margin,
            vTreeFoliarMois->m_nativeValue,
            vTreeFoliarMois->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireCritSurfFireInt %g %s\n", Margin,
            vCrownFireCritSurfFireInt->m_nativeValue,
            vCrownFireCritSurfFireInt->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireCritSurfFlameLeng
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireCritSurfFlameLeng (ft)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireCritSurfFireInt (Btu/ft/s)
 */

void EqCalc::CrownFireCritSurfFlameLeng( void )
{
    // Access current input values
    double cfli = vCrownFireCritSurfFireInt->m_nativeValue;
    // Calculate results
    double cfl  = FBL_CrownFireCriticalSurfaceFlameLength( cfli );
    // Store results
    vCrownFireCritSurfFlameLeng->update( cfl );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireCritSurfFlameLeng() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireCritSurfFireInt %g %s\n", Margin,
            vCrownFireCritSurfFireInt->m_nativeValue,
            vCrownFireCritSurfFireInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireCritSurfFlameLeng %g %s\n", Margin,
            vCrownFireCritSurfFlameLeng->m_nativeValue,
            vCrownFireCritSurfFlameLeng->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireFlameLength
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireFlameLeng (ft)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireLineInt (Btu/ft/s)
 */

void EqCalc::CrownFireFlameLength( void )
{
    // Access current input values
    double cfli = vCrownFireLineInt->m_nativeValue;
    // Calculate results
    double cfl  = FBL_CrownFireFlameLength( cfli );
    // Store results
    vCrownFireFlameLeng->update( cfl );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireFlameLength() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireLineInt %g %s\n", Margin,
            vCrownFireLineInt->m_nativeValue,
            vCrownFireLineInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireFlameLeng %g %s\n", Margin,
            vCrownFireFlameLeng->m_nativeValue,
            vCrownFireFlameLeng->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrownFireFuelLoad
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireFuelLoad (lb/ft2)
 *
 *  Independent Variables (Inputs)
 *      vTreeCanopyBulkDens (lb/ft3)
 *      vTreeCoverHt (ft)
 *		vTreeCrownBaseHt (ft)
 */

void EqCalc::CrownFireFuelLoad( void )
{
    // Access current input values
    double bulkDensity = vTreeCanopyBulkDens->m_nativeValue;
    double canopyHt = vTreeCoverHt->m_nativeValue;
	double baseHt = vTreeCrownBaseHt->m_nativeValue;
    // Calculate results
    double load  = FBL_CrownFuelLoad( bulkDensity, canopyHt, baseHt );
    // Store results
    vCrownFireFuelLoad->update( load );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireFuelLoad() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeCanopyBulkDens %g %s\n", Margin,
            vTreeCanopyBulkDens->m_nativeValue,
            vTreeCanopyBulkDens->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHt %g %s\n", Margin,
            vTreeCoverHt->m_nativeValue,
            vTreeCoverHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireFuelLoad %g %s\n", Margin,
            vCrownFireFuelLoad->m_nativeValue,
            vCrownFireFuelLoad->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrownFireHeatPerUnitArea
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireHeatPerUnitArea (Btu/ft2)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireHeatPerUnitAreaCanopy (Btu/ft2)
 *		vSurfaceFireHeatPerUnitArea (Btu/ft2)
 */

void EqCalc::CrownFireHeatPerUnitArea( void )
{
    // Access current input values
    double surfaceHpua = vSurfaceFireHeatPerUnitArea->m_nativeValue;
    double canopyHpua  = vCrownFireHeatPerUnitAreaCanopy->m_nativeValue;
    // Calculate results
    double crownHpua = FBL_CrownFireHeatPerUnitArea( surfaceHpua, canopyHpua );
    // Store results
    vCrownFireHeatPerUnitArea->update( crownHpua );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireHeatPerUnitArea() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireHeatPerUnitArea %g %s\n", Margin,
            vSurfaceFireHeatPerUnitArea->m_nativeValue,
            vSurfaceFireHeatPerUnitArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireHeatPerUnitAreaCanopy %g %s\n", Margin,
            vCrownFireHeatPerUnitAreaCanopy->m_nativeValue,
            vCrownFireHeatPerUnitAreaCanopy->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireHeatPerUnitArea %g %s\n", Margin,
            vCrownFireHeatPerUnitArea->m_nativeValue,
            vCrownFireHeatPerUnitArea->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrownFireHeatPerUnitAreaCanopy
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireHeatPerUnitAreaCanopy (Btu/ft2)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireFuelLoad (lb/ft2)
 */

void EqCalc::CrownFireHeatPerUnitAreaCanopy( void )
{
    // Access current input values
    double load = vCrownFireFuelLoad->m_nativeValue;
    // Calculate results
    double hpua = FBL_CrownFireHeatPerUnitAreaCanopy( load, 8000. );
    // Store results
    vCrownFireHeatPerUnitAreaCanopy->update( hpua );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireHeatPerUnitAreaCanopy() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireFuelLoad %g %s\n", Margin,
            vCrownFireFuelLoad->m_nativeValue,
            vCrownFireFuelLoad->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireHeatPerUnitAreaCanopy %g %s\n", Margin,
            vCrownFireHeatPerUnitAreaCanopy->m_nativeValue,
            vCrownFireHeatPerUnitAreaCanopy->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrowFireLengthToWidth
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireLengthToWidth (ft)
 *
 *  Independent Variables (Inputs)
 *      vWindSpeedAt20Ft (mi/h)
 */

void EqCalc::CrownFireLengthToWidth( void )
{
    // Access current input values
    double wind = vWindSpeedAt20Ft->m_nativeValue;
    // Calculate results
    double ratio = FBL_CrownFireLengthToWidthRatio( wind );
    // Store results
    vCrownFireLengthToWidth->update( ratio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireLengthToWidth() 1 1\n", Margin );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireLengthToWidth %g %s\n", Margin,
            vCrownFireLengthToWidth->m_nativeValue,
            vCrownFireLengthToWidth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireLineIntensity
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireLineInt (Btu/ft/s)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireHeatPerUnitArea (Btu/ft2)
 *      vCrownFireSpreadRate (ft/min)
 */

void EqCalc::CrownFireLineIntensity( void )
{
    // Access current input values
    double chpua = vCrownFireHeatPerUnitArea->m_nativeValue;
    double cros = vCrownFireSpreadRate->m_nativeValue;
    // Calculate results
    double cfli = FBL_CrownFireFirelineIntensity( chpua, cros );
    // Store results
    vCrownFireLineInt->update( cfli );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireLineInt() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireHeatPerUnitArea %g %s\n", Margin,
            vCrownFireHeatPerUnitArea->m_nativeValue,
            vCrownFireHeatPerUnitArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireSpreadRate %g %s\n", Margin,
            vCrownFireSpreadRate->m_nativeValue,
            vCrownFireSpreadRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireLineInt %g %s\n", Margin,
            vCrownFireLineInt->m_nativeValue,
            vCrownFireLineInt->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrowFirePerimeter
 *
 *  Dependent Variables (Outputs)
 *      vCrownFirePerimeter (ft)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireSpreadDist (ft)
 *      vCrownFireLengthToWidth (ft/ft)
 */

void EqCalc::CrownFirePerimeter( void )
{
    // Access current input values
    double dist  = vCrownFireSpreadDist->m_nativeValue;
    double ratio = vCrownFireLengthToWidth->m_nativeValue;
    // Calculate results
    double perim = FBL_CrownFirePerimeter( dist, ratio );
    // Store results
    vCrownFirePerimeter->update( perim );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFirePerimeter() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireSpreadDist %g %s\n", Margin,
            vCrownFireSpreadDist->m_nativeValue,
            vCrownFireSpreadDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireLengthToWidth %g %s\n", Margin,
            vSurfaceFireLengthToWidth->m_nativeValue,
            vSurfaceFireLengthToWidth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFirePerimeter %g %s\n", Margin,
            vCrownFirePerimeter->m_nativeValue,
            vCrownFirePerimeter->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFirePowerOfFire
 *
 *  Dependent Variables (Outputs)
 *      vCrownFirePowerOfFire (ft-lb/s/ft2)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireLineInt (Btu/ft/s)
 */

void EqCalc::CrownFirePowerOfFire( void )
{
    // Access current input values
    double cfli = vCrownFireLineInt->m_nativeValue;
    // Calculate results
    double power = FBL_CrownFirePowerOfFire( cfli );
    // Store results
    vCrownFirePowerOfFire->update( power );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFirePowerFire() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireLineInt %g %s\n", Margin,
            vCrownFireLineInt->m_nativeValue,
            vCrownFireLineInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFirePowerOfFire %g %s\n", Margin,
            vCrownFirePowerOfFire->m_nativeValue,
            vCrownFirePowerOfFire->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrownFirePowerOfWind
 *
 *  Dependent Variables (Outputs)
 *      vCrownFirePowerOfWind (ft-lb/s/ft2)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireSpreadrate (ft/min)
 *      vWindSpeedAt20Ft (ft/min)
 */

void EqCalc::CrownFirePowerOfWind( void )
{
    // Access current input values
    double wind = vWindSpeedAt20Ft->m_nativeValue;
    double cros = vCrownFireSpreadRate->m_nativeValue;
    // Calculate results
	wind = wind * 5280. / 60.;	// convert from miles/hour to ft/min
    double power = FBL_CrownFirePowerOfWind( wind, cros );
    // Store results
    vCrownFirePowerOfWind->update( power );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFirePowerWind() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireSpreadRate %g %s\n", Margin,
            vCrownFireSpreadRate->m_nativeValue,
            vCrownFireSpreadRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFirePowerOfWind %g %s\n", Margin,
            vCrownFirePowerOfWind->m_nativeValue,
            vCrownFirePowerOfWind->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrownFirePowerRatio
 *
 *  Dependent Variables (Outputs)
 *      vCrownFirePowerRatio (ratio)
 *
 *  Independent Variables (Inputs)
 *      vCrownFirePowerOfFire (ft-lb/s/ft2)
 *      vCrownFirePowerOfWind (ft-lb/s/ft2)
 */

void EqCalc::CrownFirePowerRatio( void )
{
    // Access current input values
    double fire = vCrownFirePowerOfFire->m_nativeValue;
    double wind = vCrownFirePowerOfWind->m_nativeValue;
    // Calculate results
    double ratio = FBL_CrownFirePowerRatio( fire, wind );
    // Store results
    vCrownFirePowerRatio->update( ratio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFirePowerRatio() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFirePowerOfFire %g %s\n", Margin,
            vCrownFirePowerOfFire->m_nativeValue,
            vCrownFirePowerOfFire->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFirePowerOfWind %g %s\n", Margin,
            vCrownFirePowerOfWind->m_nativeValue,
            vCrownFirePowerOfWind->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFirePowerRatio %g %s\n", Margin,
            vCrownFirePowerRatio->m_nativeValue,
            vCrownFirePowerRatio->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief CrownFireSpreadDist
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireSpreadDist (ft)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireSpreadRate (ft/min)
 *      vSurfaceFireElapsedTime ( min )
 */

void EqCalc::CrownFireSpreadDist( void )
{
    // Access current input values
    double cros    = vCrownFireSpreadRate->m_nativeValue;
    double elapsed = vSurfaceFireElapsedTime->m_nativeValue;
    // Calculate results
    double dist = cros * elapsed;
    // Store results
    vCrownFireSpreadDist->update( dist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireSpreadDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireSpreadRate %g %s\n", Margin,
            vCrownFireSpreadRate->m_nativeValue,
            vCrownFireSpreadRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireElapsedTime %g %s\n", Margin,
            vSurfaceFireElapsedTime->m_nativeValue,
            vSurfaceFireElapsedTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireSpreadDist %g %s\n", Margin,
            vCrownFireSpreadDist->m_nativeValue,
            vCrownFireSpreadDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireSpreadMapDist
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireSpreadMapDist (ft)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireSpreadDist (ft)
 *      vMapScale (ratio)
 */

void EqCalc::CrownFireSpreadMapDist( void )
{
    // Access current input values
    double dist  = vCrownFireSpreadDist->m_nativeValue;
    double scale = vMapScale->m_nativeValue;
    // Calculate results
    double md    = scale * dist / 5280.;
    // Store results
    vCrownFireSpreadMapDist->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireSpreadMapDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireSpreadDist %g %s\n", Margin,
            vCrownFireSpreadDist->m_nativeValue,
            vCrownFireSpreadDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireSpreadMapDist %g %s\n", Margin,
            vCrownFireSpreadMapDist->m_nativeValue,
            vCrownFireSpreadMapDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireSpreadRate
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireSpreadRate (ft/min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelMoisDead1 (lb/lb)
 *      vSurfaceFuelMoisDead10 (lb/lb)
 *      vSurfaceFuelMoisDead100 (lb/lb)
 *      vSurfaceFuelMoisLiveWood (lb/lb)
 *      vWindSpeedAt20Ft (ft/min)
 */

void EqCalc::CrownFireSpreadRate( void )
{
    // Access current input values
    double mc1      = vSurfaceFuelMoisDead1->m_nativeValue;
    double mc10     = vSurfaceFuelMoisDead10->m_nativeValue;
    double mc100    = vSurfaceFuelMoisDead100->m_nativeValue;
    double mcWood   = vSurfaceFuelMoisLiveWood->m_nativeValue;
    double wind20Ft = vWindSpeedAt20Ft->m_nativeValue;
    // Calculate results
    double cros = FBL_CrownFireSpreadRate( wind20Ft, mc1, mc10, mc100, mcWood );
    // Store results
    vCrownFireSpreadRate->update( cros );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireSpreadRate() 5 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead1 %g %s\n", Margin,
            vSurfaceFuelMoisDead1->m_nativeValue,
            vSurfaceFuelMoisDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead10 %g %s\n", Margin,
            vSurfaceFuelMoisDead10->m_nativeValue,
            vSurfaceFuelMoisDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead100 %g %s\n", Margin,
            vSurfaceFuelMoisDead100->m_nativeValue,
            vSurfaceFuelMoisDead100->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisLiveWood %g %s\n", Margin,
            vSurfaceFuelMoisLiveWood->m_nativeValue,
            vSurfaceFuelMoisLiveWood->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireSpreadRate %g %s\n", Margin,
            vCrownFireSpreadRate->m_nativeValue,
            vCrownFireSpreadRate->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireTransRatioFromFireIntAtVector
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireTransRatio (dl)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLineIntAtVector (Btu/ft/s)
 *      vCrownFireCritSurfFireInt (Btu/ft/s)
 */

void EqCalc::CrownFireTransRatioFromFireIntAtVector( void )
{
    // Access current input values
    double cfli  = vCrownFireCritSurfFireInt->m_nativeValue;
    double fli   = vSurfaceFireLineIntAtVector->m_nativeValue;
    // Calculate results
    double ratio = FBL_CrownFireTransitionRatio( fli, cfli );
    // Store results
    vCrownFireTransRatio->update( ratio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireTransRatioFromFireIntAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireCritSurfFireInt %g %s\n", Margin,
            vCrownFireCritSurfFireInt->m_nativeValue,
            vCrownFireCritSurfFireInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireLineIntAtVector %g %s\n", Margin,
            vSurfaceFireLineIntAtVector->m_nativeValue,
            vSurfaceFireLineIntAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireTransRatio %g %s\n", Margin,
            vCrownFireTransRatio->m_nativeValue,
            vCrownFireTransRatio->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireTransRatioFromFlameLengAtVector
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireTransRatio (dl)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireFlameLengAtVector (ft)
 *      vCrownFireCritSurfFireInt (Btu/ft/s)
 */

void EqCalc::CrownFireTransRatioFromFlameLengAtVector( void )
{
    // Access current input values
    double cfli  = vCrownFireCritSurfFireInt->m_nativeValue;
    double fl    = vSurfaceFireFlameLengAtVector->m_nativeValue;
    // Calculate results
    double fli   = FBL_SurfaceFireFirelineIntensity( fl );
    double ratio = FBL_CrownFireTransitionRatio( fli, cfli );
    // Store results
    vCrownFireTransRatio->update( ratio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireTransRatioFromFlameLengAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireCritSurfFireInt %g %s\n", Margin,
            vCrownFireCritSurfFireInt->m_nativeValue,
            vCrownFireCritSurfFireInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireTransRatio %g %s\n", Margin,
            vCrownFireTransRatio->m_nativeValue,
            vCrownFireTransRatio->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireTransToCrown
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireArea (ac)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengDist (ft)
 */

void EqCalc::CrownFireTransToCrown( void )
{
    // Access current input values
    double ratio = vCrownFireTransRatio->m_nativeValue;
    // Calculate results
    int   status = ( ratio < 1.0 ) ? 0 : 1;
    // Store results
    vCrownFireTransToCrown->updateItem( status );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireTransToCrown() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireTransRatio %g %s\n", Margin,
            vCrownFireTransRatio->m_nativeValue,
            vCrownFireTransRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireTransToCrown %g %s\n", Margin,
            vCrownFireTransToCrown->m_nativeValue,
            vCrownFireTransToCrown->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireType
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireType (class)
 *
 *  Independent Variables (Inputs)
 *      vCrownFireActiveRatio (dl)
 *      vCrownFireTransRatio (dl)
 */

void EqCalc::CrownFireType( void )
{
    // Access current input values
    double activeRatio = vCrownFireActiveRatio->m_nativeValue;
    double transRatio  = vCrownFireTransRatio->m_nativeValue;
    // Calculate results
    int status = FBL_FireType( transRatio, activeRatio ) ;
    // Store results
    vCrownFireType->updateItem( status );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireType() 2 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFireActiveRatio %g %s\n", Margin,
            vCrownFireActiveRatio->m_nativeValue,
            vCrownFireActiveRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vCrownFireTransRatio %g %s\n", Margin,
            vCrownFireTransRatio->m_nativeValue,
            vCrownFireTransRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireType %g %s\n", Margin,
            vCrownFireType->m_nativeValue,
            vCrownFireType->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief CrownFireWindDriven
 *
 *  Dependent Variables (Outputs)
 *      vCrownFireWindDriven
 *
 *  Independent Variables (Inputs)
 *      vCrownFirePowerRatio (ratio)
 */

void EqCalc::CrownFireWindDriven( void )
{
    // Access current input values
    double ratio = vCrownFirePowerRatio->m_nativeValue;
    // Calculate results
    int wind_driven = ( ratio > 0.00001 && ratio < 1.0 ) ? 1 : 0;
    // Store results
    vCrownFireWindDriven->updateItem( wind_driven );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc CrownFireWindDriven() 1 1\n", Margin );
        fprintf( m_log, "%s  i vCrownFirePowerRatio %g %s\n", Margin,
            vCrownFirePowerRatio->m_nativeValue,
            vCrownFirePowerRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vCrownFireWindDriven %g %s\n", Margin,
            vCrownFireWindDriven->m_nativeValue,
            vCrownFireWindDriven->m_nativeUnits.latin1() );
    }
}

//------------------------------------------------------------------------------
/*! \brief FireArea
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireArea (ac)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengDist (ft)
 *      vSurfaceFireWidthDist (ft)
 */

void EqCalc::FireArea( void )
{
    // Access current input values
    double length = vSurfaceFireLengDist->m_nativeValue;
    double width  = vSurfaceFireWidthDist->m_nativeValue;
    // Calculate results
    double acres  = FBL_SurfaceFireArea( length, width ) * 2.295684e-05;
    // Store results
    vSurfaceFireArea->update( acres );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireArea() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLengDist %g %s\n", Margin,
            vSurfaceFireLengDist->m_nativeValue,
            vSurfaceFireLengDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireWidthDist %g %s\n", Margin,
            vSurfaceFireWidthDist->m_nativeValue,
            vSurfaceFireWidthDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireArea %g %s\n", Margin,
            vSurfaceFireArea->m_nativeValue,
            vSurfaceFireArea->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireCharacteristicsDiagram
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireCharacteristicsDiagram
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireSpreadAtHead (ft/min)
 *      vSurfaceFireHeatPerUnitArea (Btu/ft2/min)
 */

void EqCalc::FireCharacteristicsDiagram( void )
{
    // Access current input values
    double x = vSurfaceFireCharacteristicsDiagram->m_nativeValue;
    // Calculate results (dummy)
    x++;
    // Store results
    vSurfaceFireCharacteristicsDiagram->update( x );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireCharacteristicsDiagram() 3 1\n", Margin );
        //fprintf( stderr, "%sbegin proc FireCharacteristicsDiagram() 3 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        //fprintf( stderr, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
        //      vSurfaceFireSpreadAtHead->m_nativeValue,
        //    vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtVector %g %s\n", Margin,
            vSurfaceFireSpreadAtVector->m_nativeValue,
               vSurfaceFireSpreadAtVector->m_nativeUnits.latin1() );
        //fprintf( stderr, "%s  i vSurfaceFireSpreadAtVector %g %s\n", Margin,
        //      vSurfaceFireSpreadAtVector->m_nativeValue,
        //    vSurfaceFireSpreadAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireHeatPerUnitArea %g %s\n", Margin,
            vSurfaceFireHeatPerUnitArea->m_nativeValue,
            vSurfaceFireHeatPerUnitArea->m_nativeUnits.latin1() );
        //fprintf( stderr, "%s  i vSurfaceFireHeatPerUnitArea %g %s\n", Margin,
        //      vSurfaceFireHeatPerUnitArea->m_nativeValue,
        //    vSurfaceFireHeatPerUnitArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireCharacteristicsDiagram %g\n", Margin,
            vSurfaceFireCharacteristicsDiagram->m_nativeValue );
        //fprintf( stderr, "%s  o vSurfaceFireCharacteristicsDiagram %g\n", Margin,
        //    vSurfaceFireCharacteristicsDiagram->m_nativeValue );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireDistAtBack
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireDistAtBack (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireElapsedTime (min)
 *      vSurfaceFireSpreadAtBack (ft/min)
 */

void EqCalc::FireDistAtBack( void )
{
    // Access current input values
    double ros     = vSurfaceFireSpreadAtBack->m_nativeValue;
    double elapsed = vSurfaceFireElapsedTime->m_nativeValue;
    // Calculate results
    double dist = ros * elapsed;
    // Store results
    vSurfaceFireDistAtBack->update( dist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireDistAtBack() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtBack %g %s\n", Margin,
            vSurfaceFireSpreadAtBack->m_nativeValue,
            vSurfaceFireSpreadAtBack->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireElapsedTime %g %s\n", Margin,
            vSurfaceFireElapsedTime->m_nativeValue,
            vSurfaceFireElapsedTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireDistAtBack %g %s\n", Margin,
            vSurfaceFireDistAtBack->m_nativeValue,
            vSurfaceFireDistAtBack->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireDistAtHead
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireDistAtHead (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireElapsedTime (min)
 *      vSurfaceFireSpreadAtHead (ft/min)
 */

void EqCalc::FireDistAtHead( void )
{
    // Access current input values
    double ros  = vSurfaceFireSpreadAtHead->m_nativeValue;
    double elapsed = vSurfaceFireElapsedTime->m_nativeValue;
    // Calculate results
    double dist = ros * elapsed;
    // Store results
    vSurfaceFireDistAtHead->update( dist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireDistAtHead() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireElapsedTime %g %s\n", Margin,
            vSurfaceFireElapsedTime->m_nativeValue,
            vSurfaceFireElapsedTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireDistAtHead %g %s\n", Margin,
            vSurfaceFireDistAtHead->m_nativeValue,
            vSurfaceFireDistAtHead->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireDistAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireDistAtVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireElapsedTime (min)
 *      vSurfaceFireSpreadAtVector (ft/min)
 */

void EqCalc::FireDistAtVector( void )
{
    // Access current input values
    double ros     = vSurfaceFireSpreadAtVector->m_nativeValue;
    double elapsed = vSurfaceFireElapsedTime->m_nativeValue;
    // Calculate results
    double dist = ros * elapsed;
    // Store results
    vSurfaceFireDistAtVector->update( dist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireDistAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtVector %g %s\n", Margin,
            vSurfaceFireSpreadAtVector->m_nativeValue,
            vSurfaceFireSpreadAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireElapsedTime %g %s\n", Margin,
            vSurfaceFireElapsedTime->m_nativeValue,
            vSurfaceFireElapsedTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireDistAtVector %g %s\n", Margin,
            vSurfaceFireDistAtVector->m_nativeValue,
            vSurfaceFireDistAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireEccentricity
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireEccentricity (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengthToWidth (fraction)
 */

void EqCalc::FireEccentricity ( void )
{
    // Access current input values
    double lw = vSurfaceFireLengthToWidth->m_nativeValue;
    // Calculate results
    double ec = FBL_SurfaceFireEccentricity( lw );
    // Store results
    vSurfaceFireEccentricity->update( ec );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireEccentricity() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLengthToWidth %g %s\n", Margin,
            vSurfaceFireLengthToWidth->m_nativeValue,
            vSurfaceFireLengthToWidth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireEccentricity %g %s\n", Margin,
            vSurfaceFireEccentricity->m_nativeValue,
            vSurfaceFireEccentricity->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireEffWindAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireEffWindAtVector (mi/h)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireNoWindRate (ft/min)
 *      vSurfaceFireSpreadAtVector (ft/min)
 */

void EqCalc::FireEffWindAtVector( void )
{
    // Access current input values
    double ros0    = vSurfaceFireNoWindRate->m_nativeValue;
    double rosV    = vSurfaceFireSpreadAtVector->m_nativeValue;
    // Calculate results
    double effWind = FBL_SurfaceFireEffectiveWindSpeedAtVector( ros0, rosV );
    // Store results
    vSurfaceFireEffWindAtVector->update( effWind );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireEffWindAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireNoWindRate %g %s\n", Margin,
            vSurfaceFireNoWindRate->m_nativeValue,
            vSurfaceFireNoWindRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtVector %g %s\n", Margin,
            vSurfaceFireSpreadAtVector->m_nativeValue,
            vSurfaceFireSpreadAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireEffWindAtVector %g %s\n", Margin,
            vSurfaceFireEffWindAtVector->m_nativeValue,
            vSurfaceFireEffWindAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireFlameHtAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireFlameHtAtVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireFlameLengAtVector (ft)
 */

void EqCalc::FireFlameHtAtVector( void )
{
    // Access current input values
    double fl = vSurfaceFireFlameLengAtVector->m_nativeValue;
    //double fa = vSurfaceFireFlameAngleAtVector->m_nativeValue;
    // Calculate results
    double fh = fl;
    // Store results
    vSurfaceFireFlameHtAtVector->update( fh );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireFlameHtAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireFlameAngleAtVector %g %s\n", Margin,
            vSurfaceFireFlameAngleAtVector->m_nativeValue,
            vSurfaceFireFlameAngleAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireFlameHtAtVector %g %s\n", Margin,
            vSurfaceFireFlameHtAtVector->m_nativeValue,
            vSurfaceFireFlameHtAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireFlameLengAtHead
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireFlameLengAtHead (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLineIntAtHead (Btu/ft/s)
 */

void EqCalc::FireFlameLengAtHead( void )
{
    // Access current input values
    double firelineIntensity = vSurfaceFireLineIntAtHead->m_nativeValue;
    // Calculate results
    double fl = FBL_SurfaceFireFlameLength( firelineIntensity );
    // Store results
    vSurfaceFireFlameLengAtHead->update( fl );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireFlameLengAtHead() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLineIntAtHead %g %s\n", Margin,
            vSurfaceFireLineIntAtHead->m_nativeValue,
            vSurfaceFireLineIntAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireFlameLengAtHead %g %s\n", Margin,
            vSurfaceFireFlameLengAtHead->m_nativeValue,
            vSurfaceFireFlameLengAtHead->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireFlameLengAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireFlameLengAtVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLineIntAtVector (Btu/ft/s)
 */

void EqCalc::FireFlameLengAtVector( void )
{
    // Access current input values
    double firelineIntensity = vSurfaceFireLineIntAtVector->m_nativeValue;
    // Calculate results
    double fl = FBL_SurfaceFireFlameLength( firelineIntensity );
    // Store results
    vSurfaceFireFlameLengAtVector->update( fl );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireFlameLengAtVector() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLineIntAtVector %g %s\n", Margin,
            vSurfaceFireLineIntAtVector->m_nativeValue,
            vSurfaceFireLineIntAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireHeatPerUnitArea
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireHeatPerUnitArea (Btu/ft2)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 *      vSurfaceFireResidenceTime (min)
 */

void EqCalc::FireHeatPerUnitArea( void )
{
    // Access current input values
    double rxi  = vSurfaceFireReactionInt->m_nativeValue;
    double tau  = vSurfaceFireResidenceTime->m_nativeValue;
    // Calculate results
    double hpua = FBL_SurfaceFireHeatPerUnitArea( rxi, tau ) ;
    // Store results
    vSurfaceFireHeatPerUnitArea->update( hpua );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireHeatPerUnitArea() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireResidenceTime %g %s\n", Margin,
            vSurfaceFireResidenceTime->m_nativeValue,
            vSurfaceFireResidenceTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireHeatPerUnitArea %g %s\n", Margin,
            vSurfaceFireHeatPerUnitArea->m_nativeValue,
            vSurfaceFireHeatPerUnitArea->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireHeatSource
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireHeatSource (btu/ft2/min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireSpreadAtHead (ft/min)
 *      vSurfaceFuelBedHeatSink (btu/ft3)
 */

void EqCalc::FireHeatSource( void )
{
    // Access current input values
    double spread = vSurfaceFireSpreadAtHead->m_nativeValue;
    double sink   = vSurfaceFuelBedHeatSink->m_nativeValue;
    // Calculate results
    double source = FBL_SurfaceFireHeatSource( spread, sink );
    // Store results
    vSurfaceFireHeatSource->update( source );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireHeatSource() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedHeatSink %g %s\n", Margin,
            vSurfaceFuelBedHeatSink->m_nativeValue,
            vSurfaceFuelBedHeatSink->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireHeatSource %g %s\n", Margin,
            vSurfaceFireHeatSource->m_nativeValue,
            vSurfaceFireHeatSource->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireLengDist
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireLengDist (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireDistAtBack (ft)
 *      vSurfaceFireDistAtHead (ft)
 */

void EqCalc::FireLengDist( void )
{
    // Access current input values
    double bd = vSurfaceFireDistAtBack->m_nativeValue;
    double fd = vSurfaceFireDistAtHead->m_nativeValue;
    // Calculate results
    double ld = bd + fd;
    // Store results
    vSurfaceFireLengDist->update( ld );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireLengDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireDistAtBack %g %s\n", Margin,
            vSurfaceFireDistAtBack->m_nativeValue,
            vSurfaceFireDistAtBack->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireDistAtHead %g %s\n", Margin,
            vSurfaceFireDistAtHead->m_nativeValue,
            vSurfaceFireDistAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLengDist %g %s\n", Margin,
            vSurfaceFireLengDist->m_nativeValue,
            vSurfaceFireLengDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireLengMapDist
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireLengMapDist (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengDist (ft)
 *      vMapScale (in/mi)
 */

void EqCalc::FireLengMapDist( void )
{
    // Access current input values
    double ld = vSurfaceFireLengDist->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * ld / 5280.;
    // Store results
    vSurfaceFireLengMapDist->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireLengMapDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLengDist %g %s\n", Margin,
            vSurfaceFireLengDist->m_nativeValue,
            vSurfaceFireLengDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLengMapDist %g %s\n", Margin,
            vSurfaceFireLengMapDist->m_nativeValue,
            vSurfaceFireLengMapDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireLengthtoWidth
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireLengthToWidth (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireEffWindAtHead (mi/h)
 */

void EqCalc::FireLengthToWidth( void )
{
    // Access current input values
    double effWind = vSurfaceFireEffWindAtHead->m_nativeValue;
    // Calculate results
    double lwRatio = FBL_SurfaceFireLengthToWidthRatio( effWind );
    // Store results
    vSurfaceFireLengthToWidth->update( lwRatio );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireLengthToWidth() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireEffWindAtHead %g %s\n", Margin,
            vSurfaceFireEffWindAtHead->m_nativeValue,
            vSurfaceFireEffWindAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLengthToWidth %g %s\n", Margin,
            vSurfaceFireLengthToWidth->m_nativeValue,
            vSurfaceFireLengthToWidth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireLineIntAtHead
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireLineIntAtHead (Btu/ft/s)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 *      vSurfaceFireResidenceTime (min)
 *      vSurfaceFireSpreadAtHead (ft/min)
 */

void EqCalc::FireLineIntAtHead( void )
{
    // Access current input values
    double tau = vSurfaceFireResidenceTime->m_nativeValue;
    double ros = vSurfaceFireSpreadAtHead->m_nativeValue;
    double rxi = vSurfaceFireReactionInt->m_nativeValue;
    // Calculate results
    double fli = FBL_SurfaceFireFirelineIntensity( ros, rxi, tau ) ;
    // Store results
    vSurfaceFireLineIntAtHead->update( fli );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireLineIntAtHead() 3 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireResidenceTime %g %s\n", Margin,
            vSurfaceFireResidenceTime->m_nativeValue,
            vSurfaceFireResidenceTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLineIntAtHead %g %s\n", Margin,
            vSurfaceFireLineIntAtHead->m_nativeValue,
            vSurfaceFireLineIntAtHead->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireLineIntAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireLineIntAtVector (Btu/ft/s)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 *      vSurfaceFireResidenceTime (min)
 *      vSurfaceFireSpreadAtVector (ft/min)
 */

void EqCalc::FireLineIntAtVector( void )
{
    // Access current input values
    double tau = vSurfaceFireResidenceTime->m_nativeValue;
    double ros = vSurfaceFireSpreadAtVector->m_nativeValue;
    double rxi = vSurfaceFireReactionInt->m_nativeValue;
    // Calculate results
    // Calculate results
    double fli = FBL_SurfaceFireFirelineIntensity( ros, rxi, tau ) ;
    // Store results
    vSurfaceFireLineIntAtVector->update( fli );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireLineIntAtVector() 3 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireResidenceTime %g %s\n", Margin,
            vSurfaceFireResidenceTime->m_nativeValue,
            vSurfaceFireResidenceTime->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtVector %g %s\n", Margin,
            vSurfaceFireSpreadAtVector->m_nativeValue,
            vSurfaceFireSpreadAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLineIntAtVector %g %s\n", Margin,
            vSurfaceFireLineIntAtVector->m_nativeValue,
            vSurfaceFireLineIntAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireMapDistAtBack
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireMapDistAtBack (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireDistAtBack (ft)
 *      vMapScale (in/mi)
 */

void EqCalc::FireMapDistAtBack( void )
{
    // Access current input values
    double bd = vSurfaceFireDistAtBack->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * bd / 5280.;
    // Store results
    vSurfaceFireMapDistAtBack->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireMapDistAtBack() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireDistAtBack %g %s\n", Margin,
            vSurfaceFireDistAtBack->m_nativeValue,
            vSurfaceFireDistAtBack->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMapDistAtBack %g %s\n", Margin,
            vSurfaceFireMapDistAtBack->m_nativeValue,
            vSurfaceFireMapDistAtBack->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireMapDistAtHead
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireMapDistAtHead (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireDistAtHead (ft)
 *      vMapScale (in/mi)
 */

void EqCalc::FireMapDistAtHead( void )
{
    // Access current input values
    double fd = vSurfaceFireDistAtHead->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * fd / 5280.;
    // Store results
    vSurfaceFireMapDistAtHead->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireMapDistAtHead() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireDistAtHead %g %s\n", Margin,
            vSurfaceFireDistAtHead->m_nativeValue,
            vSurfaceFireDistAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMapDistAtHead %g %s\n", Margin,
            vSurfaceFireMapDistAtHead->m_nativeValue,
            vSurfaceFireMapDistAtHead->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireMapDistAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireMapDistAtVector (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireDistAtVector (ft)
 *      vMapScale (in/mi)
 */

void EqCalc::FireMapDistAtVector( void )
{
    // Access current input values
    double vd = vSurfaceFireDistAtVector->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * vd / 5280.;
    // Store results
    vSurfaceFireMapDistAtVector->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireMapDistAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireDistAtVector %g %s\n", Margin,
            vSurfaceFireDistAtVector->m_nativeValue,
            vSurfaceFireDistAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMapDistAtVector %g %s\n", Margin,
            vSurfaceFireMapDistAtVector->m_nativeValue,
            vSurfaceFireMapDistAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireMaxDirDiagram
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireMaxDirDiagram (diagram)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireMaxDirFromNorth (degrees clockwise from North)
 *      vSurfaceFireMaxDirFromUpslope (degrees clockwise from North)
 *      vSiteAspectDirFromNorth (degrees clockwise from North)
 *      vSiteUpslopeDirFromNorth (degrees clockwise from North)
 *      vWindDirFromNorth (degrees clockwqise from North)
 *      vWindDirFromUpslope (degrees clockwqise from North)
 */

void EqCalc::FireMaxDirDiagram( void )
{
    // Access current input values
    double x = vSurfaceFireMaxDirDiagram->m_nativeValue;
    // Calculate results (dummy)
    x++;
    // Store results
    vSurfaceFireMaxDirDiagram->update( x );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireMaxDirDiagram() 3 1\n",
            Margin );
        fprintf( m_log, "%s  i vSurfaceFireMaxDirFromUpslope %g %d %s\n", Margin,
            vSurfaceFireMaxDirFromUpslope->m_displayValue,
            vSurfaceFireMaxDirFromUpslope->m_displayDecimals,
            vSurfaceFireMaxDirFromUpslope->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireMaxDirFromNorth %g %d %s\n", Margin,
            vSurfaceFireMaxDirFromNorth->m_displayValue,
            vSurfaceFireMaxDirFromNorth->m_displayDecimals,
            vSurfaceFireMaxDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromUpslope %g %d %s\n", Margin,
            vWindDirFromUpslope->m_displayValue,
            vWindDirFromUpslope->m_displayDecimals,
            vWindDirFromUpslope->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromNorth %g %d %s\n", Margin,
            vWindDirFromNorth->m_displayValue,
            vWindDirFromNorth->m_displayDecimals,
            vWindDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSiteAspectDirFromNorth %g %d %s\n", Margin,
            vSiteAspectDirFromNorth->m_displayValue,
            vSiteAspectDirFromNorth->m_displayDecimals,
            vSiteAspectDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSiteUpslopeDirFromNorth %g %d %s\n", Margin,
            vSiteUpslopeDirFromNorth->m_displayValue,
            vSiteUpslopeDirFromNorth->m_displayDecimals,
            vSiteUpslopeDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMaxDirDiagram %g %s\n", Margin,
            vSurfaceFireMaxDirDiagram->m_nativeValue,
            vSurfaceFireMaxDirDiagram->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireMaxDirFromNorth
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireMaxDirFromNorth (degrees clockwise from north)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireMaxDirFromUpslope (degrees clockwise from upslope)
 *      vSiteUpslopeDirFromNorth (degrees clockwise from North)
 */

void EqCalc::FireMaxDirFromNorth( void )
{
    // Access current input values
    double sd = vSiteUpslopeDirFromNorth->m_nativeValue;
    double fd = vSurfaceFireMaxDirFromUpslope->m_nativeValue;
    // Calculate results
    double dd = sd + fd;
    if ( dd >= 360. )
    {
        dd -= 360.;
    }
    if ( dd < 0. )
    {
        dd += 360.;
    }
    if ( fabs( dd ) < 0.50 )
    {
        dd = 0.0;
    }
    // Store results
    vSurfaceFireMaxDirFromNorth->update( dd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireMaxcDirFromNorth() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSiteUpslopeDirFromNorth %g %s\n", Margin,
            vSiteUpslopeDirFromNorth->m_nativeValue,
            vSiteUpslopeDirFromNorth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireMaxDirFromUpslope %g %s\n", Margin,
            vSurfaceFireMaxDirFromUpslope->m_nativeValue,
            vSurfaceFireMaxDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMaxDirFromNorth %g %s\n", Margin,
            vSurfaceFireMaxDirFromNorth->m_nativeValue,
            vSurfaceFireMaxDirFromNorth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireNoWindRate
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireNoWindRate (ft/min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedHeatSink (Btu/ft3)
 *      vSurfaceFirePropagatingFlux (fraction)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 */

void EqCalc::FireNoWindRate ( void )
{
    // Access current input values
    double rbQig    = vSurfaceFuelBedHeatSink->m_nativeValue;
    double rxInt    = vSurfaceFireReactionInt->m_nativeValue;
    double propFlux = vSurfaceFirePropagatingFlux->m_nativeValue;
    // Calculate results
    double ros0 = FBL_SurfaceFireNoWindNoSlopeSpreadRate(
                    rxInt, propFlux, rbQig ) ;
    // Store results
    vSurfaceFireNoWindRate->update( ros0 );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireNoWindRate() 3 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedHeatSink %g %s\n", Margin,
            vSurfaceFuelBedHeatSink->m_nativeValue,
            vSurfaceFuelBedHeatSink->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFirePropagatingFlux %g %s\n", Margin,
            vSurfaceFirePropagatingFlux->m_nativeValue,
            vSurfaceFirePropagatingFlux->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireNoWindRate %g %s\n", Margin,
            vSurfaceFireNoWindRate->m_nativeValue,
            vSurfaceFireNoWindRate->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FirePerimeter
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFirePerimeter (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengDist (ft)
 *      vSurfaceFireWidthDist (ft)
 */

void EqCalc::FirePerimeter( void )
{
    // Access current input values
    double ld = vSurfaceFireLengDist->m_nativeValue;
    double wd = vSurfaceFireWidthDist->m_nativeValue;
    // Calculate results
    double perim = FBL_SurfaceFirePerimeter( ld, wd );
    // Store results
    vSurfaceFirePerimeter->update( perim );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FirePerimeter() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLengDist %g %s\n", Margin,
            vSurfaceFireLengDist->m_nativeValue,
            vSurfaceFireLengDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireWidthDist %g %s\n", Margin,
            vSurfaceFireWidthDist->m_nativeValue,
            vSurfaceFireWidthDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFirePerimeter %g %s\n", Margin,
            vSurfaceFirePerimeter->m_nativeValue,
            vSurfaceFirePerimeter->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FirePropagatingFlux
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFirePropagatingFlux (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedPackingRatio (fraction)
 *      vSurfaceFuelBedSigma (ft2/ft3)
 */

void EqCalc::FirePropagatingFlux( void )
{
    // Access current input values
    double beta = vSurfaceFuelBedPackingRatio->m_nativeValue;
    double sigma = vSurfaceFuelBedSigma->m_nativeValue;
    // Calculate results
    double propFlux = FBL_SurfaceFirePropagatingFlux( beta, sigma );
    // Store results
    vSurfaceFirePropagatingFlux->update( propFlux );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FirePropagatingFlux() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedPackingRatio %g %s\n", Margin,
            vSurfaceFuelBedPackingRatio->m_nativeValue,
            vSurfaceFuelBedPackingRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedSigma %g %s\n", Margin,
            vSurfaceFuelBedSigma->m_nativeValue,
            vSurfaceFuelBedSigma->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFirePropagatingFlux %g %s\n", Margin,
            vSurfaceFirePropagatingFlux->m_nativeValue,
            vSurfaceFirePropagatingFlux->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireReactionInt
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 *      vSurfaceFireReactionIntDead (Btu/ft2/min)
 *      vSurfaceFireReactionIntLive (Btu/ft2/min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedMextDead (fraction)
 *      vSurfaceFuelBedMoisDead (fraction)
 *      vSurfaceFuelBedMextLive (fraction)
 *      vSurfaceFuelBedMoisLive (fraction)
 */

void EqCalc::FireReactionInt( void )
{
    // Access current input values
    double deadMext = vSurfaceFuelBedMextDead->m_nativeValue;
    double deadMois = vSurfaceFuelBedMoisDead->m_nativeValue;
    double liveMext = vSurfaceFuelBedMextLive->m_nativeValue;
    double liveMois = vSurfaceFuelBedMoisLive->m_nativeValue;
    // Calculate results
    double rxIntDead = 0.0;
    double rxIntLive = 0.0;
    double rxInt = FBL_SurfaceFireReactionIntensity( deadMois, deadMext,
                        liveMois, liveMext, &rxIntDead, &rxIntLive );
    // Store results
    vSurfaceFireReactionInt->update( rxInt );
    vSurfaceFireReactionIntDead->update( rxIntDead );
    vSurfaceFireReactionIntLive->update( rxIntLive );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireReactionIntensity() 4 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedMextDead %g %s\n", Margin,
            vSurfaceFuelBedMextDead->m_nativeValue,
            vSurfaceFuelBedMextDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedMoisDead %g %s\n", Margin,
            vSurfaceFuelBedMoisDead->m_nativeValue,
            vSurfaceFuelBedMoisDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedMextLive %g %s\n", Margin,
            vSurfaceFuelBedMextLive->m_nativeValue,
            vSurfaceFuelBedMextLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedMoisLive %g %s\n", Margin,
            vSurfaceFuelBedMoisLive->m_nativeValue,
            vSurfaceFuelBedMoisLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireResidenceTime
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireResidenceTime (min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedSigma (ft2/ft3)
 */

void EqCalc::FireResidenceTime( void )
{
    // Access current input values
    double sigma = vSurfaceFuelBedSigma->m_nativeValue;
    // Calculate results
    double rt = FBL_SurfaceFireResidenceTime( sigma );
    // Store results
    vSurfaceFireResidenceTime->update( rt );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireResidenceTime() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedSigma %g %s\n", Margin,
            vSurfaceFuelBedSigma->m_nativeValue,
            vSurfaceFuelBedSigma->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireResidenceTime %g %s\n", Margin,
            vSurfaceFireResidenceTime->m_nativeValue,
            vSurfaceFireResidenceTime->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireScorchHtFromFlameLengAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireScorchHtatVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vSurfaceFireFlameLengAtVector (ft)
 *      vWindSpeedAtMidflame (mi/h)
 *
 *  Intermediate Variables
 *      bi - Byram's fireline intensity (btu/ft/s)
 */

void EqCalc::FireScorchHtFromFlameLengAtVector( void )
{
    // Access current input values
    double airTemp     = vWthrAirTemp->m_nativeValue;
    double windSpeed   = vWindSpeedAtMidflame->m_nativeValue;
    double flameLength = vSurfaceFireFlameLengAtVector->m_nativeValue;
    // Calculate results
    double firelineIntensity = FBL_SurfaceFireFirelineIntensity( flameLength );
    double scorchHt = FBL_SurfaceFireScorchHeight( firelineIntensity,
                        windSpeed, airTemp );
    // Store results
    vSurfaceFireScorchHtAtVector->update( scorchHt );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireScorchHtAtVector() 3 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireScorchHtFromFliAtVector
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireScorchHtAtVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vSurfaceFireLineIntAtVector (Btu/ft/s)
 *      vSurfaceFireFlameLeng (ft)
 */

void EqCalc::FireScorchHtFromFliAtVector( void )
{
    // Access current input values
    double airTemp           = vWthrAirTemp->m_nativeValue;
    double windSpeed         = vWindSpeedAtMidflame->m_nativeValue;
    double firelineIntensity = vSurfaceFireLineIntAtVector->m_nativeValue;
    // Calculate results
    double scorchHt = FBL_SurfaceFireScorchHeight( firelineIntensity,
                            windSpeed, airTemp );
    // Store results
    vSurfaceFireScorchHtAtVector->update( scorchHt );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireScorchHtAtVector() 3 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireLineIntAtVector %g %s\n", Margin,
            vSurfaceFireLineIntAtVector->m_nativeValue,
            vSurfaceFireLineIntAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireShapeDiagram
 *
 *  Dependent Variables (outputs)
 *      vSurfaceFireShapeDiagram (diagram)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireArea
 *      vSurfaceFireDistAtHead
 *      vSurfaceFireDistAtBack
 *      vSurfaceFireElapsedTime
 *      vSurfaceFireLengDist
 *      vSurfaceFirePerimeter
 *      vSurfaceFireMaxDirFromNorth (degrees clockwise from North)
 *      vSurfaceFireMaxDirFromUpslope (degrees clockwise from Upslope)
 *      vSurfaceFireWidthDist
 *      vSiteAspectDirFromNorth
 *      vWindDirFromUpslope
 *      vWindDirFromNorth
 */

void EqCalc::FireShapeDiagram( void )
{
    // Access current input values
    double x = vSurfaceFireShapeDiagram->m_nativeValue;
    // Calculate results (dummy)
    x++;
    // Store results
    vSurfaceFireShapeDiagram->update( x );
    // Log results
    // The DISPLAY values, decimals, and units are printed so they can
    // annotate the diagram.
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireShapeDiagram() 8 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLengDist %g %d %s\n", Margin,
            vSurfaceFireLengDist->m_displayValue,
            vSurfaceFireLengDist->m_displayDecimals,
            vSurfaceFireLengDist->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireWidthDist %g %d %s\n", Margin,
            vSurfaceFireWidthDist->m_displayValue,
            vSurfaceFireWidthDist->m_displayDecimals,
            vSurfaceFireWidthDist->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireArea %g %d %s\n", Margin,
            vSurfaceFireArea->m_displayValue,
            vSurfaceFireArea->m_displayDecimals,
            vSurfaceFireArea->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFirePerimeter %g %d %s\n", Margin,
            vSurfaceFirePerimeter->m_displayValue,
            vSurfaceFirePerimeter->m_displayDecimals,
            vSurfaceFirePerimeter->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireElapsedTime %g %d %s\n", Margin,
            vSurfaceFireElapsedTime->m_displayValue,
            vSurfaceFireElapsedTime->m_displayDecimals,
            vSurfaceFireElapsedTime->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireDistAtHead %g %d %s\n", Margin,
            vSurfaceFireDistAtHead->m_displayValue,
            vSurfaceFireDistAtHead->m_displayDecimals,
            vSurfaceFireDistAtHead->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireDistAtBack %g %d %s\n", Margin,
            vSurfaceFireDistAtBack->m_displayValue,
            vSurfaceFireDistAtBack->m_displayDecimals,
            vSurfaceFireDistAtBack->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireMaxDirFromUpslope %g %d %s\n", Margin,
            vSurfaceFireMaxDirFromUpslope->m_displayValue,
            vSurfaceFireMaxDirFromUpslope->m_displayDecimals,
            vSurfaceFireMaxDirFromUpslope->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireMaxDirFromNorth %g %d %s\n", Margin,
            vSurfaceFireMaxDirFromNorth->m_displayValue,
            vSurfaceFireMaxDirFromNorth->m_displayDecimals,
            vSurfaceFireMaxDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromUpslope %g %d %s\n", Margin,
            vWindDirFromUpslope->m_displayValue,
            vWindDirFromUpslope->m_displayDecimals,
            vWindDirFromUpslope->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromNorth %g %d %s\n", Margin,
            vWindDirFromNorth->m_displayValue,
            vWindDirFromNorth->m_displayDecimals,
            vWindDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  i vSiteAspectDirFromNorth %g %d %s\n", Margin,
            vSiteAspectDirFromNorth->m_displayValue,
            vSiteAspectDirFromNorth->m_displayDecimals,
            vSiteAspectDirFromNorth->m_displayUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireShapeDiagram %g %s\n", Margin,
            vSurfaceFireShapeDiagram->m_nativeValue,
            vSurfaceFireShapeDiagram->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireSpreadAtBack
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireSpreadAtBack (ft/min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireEccentricity (fraction)
 *      vSurfaceFireSpreadAtHead (ft/min)
 */

void EqCalc::FireSpreadAtBack( void )
{
    // Access current input values
    double eccent = vSurfaceFireEccentricity->m_nativeValue;
    double rosMax = vSurfaceFireSpreadAtHead->m_nativeValue;
    // Calculate results
    double rosBacking = FBL_SurfaceFireBackingSpreadRate( rosMax, eccent );
    // Store results
    vSurfaceFireSpreadAtBack->update( rosBacking );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireSpreadAtBack() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireEccentricity %g %s\n", Margin,
            vSurfaceFireEccentricity->m_nativeValue,
            vSurfaceFireEccentricity->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireSpreadAtBack %g %s\n", Margin,
            vSurfaceFireSpreadAtBack->m_nativeValue,
            vSurfaceFireSpreadAtBack->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireSpreadAtBeta
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireSpreadAtVector (ft/min)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireEccentricity (fraction)
 *      vSurfaceFireSpreadAtHead (ft/min)
 *      vSurfaceFireVectorBeta (degrees)
 */
void EqCalc::FireSpreadAtBeta( void )
{
    // Access current input values
    double eccent = vSurfaceFireEccentricity->m_nativeValue;
    double rosMax = vSurfaceFireSpreadAtHead->m_nativeValue;
    double beta   = vSurfaceFireVectorBeta->m_nativeValue;
    // Calculate results
    double rosVec   = FBL_SurfaceFireSpreadRateAtBeta( rosMax, eccent, beta );
    // Store results
    vSurfaceFireSpreadAtVector->update( rosVec );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireSpreadAtBeta() 3 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireEccentricity %g %s\n", Margin,
            vSurfaceFireEccentricity->m_nativeValue,
            vSurfaceFireEccentricity->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireVectorBeta %g %s\n", Margin,
            vSurfaceFireVectorBeta->m_nativeValue,
            vSurfaceFireVectorBeta->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireSpreadAtHead
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireSpreadAtHead (ft/min)
 *      vSurfaceFireMaxDirFromUpslope (clockwise from upslope)
 *      vSurfaceFireEffWindAtHead (mi/h)
 *      vSurfaceFireWindSpeedLimit (mi/h)
 *      vSurfaceFireWindSpeedFlag (flag)
 *      vSurfaceFiewWindFactor (dl)
 *      vSurfaceFiewSlopeFactor (dl)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireNoWindRate (ft/min)
 *      vSiteSlopeFraction (rise/reach)
 *      vWindDirFromUpslope (degrees)
 *      vWindSpeedAtMidflame (mi/h)
 *
 *  Intermediate Variables
 *      phiEw
 */

void EqCalc::FireSpreadAtHead( void )
{
    // Access current input values
    double ros0  = vSurfaceFireNoWindRate->m_nativeValue;
    double rxi   = vSurfaceFireReactionInt->m_nativeValue;
    double slope = vSiteSlopeFraction->m_nativeValue;
    double windSpeed = vWindSpeedAtMidflame->m_nativeValue;
    double windDir = vWindDirFromUpslope->m_nativeValue;

    // We'll need to get some properties
    PropertyDict *prop = m_eqTree->m_propDict;
    bool applyWindLimit = prop->boolean( "surfaceConfWindLimitApplied" );
    bool aspenFuels = prop->boolean( "surfaceConfFuelAspen" );
    if ( aspenFuels )
    {
        applyWindLimit = false;
    }

    // Calculate results
    double dirMax, effWind, maxWind, windFactor, slopeFactor;
    int windLimit;
    double rosMax = FBL_SurfaceFireForwardSpreadRate( ros0, rxi, slope,
        windSpeed, windDir, &dirMax, &effWind, &maxWind, &windLimit,
        &windFactor, &slopeFactor, applyWindLimit );
    // Store results
    vSurfaceFireSpreadAtHead->update( rosMax );
    vSurfaceFireMaxDirFromUpslope->update( dirMax );
    vSurfaceFireEffWindAtHead->update( effWind );
    vSurfaceFireWindSpeedLimit->update( maxWind );
    vSurfaceFireWindSpeedFlag->updateItem( windLimit );
    vSurfaceFireWindFactor->update( windFactor );
    vSurfaceFireSlopeFactor->update( slopeFactor );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireSpreadAtHead() 4 5\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireNoWindRate %g %s\n", Margin,
            vSurfaceFireNoWindRate->m_nativeValue,
            vSurfaceFireNoWindRate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteSlopeFraction %g %s\n", Margin,
            vSiteSlopeFraction->m_nativeValue,
            vSiteSlopeFraction->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromUpslope %g %s\n", Margin,
            vWindDirFromUpslope->m_nativeValue,
            vWindDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMaxDirFromUpslope %g %s\n", Margin,
            vSurfaceFireMaxDirFromUpslope->m_nativeValue,
            vSurfaceFireMaxDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireEffWindAtHead %g %s\n", Margin,
            vSurfaceFireEffWindAtHead->m_nativeValue,
            vSurfaceFireEffWindAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWindSpeedLimit %g %s\n", Margin,
            vSurfaceFireWindSpeedLimit->m_nativeValue,
            vSurfaceFireWindSpeedLimit->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWindSpeedFlag %g %s\n", Margin,
            vSurfaceFireWindSpeedFlag->m_nativeValue,
            vSurfaceFireWindSpeedFlag->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWindFactor %g %s\n", Margin,
            vSurfaceFireWindFactor->m_nativeValue,
            vSurfaceFireWindFactor->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireSlopeFactor %g %s\n", Margin,
            vSurfaceFireSlopeFactor->m_nativeValue,
            vSurfaceFireSlopeFactor->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireVectorBeta
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireVectorBeta (degrees)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireMaxDirFromUpslope (degrees)
 *      vSurfaceFireVectorDirFromUpslope (degrees)
 */
void EqCalc::FireVectorBeta( void )
{
    // Access current input values
    double rosMaxAz = vSurfaceFireMaxDirFromUpslope->m_nativeValue;
    double rosVecAz = vSurfaceFireVectorDirFromUpslope->m_nativeValue;
    // Calculate results
    double beta = FBL_SurfaceFireVectorBeta( rosMaxAz, rosVecAz );
    // Store restults
    vSurfaceFireVectorBeta->update( beta );
    // Log results
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireVectorBeta() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireMaxDirFromUpslope %g %s\n", Margin,
            vSurfaceFireMaxDirFromUpslope->m_nativeValue,
            vSurfaceFireMaxDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireVectorDirFromUpslope %g %s\n", Margin,
            vSurfaceFireVectorDirFromUpslope->m_nativeValue,
            vSurfaceFireVectorDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireVectorBeta %g %s\n", Margin,
            vSurfaceFireVectorBeta->m_nativeValue,
            vSurfaceFireVectorBeta->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireVectorDirFromNorth
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireVectorDirFromNorth (degrees clockwise from North)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireVectorDirFromCompass (compass point name)
 */

void EqCalc::FireVectorDirFromNorth( void )
{
    // Access current input values
    int id = vSurfaceFireVectorDirFromCompass->activeItemDataIndex();
    // Calculate results
    double fd = 22.5 * id;
    // Store results
    vSurfaceFireVectorDirFromNorth->update( fd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireVectorDirFromNorth() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireVectorDirFromCompass %d %s\n", Margin,
            vSurfaceFireVectorDirFromCompass->activeItemDataIndex(),
            vSurfaceFireVectorDirFromCompass->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSurfaceFireVectorDirFromNorth %g %s\n", Margin,
            vSurfaceFireVectorDirFromNorth->m_nativeValue,
            vSurfaceFireVectorDirFromNorth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireVectorDirFromUpslope
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireVectorDirFromUpslope (degrees clockwise from upslope)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireVectorDirFromNorth (degrees clockwise from north)
 *      vSiteUpslopeDirFromNorth (degrees clockwise from north)
 */

void EqCalc::FireVectorDirFromUpslope( void )
{
    // Access current input values
    double sd = vSiteUpslopeDirFromNorth->m_nativeValue;
    double fd = vSurfaceFireVectorDirFromNorth->m_nativeValue;
    // Calculate results
    double dd = fd - sd;
    if ( dd < 0. )
    {
        dd += 360.;
    }
    // Store results
    vSurfaceFireVectorDirFromUpslope->update( dd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireVectorDirFromUpslope() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSiteUpslopeDirFromNorth %g %s\n", Margin,
            vSiteUpslopeDirFromNorth->m_nativeValue,
            vSiteUpslopeDirFromNorth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireVectorDirFromNorth %g %s\n", Margin,
            vSurfaceFireVectorDirFromNorth->m_nativeValue,
            vSurfaceFireVectorDirFromNorth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireVectorDirFromUpslope %g %s\n", Margin,
            vSurfaceFireVectorDirFromUpslope->m_nativeValue,
            vSurfaceFireVectorDirFromUpslope->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireWidthDist
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireWidthDist (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireLengDist (ft)
 *      vSurfaceFireLengthToWidth (fraction)
 */

void EqCalc::FireWidthDist( void )
{
    // Access current input values
    double ld = vSurfaceFireLengDist->m_nativeValue;
    double lw = vSurfaceFireLengthToWidth->m_nativeValue;
    // Calculate results
    double wd = FBL_SurfaceFireWidth( ld, lw );
    // Store results
    vSurfaceFireWidthDist->update( wd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireWidthDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireLengDist %g %s\n", Margin,
            vSurfaceFireLengDist->m_nativeValue,
            vSurfaceFireLengDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireLengthToWidth %g %s\n", Margin,
            vSurfaceFireLengthToWidth->m_nativeValue,
            vSurfaceFireLengthToWidth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWidthDist %g %s\n", Margin,
            vSurfaceFireWidthDist->m_nativeValue,
            vSurfaceFireWidthDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FireWidthMapDist
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireWidthMapDist (in)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireWidthDist (ft)
 *      vMapScale (fraction)
 */

void EqCalc::FireWidthMapDist( void )
{
    // Access current input values
    double wd = vSurfaceFireWidthDist->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * wd / 5280.;
    // Store results
    vSurfaceFireWidthMapDist->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FireWidthMapDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireWidthDist %g %s\n", Margin,
            vSurfaceFireWidthDist->m_nativeValue,
            vSurfaceFireWidthDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWidthMapDist %g %s\n", Margin,
            vSurfaceFireWidthMapDist->m_nativeValue,
            vSurfaceFireWidthMapDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelAspenModel
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelBedDepth (ft)
 *      vSurfaceFuelBedMextDead (lb/lb)
 *      vSurfaceFuelAspenLoadDead1 (lb/ft2)
 *      vSurfaceFuelAspenLoadDead10 (lb/ft2)
 *      vSurfaceFuelAspenLoadLiveHerb (lb/ft2)
 *      vSurfaceFuelAspenLoadLiveWoody (lb/ft2)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelAspenCuring (%)
 *      vSurfaceFuelAspenType (discrete)
 */

void EqCalc::FuelAspenModel( void )
{
    // Access current input values
    double curing    = vSurfaceFuelAspenCuring->m_nativeValue;
    int    typeIndex = vSurfaceFuelAspenType->activeItemDataIndex();
    // Calculate
    double depth         = FBL_AspenFuelBedDepth( typeIndex, curing );
    double mextDead      = FBL_AspenFuelMextDead( typeIndex, curing );
    double loadDead1     = FBL_AspenLoadDead1( typeIndex, curing );
    double loadDead10    = FBL_AspenLoadDead10( typeIndex, curing );
    double loadLiveHerb  = FBL_AspenLoadLiveHerb( typeIndex, curing );
    double loadLiveWoody = FBL_AspenLoadLiveWoody( typeIndex, curing );
    double savrDead1     = FBL_AspenSavrDead1( typeIndex, curing );
    double savrDead10    = FBL_AspenSavrDead10( typeIndex, curing );
    double savrLiveHerb  = FBL_AspenSavrLiveHerb( typeIndex, curing );
    double savrLiveWoody = FBL_AspenSavrLiveWoody( typeIndex, curing );
    // Update
    vSurfaceFuelBedDepth->update( depth );
    vSurfaceFuelBedMextDead->update( mextDead );
    vSurfaceFuelAspenLoadDead1->update( loadDead1 );
    vSurfaceFuelAspenLoadDead10->update( loadDead10 );
    vSurfaceFuelAspenLoadLiveHerb->update( loadLiveHerb );
    vSurfaceFuelAspenLoadLiveWoody->update( loadLiveWoody );
    vSurfaceFuelAspenSavrDead1->update( savrDead1 );
    vSurfaceFuelAspenSavrDead10->update( savrDead10 );
    vSurfaceFuelAspenSavrLiveHerb->update( savrLiveHerb );
    vSurfaceFuelAspenSavrLiveWoody->update( savrLiveWoody );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelAspenModel() 2 10\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelAspenCuring %g %s\n", Margin,
            vSurfaceFuelAspenCuring->m_nativeValue,
            vSurfaceFuelAspenCuring->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSeverityAspen %d %s\n", Margin,
            vSurfaceFireSeverityAspen->activeItemDataIndex(),
            vSurfaceFireSeverityAspen->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedDepth %g %s\n", Margin,
            vSurfaceFuelBedDepth->m_nativeValue,
            vSurfaceFuelBedDepth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedMextDead %g %s\n", Margin,
            vSurfaceFuelBedMextDead->m_nativeValue,
            vSurfaceFuelBedMextDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenLoadDead1 %g %s\n", Margin,
            vSurfaceFuelAspenLoadDead1->m_nativeValue,
            vSurfaceFuelAspenLoadDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenLoadDead10 %g %s\n", Margin,
            vSurfaceFuelAspenLoadDead10->m_nativeValue,
            vSurfaceFuelAspenLoadDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenLoadLiveHerb %g %s\n", Margin,
            vSurfaceFuelAspenLoadLiveHerb->m_nativeValue,
            vSurfaceFuelAspenLoadLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenLoadLiveWoody %g %s\n", Margin,
            vSurfaceFuelAspenLoadLiveWoody->m_nativeValue,
            vSurfaceFuelAspenLoadLiveWoody->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenSavrDead1 %g %s\n", Margin,
            vSurfaceFuelAspenSavrDead1->m_nativeValue,
            vSurfaceFuelAspenSavrDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenSavrDead10 %g %s\n", Margin,
            vSurfaceFuelAspenSavrDead10->m_nativeValue,
            vSurfaceFuelAspenSavrDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenSavrLiveHerb %g %s\n", Margin,
            vSurfaceFuelAspenSavrLiveHerb->m_nativeValue,
            vSurfaceFuelAspenSavrLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelAspenSavrLiveWoody %g %s\n", Margin,
            vSurfaceFuelAspenSavrLiveWoody->m_nativeValue,
            vSurfaceFuelAspenSavrLiveWoody->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelAspenParms
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelDens# (lb/ft3)
 *      vSurfaceFuelHeat# (Btu/lb)
 *      vSurfaceFuelLife# (category)
 *      vSurfaceFuelLoad# (lb/ft2)
 *      vSurfaceFuelSavr# (ft2/ft3)
 *      vSurfaceFuelSeff# (fraction)
 *      vSurfaceFuelStot# (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelAspenDepth (ft)
 *      vSurfaceFuelAspenLoadDead1 (lb/ft2)
 *      vSurfaceFuelAspenLoadDead10 (lb/ft2)
 *      vSurfaceFuelAspenLoadDeadFoliage (lb/ft2)
 *      vSurfaceFuelAspenLoadLitter (lb/ft2)
 *      vSurfaceFuelAspenLoadLive1 (lb/ft2)
 *      vSurfaceFuelAspenLoadLive10 (lb/ft2)
 *      vSurfaceFuelAspenLoadLiveFoliage (lb/ft2)
 */

void EqCalc::FuelAspenParms( void )
{
    // Assign life values
    vSurfaceFuelLife[0]->updateItem( FuelLifeType_DeadTimeLag ); // Dead 0.0 - 0.25"
    vSurfaceFuelLife[1]->updateItem( FuelLifeType_DeadTimeLag ); // Dead 0.25 - 1.0"
    vSurfaceFuelLife[2]->updateItem( FuelLifeType_LiveHerb );    // Live herbaceous
    vSurfaceFuelLife[3]->updateItem( FuelLifeType_LiveWood );    // Live woody
    vSurfaceFuelLife[4]->updateItem( FuelLifeType_DeadTimeLag ); // Unused
    vSurfaceFuelLife[5]->updateItem( FuelLifeType_DeadTimeLag ); // Unused
    vSurfaceFuelLife[6]->updateItem( FuelLifeType_DeadTimeLag ); // Unused
    vSurfaceFuelLife[7]->updateItem( FuelLifeType_DeadTimeLag ); // Unused
    // Assign load values
    vSurfaceFuelLoad[0]->update( vSurfaceFuelAspenLoadDead1->m_nativeValue );
    vSurfaceFuelLoad[1]->update( vSurfaceFuelAspenLoadDead10->m_nativeValue );
    vSurfaceFuelLoad[2]->update( vSurfaceFuelAspenLoadLiveHerb->m_nativeValue );
    vSurfaceFuelLoad[3]->update( vSurfaceFuelAspenLoadLiveWoody->m_nativeValue );
    vSurfaceFuelLoad[4]->update( 0. );
    vSurfaceFuelLoad[5]->update( 0. );
    vSurfaceFuelLoad[6]->update( 0. );
    vSurfaceFuelLoad[7]->update( 0. );
    // Assign savr values
    vSurfaceFuelSavr[0]->update( vSurfaceFuelAspenSavrDead1->m_nativeValue );
    vSurfaceFuelSavr[1]->update( vSurfaceFuelAspenSavrDead10->m_nativeValue );
    vSurfaceFuelSavr[2]->update( vSurfaceFuelAspenSavrLiveHerb->m_nativeValue );
    vSurfaceFuelSavr[3]->update( vSurfaceFuelAspenSavrLiveWoody->m_nativeValue );
    vSurfaceFuelSavr[4]->update( 30.0 );
    vSurfaceFuelSavr[5]->update( 30.0 );
    vSurfaceFuelSavr[6]->update( 30.0 );
    vSurfaceFuelSavr[7]->update( 30.0 );
    // Assign heat and stot values
    for ( int i = 0;
          i < MaxParts;
          i++ )
    {
        vSurfaceFuelDens[i]->update( 32.0 );
        vSurfaceFuelHeat[i]->update( 8000. );
        vSurfaceFuelSeff[i]->update( 0.010 );
        vSurfaceFuelStot[i]->update( 0.055 );
    }
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelAspenParms() 0 0\n", Margin );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedHeatSink
 *      Performs all computations affected by changes in any of the
 *      fuel particle moisture contents.
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelBedHeatSink (Btu/ft3)
 *      vSurfaceFuelBedMextLive (fraction)
 *      vSurfaceFuelBedMoisDead (fraction)
 *      vSurfaceFuelBedMoisLive (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedBulkDensity (lb/ft3)
 *      vSurfaceFuelBedMextDead (fraction)
 *      vSurfaceFuelMois# (fraction)
 */

void EqCalc::FuelBedHeatSink( void )
{
    // Access current input values
    double mois[MaxParts];
    for ( int p = 0;
          p < MaxParts;
          p++)
    {
        mois[p] = vSurfaceFuelMois[p]->m_nativeValue;
    }
    // Calculate results
    double deadMois, liveMois, liveMext;
    double bulkDensity = vSurfaceFuelBedBulkDensity->m_nativeValue;
    double deadMext = vSurfaceFuelBedMextDead->m_nativeValue;
    double rbQig = FBL_SurfaceFuelBedHeatSink( bulkDensity, deadMext, mois,
        &deadMois, &liveMois, &liveMext );
    // Store results
    vSurfaceFuelBedMoisDead->update( deadMois );
    vSurfaceFuelBedMoisLive->update( liveMois );
    vSurfaceFuelBedMextLive->update( liveMext );
    vSurfaceFuelBedHeatSink->update( rbQig );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelBedHeatSink() 2 4\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedBulkDensity %g %s\n", Margin,
            vSurfaceFuelBedBulkDensity->m_nativeValue,
            vSurfaceFuelBedBulkDensity->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedMextDead %g %s\n", Margin,
            vSurfaceFuelBedMextDead->m_nativeValue,
            vSurfaceFuelBedMextDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedHeatSink %g %s\n", Margin,
            vSurfaceFuelBedHeatSink->m_nativeValue,
            vSurfaceFuelBedHeatSink->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedMoisLive %g %s\n", Margin,
            vSurfaceFuelBedMoisLive->m_nativeValue,
            vSurfaceFuelBedMoisLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedMextLive %g %s\n", Margin,
            vSurfaceFuelBedMextLive->m_nativeValue,
            vSurfaceFuelBedMextLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedMoisDead %g %s\n", Margin,
            vSurfaceFuelBedMoisDead->m_nativeValue,
            vSurfaceFuelBedMoisDead->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedIntermediates
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelBedBetaRatio (fraction)
 *      vSurfaceFuelBedBulkDensity (lbs/ft3)
 *      vSurfaceFuelBedDeadFraction (fraction)
 *		vSurfaceFuelBedLiveFraction (fraction)
 *      vSurfaceFuelBedPackingRatio (fraction)
 *      vSurfaceFuelBedSigma (ft2/ft3)
 *      vSurfaceFuelLoadDeadHerb (lbs/ft2)
 *      vSurfaceFuelLoadUndeadHerb (lbs/ft2)
 *      vSurfaceFuelLoadDead (lbs/ft2)
 *      vSurfaceFuelLoadLive (lbs/ft2)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedDepth (ft)
 *      vSurfaceFuelBedMextDead (lb/lb)
 *      vSurfaceFuelDens# (lbs/ft3)
 *      vSurfaceFuelHeat# (Btu/lb)
 *      vSurfaceFuelLife# (category)
 *      vSurfaceFuelLoad# (lbs/ft2)
 *      vSurfaceFuelMois# (fraction)
 *      vSurfaceFuelSavr# (ft2/ft3)
 *      vSurfaceFuelSeff# (fraction)
 *      vSurfaceFuelStot# (fraction)
 *      vSurfaceFuelLoadTransferFraction (fraction)
 */

void EqCalc::FuelBedIntermediates ( void )
{
    // Access current input values
    double dens[MaxParts], heat[MaxParts], load[MaxParts], savr[MaxParts];
    double seff[MaxParts], stot[MaxParts], mois[MaxParts];
    int    life[MaxParts];
    double deadLoad = 0.;
    double liveLoad = 0.;
    for ( int p = 0;
          p < MaxParts;
          p++ )
    {
        dens[p] = vSurfaceFuelDens[p]->m_nativeValue;
        heat[p] = vSurfaceFuelHeat[p]->m_nativeValue;
        life[p] = vSurfaceFuelLife[p]->activeItemDataIndex();
        load[p] = vSurfaceFuelLoad[p]->m_nativeValue;
        savr[p] = vSurfaceFuelSavr[p]->m_nativeValue;
        seff[p] = vSurfaceFuelSeff[p]->m_nativeValue;
        stot[p] = vSurfaceFuelStot[p]->m_nativeValue;
        mois[p] = vSurfaceFuelMois[p]->m_nativeValue;
        // Dead=0, Herb=1, Wood=2, Litter=3
        if ( life[p] == 0 || life[p] == 3 )
        {
            deadLoad += load[p];
        }
        else
        {
            liveLoad += load[p];
        }
    }
    // Fuel load transfer
    double fraction = 0.0;
    int transferEq = vSurfaceFuelLoadTransferEq->activeItemDataIndex();
    if ( transferEq )
    {
        fraction = vSurfaceFuelLoadTransferFraction->m_nativeValue;
    }
    int LiveHerb = 3;
    int DeadHerb = 5;
    if ( fraction > 0.00001 )
    {
        load[DeadHerb] = fraction * load[LiveHerb];
        load[LiveHerb] -= load[DeadHerb];
        deadLoad += load[DeadHerb];
        liveLoad -= load[DeadHerb];
    }
    // Fuel bed dead load fraction
    double totalLoad = deadLoad + liveLoad;
    double deadFraction = ( totalLoad < SMIDGEN )
                        ? ( 0. )
                        : ( deadLoad / totalLoad );
    // Fuel bed intermediates
    double depth    = vSurfaceFuelBedDepth->m_nativeValue;
    double deadMext = vSurfaceFuelBedMextDead->m_nativeValue;
    double bulkDensity, packingRatio, betaRatio;
    double sigma = FBL_SurfaceFuelBedIntermediates( depth, deadMext, MaxParts,
        life, load, savr, heat, dens, stot, seff,
        &bulkDensity, &packingRatio, &betaRatio );
    //double betaOpt = packingRatio / betaRatio;

//printf( "%-4.4s %6.1f %6.4f %6.4f %6.4f %6.4f\n",
//  vSurfaceFuelBedModel->activeItemName().latin1(),
//  sigma, bulkDensity, packingRatio, betaOpt, betaRatio );

    // Store results
    vSurfaceFuelBedSigma->update( sigma );
    vSurfaceFuelBedBulkDensity->update( bulkDensity );
    vSurfaceFuelBedPackingRatio->update( packingRatio );
    vSurfaceFuelBedBetaRatio->update( betaRatio );
    vSurfaceFuelBedDeadFraction->update( deadFraction );
    vSurfaceFuelBedLiveFraction->update( 1. - deadFraction );
    vSurfaceFuelLoadDeadHerb->update( load[DeadHerb] );
    vSurfaceFuelLoadUndeadHerb->update( load[LiveHerb] );
    vSurfaceFuelLoadDead->update( deadLoad );
    vSurfaceFuelLoadLive->update( liveLoad );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelBedIntermediates() 3 11\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelLoadTransferEq %d %s\n", Margin,
            vSurfaceFuelLoadTransferEq->activeItemDataIndex(),
            vSurfaceFuelLoadTransferEq->activeItemName().latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedDepth %g %s\n", Margin,
            vSurfaceFuelBedDepth->m_nativeValue,
            vSurfaceFuelBedDepth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedMextDead %g %s\n", Margin,
            vSurfaceFuelBedMextDead->m_nativeValue,
            vSurfaceFuelBedMextDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedBetaRatio %g %s\n", Margin,
            vSurfaceFuelBedBetaRatio->m_nativeValue,
            vSurfaceFuelBedBetaRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedBulkDensity %g %s\n", Margin,
            vSurfaceFuelBedBulkDensity->m_nativeValue,
            vSurfaceFuelBedBulkDensity->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedPackingRatio %g %s\n", Margin,
            vSurfaceFuelBedPackingRatio->m_nativeValue,
            vSurfaceFuelBedPackingRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedSigma %g %s\n", Margin,
            vSurfaceFuelBedSigma->m_nativeValue,
            vSurfaceFuelBedSigma->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadTransferFraction %g %s\n", Margin,
            vSurfaceFuelLoadTransferFraction->m_nativeValue,
            vSurfaceFuelLoadTransferFraction->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadDeadHerb %g %s\n", Margin,
            vSurfaceFuelLoadDeadHerb->m_nativeValue,
            vSurfaceFuelLoadDeadHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadUndeadHerb %g %s\n", Margin,
            vSurfaceFuelLoadUndeadHerb->m_nativeValue,
            vSurfaceFuelLoadUndeadHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadDead %g %s\n", Margin,
            vSurfaceFuelLoadDead->m_nativeValue,
            vSurfaceFuelLoadDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadLive %g %s\n", Margin,
            vSurfaceFuelLoadLive->m_nativeValue,
            vSurfaceFuelLoadLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedDeadFraction %g %s\n", Margin,
            vSurfaceFuelBedDeadFraction->m_nativeValue,
            vSurfaceFuelBedDeadFraction->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedLiveFraction %g %s\n", Margin,
            vSurfaceFuelBedLiveFraction->m_nativeValue,
            vSurfaceFuelBedLiveFraction->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedModel
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelLoadTransferEq (category)
 *      vSurfaceFuelBedDepth (ft)
 *      vSurfaceFuelBedMextDead (fraction )
 *      vSurfaceFuelHeatDead (Btu/lb)
 *      vSurfaceFuelHeatLive (Btu/lb)
 *      vSurfaceFuelLoadDead1 (lb/ft2)
 *      vSurfaceFuelLoadDead10 (lb/ft2)
 *      vSurfaceFuelLoadDead100 (lb/ft2)
 *      vSurfaceFuelLoadLiveHerb (lb/ft2)
 *      vSurfaceFuelLoadLiveWood (lb/ft2)
 *      vSurfaceFuelSavrDead1 (ft2/ft3)
 *      vSurfaceFuelSavrLiveHerb (ft2/ft3)
 *      vSurfaceFuelSavrLiveWood (ft2/ft3)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedModel (item)
 */

void EqCalc::FuelBedModel( void )
{
    // Access current input values
    FuelModel *fm = currentFuelModel( 0 );

    // Copy values from the FuelModel into the EqTree
    vSurfaceFuelLoadTransferEq->updateItem( fm->m_transfer );

    vSurfaceFuelBedDepth->update( fm->m_depth );
    vSurfaceFuelBedDepth->m_store.setNum( vSurfaceFuelBedDepth->m_displayValue,
        'f', vSurfaceFuelBedDepth->m_displayDecimals );

    vSurfaceFuelBedMextDead->update( fm->m_mext );
    vSurfaceFuelBedMextDead->m_store.setNum( vSurfaceFuelBedMextDead->m_displayValue,
        'f', vSurfaceFuelBedMextDead->m_displayDecimals );

    vSurfaceFuelHeatDead->update( fm->m_heatDead );
    vSurfaceFuelHeatDead->m_store.setNum( vSurfaceFuelHeatDead->m_displayValue,
        'f', vSurfaceFuelHeatDead->m_displayDecimals );

    vSurfaceFuelHeatLive->update( fm->m_heatLive );
    vSurfaceFuelHeatLive->m_store.setNum( vSurfaceFuelHeatLive->m_displayValue,
        'f', vSurfaceFuelHeatLive->m_displayDecimals );

    vSurfaceFuelLoadDead1->update( fm->m_load1 );
    vSurfaceFuelLoadDead1->m_store.setNum( vSurfaceFuelLoadDead1->m_displayValue,
        'f', vSurfaceFuelLoadDead1->m_displayDecimals );

    vSurfaceFuelLoadDead10->update( fm->m_load10 );
    vSurfaceFuelLoadDead10->m_store.setNum( vSurfaceFuelLoadDead10->m_displayValue,
        'f', vSurfaceFuelLoadDead10->m_displayDecimals );

    vSurfaceFuelLoadDead100->update( fm->m_load100 );
    vSurfaceFuelLoadDead100->m_store.setNum( vSurfaceFuelLoadDead100->m_displayValue,
        'f', vSurfaceFuelLoadDead100->m_displayDecimals );

    vSurfaceFuelLoadLiveHerb->update( fm->m_loadHerb );
    vSurfaceFuelLoadLiveHerb->m_store.setNum( vSurfaceFuelLoadLiveHerb->m_displayValue,
        'f', vSurfaceFuelLoadLiveHerb->m_displayDecimals );

    vSurfaceFuelLoadLiveWood->update( fm->m_loadWood );
    vSurfaceFuelLoadLiveWood->m_store.setNum( vSurfaceFuelLoadLiveWood->m_displayValue,
        'f', vSurfaceFuelLoadLiveWood->m_displayDecimals );

    vSurfaceFuelSavrDead1->update( fm->m_savr1 );
    vSurfaceFuelSavrDead1->m_store.setNum( vSurfaceFuelSavrDead1->m_displayValue,
        'f', vSurfaceFuelSavrDead1->m_displayDecimals );

    vSurfaceFuelSavrLiveHerb->update( fm->m_savrHerb );
    vSurfaceFuelSavrLiveHerb->m_store.setNum( vSurfaceFuelSavrLiveHerb->m_displayValue,
        'f', vSurfaceFuelSavrLiveHerb->m_displayDecimals );

    vSurfaceFuelSavrLiveWood->update( fm->m_savrWood );
    vSurfaceFuelSavrLiveWood->m_store.setNum( vSurfaceFuelSavrLiveWood->m_displayValue,
        'f', vSurfaceFuelSavrLiveWood->m_displayDecimals );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelBedModel() 1 13\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedModel %d %s\n", Margin,
            vSurfaceFuelBedModel->activeItemDataIndex(),
            vSurfaceFuelBedModel->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadTransferEq %d %s\n", Margin,
            vSurfaceFuelLoadTransferEq->activeItemDataIndex(),
            vSurfaceFuelLoadTransferEq->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedDepth %g %s\n", Margin,
            vSurfaceFuelBedDepth->m_nativeValue,
            vSurfaceFuelBedDepth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedMextDead %g %s\n", Margin,
            vSurfaceFuelBedMextDead->m_nativeValue,
            vSurfaceFuelBedMextDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelHeatDead %g %s\n", Margin,
            vSurfaceFuelHeatDead->m_nativeValue,
            vSurfaceFuelHeatDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelHeatLive %g %s\n", Margin,
            vSurfaceFuelHeatLive->m_nativeValue,
            vSurfaceFuelHeatLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadDead1 %g %s\n", Margin,
            vSurfaceFuelLoadDead1->m_nativeValue,
            vSurfaceFuelLoadDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadDead10 %g %s\n", Margin,
            vSurfaceFuelLoadDead10->m_nativeValue,
            vSurfaceFuelLoadDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadDead100 %g %s\n", Margin,
            vSurfaceFuelLoadDead100->m_nativeValue,
            vSurfaceFuelLoadDead100->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadLiveHerb %g %s\n", Margin,
            vSurfaceFuelLoadLiveHerb->m_nativeValue,
            vSurfaceFuelLoadLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadLiveWood %g %s\n", Margin,
            vSurfaceFuelLoadLiveWood->m_nativeValue,
            vSurfaceFuelLoadLiveWood->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelSavrDead1 %g %s\n", Margin,
            vSurfaceFuelSavrDead1->m_nativeValue,
            vSurfaceFuelSavrDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelSavrLiveHerb %g %s\n", Margin,
            vSurfaceFuelSavrLiveHerb->m_nativeValue,
            vSurfaceFuelSavrLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelSavrLiveWood %g %s\n", Margin,
            vSurfaceFuelSavrLiveWood->m_nativeValue,
            vSurfaceFuelSavrLiveWood->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedParms
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelDens# (lb/ft3)
 *      vSurfaceFuelHeat# (Btu/lb)
 *      vSurfaceFuelLife# (category)
 *      vSurfaceFuelLoad# (lb/ft2)
 *      vSurfaceFuelSavr# (ft2/ft3)
 *      vSurfaceFuelSeff# (fraction)
 *      vSurfaceFuelStot# (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelHeatDead (Btu/lb)
 *      vSurfaceFuelHeatLive (Btu/lb)
 *      vSurfaceFuelSavrDead1 (ft2/ft3)
 *      vSurfaceFuelSavrLiveHerb (ft2/ft3)
 *      vSurfaceFuelSavrLiveWood (ft2/ft3)
 *      vSurfaceFuelLoadDead1 (lbs/ft2)
 *      vSurfaceFuelLoadDead100 (lbs/ft2)
 *      vSurfaceFuelLoadDead100 (lbs/ft2)
 *      vSurfaceFuelLoadLiveHerb (lbs/ft2)
 *      vSurfaceFuelLoadLiveWood (lbs/ft2)
 *
 *  Notes
 *      This allows input of the traditional fuel bed parameters
 *      which are then assigned to the vSurfaceFuelLife#, vSurfaceFuelLoad#, vSurfaceFuelSavr#,
 *      vSurfaceFuelHeat#, and vSurfaceFuelDens# values.
 *      The intent is to support "custom fuel modeling".
 */

void EqCalc::FuelBedParms( void )
{
    // Give default values to all particles.
    for ( int i = 0;
          i < MaxParts;
          i++ )
    {
        vSurfaceFuelDens[i]->update( 32. );
        vSurfaceFuelLife[i]->updateItem( 0 );
        vSurfaceFuelLoad[i]->update( 0.0 );
        vSurfaceFuelSavr[i]->update( 1. );
        vSurfaceFuelStot[i]->update( 0.0555 );
        vSurfaceFuelSeff[i]->update( 0.0100 );
    }
    // Assign fixed life values
    vSurfaceFuelLife[0]->updateItem( 0 );   // 1-h
    vSurfaceFuelLife[1]->updateItem( 0 );   // 10-h
    vSurfaceFuelLife[2]->updateItem( 0 );   // 100-h
    vSurfaceFuelLife[3]->updateItem( 1 );   // live herb
    vSurfaceFuelLife[4]->updateItem( 2 );   // live wood
    vSurfaceFuelLife[5]->updateItem( 0 );   // dead herb

    // Access current input values
    // Assign load values
    vSurfaceFuelLoad[0]->update( vSurfaceFuelLoadDead1->m_nativeValue );
    vSurfaceFuelLoad[1]->update( vSurfaceFuelLoadDead10->m_nativeValue );
    vSurfaceFuelLoad[2]->update( vSurfaceFuelLoadDead100->m_nativeValue );
    vSurfaceFuelLoad[3]->update( vSurfaceFuelLoadLiveHerb->m_nativeValue );
    vSurfaceFuelLoad[4]->update( vSurfaceFuelLoadLiveWood->m_nativeValue );
    vSurfaceFuelLoad[5]->update( 0.0 );
    // Assign heat values
    vSurfaceFuelHeat[0]->update( vSurfaceFuelHeatDead->m_nativeValue );
    vSurfaceFuelHeat[1]->update( vSurfaceFuelHeatDead->m_nativeValue );
    vSurfaceFuelHeat[2]->update( vSurfaceFuelHeatDead->m_nativeValue );
    vSurfaceFuelHeat[3]->update( vSurfaceFuelHeatLive->m_nativeValue );
    vSurfaceFuelHeat[4]->update( vSurfaceFuelHeatLive->m_nativeValue );
    vSurfaceFuelHeat[5]->update( vSurfaceFuelHeatDead->m_nativeValue );
    // Assign savr values
    vSurfaceFuelSavr[0]->update( vSurfaceFuelSavrDead1->m_nativeValue );
    vSurfaceFuelSavr[1]->update( 109.0 );
    vSurfaceFuelSavr[2]->update( 30.0 );
    vSurfaceFuelSavr[3]->update( vSurfaceFuelSavrLiveHerb->m_nativeValue );
    vSurfaceFuelSavr[4]->update( vSurfaceFuelSavrLiveWood->m_nativeValue );
    vSurfaceFuelSavr[5]->update( vSurfaceFuelSavrLiveHerb->m_nativeValue );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelBedParms() 0 0\n", Margin );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelBedWeighted
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFireReactionInt (Btu/ft2/min)
 *      vSurfaceFireSpreadAtHead (ft/min)
 *      vSurfaceFireSpreadAtVector (ft/min)
 *      vSurfaceFireMaxDirFromUpslope (clockwise from upslope)
 *      vSurfaceFireEffWindAtHead (mi/h)
 *      vSurfaceFireEffWindAtVector (mi/h)
 *      vSurfaceFireWindSpeedLimit (mi/h)
 *      vSurfaceFireWindSpeedFlag (flag)
 *      vSurfaceFireHeatPerUnitArea (Btu/ft2)
 *      vSurfaceFireLineIntAtHead (Btu/ft/s)
 *      vSurfaceFireLineIntAtVector (Btu/ft/s)
 *      vSurfaceFireFlameLengIntAtHead (ft)
 *      vSurfaceFireFlameLengIntAtVector (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelBedModel1 (item)
 *      vSurfaceFuelBedModel2 (item)
 *      vSurfaceFuelBedCoverage1 (fraction)
 *      vSiteSlopeFraction (rise/reach)
 *      vWindDirFromUpslope (degrees)
 *      vWindSpeedAtMidflame (mi/h)
 *      vSurfaceFuelMoisDead1 (fraction)
 *      vSurfaceFuelMoisDead10 (fraction)
 *      vSurfaceFuelMoisDead100 (fraction)
 *      vSurfaceFuelMoisDead1000 (fraction)
 *      vSurfaceFuelMoisLiveHerb (fraction)
 *      vSurfaceFuelMoisLiveWood (fraction)
 *      vSurfaceFuelLoadTransferFraction (fraction)
 *      vSurfaceFireVectorDirFromUpslope (deg)
 */

void EqCalc::FuelBedWeighted( void )
{
    // Get the primary and secondary fuel models
    FuelModel *fm[2];
    fm[0] = currentFuelModel( 1 );
    fm[1] = currentFuelModel( 2 );

    // Get the primary and secondary fuel model coverages
    double cov[2];
    cov[0] = vSurfaceFuelBedCoverage1->m_nativeValue;
    cov[1] = 1. - cov[0];

    // We'll need to get some properties
    PropertyDict *prop = m_eqTree->m_propDict;

    //----------------------------------------
    // Determine individual fuel model outputs
    //----------------------------------------

    // Intermediate outputs for each fuel model
    double rosh[2], rosv[2];	// ros at head and vector
	double flih[2], fliv[2];	// fireline intensity at head and vector
	double flh[2], flv[2];		// flame length at head and vector
	double ewsh[2], ewsv[2];	// effective wind speed at head and vector
	double flw[2];				// fire length-to-width ratio
    double rxi[2], hua[2], mxd[2];	// reaction intensity, heat per unit area, dir of max spread
	double waf[2], wmf[2];		// wind adjustment factor and wind speed at midflame
	double wsl[2];				// wind speed limit
	int    wsf[2];				// wind speed flag

    // Calculate fire outputs for each fuel model
    for ( int i=0; i<2; i++ )
    {
        // Load the fuel attributes into the equation tree's fuel model
        // This replaces the call to FuelBedModel()
        vSurfaceFuelBedDepth->update( fm[i]->m_depth );
        vSurfaceFuelBedMextDead->update( fm[i]->m_mext );
        vSurfaceFuelHeatDead->update( fm[i]->m_heatDead );
        vSurfaceFuelHeatLive->update( fm[i]->m_heatLive );
        vSurfaceFuelLoadDead1->update( fm[i]->m_load1 );
        vSurfaceFuelLoadDead10->update( fm[i]->m_load10 );
        vSurfaceFuelLoadDead100->update( fm[i]->m_load100 );
        vSurfaceFuelLoadLiveHerb->update( fm[i]->m_loadHerb );
        vSurfaceFuelLoadLiveWood->update( fm[i]->m_loadWood );
        vSurfaceFuelSavrDead1->update( fm[i]->m_savr1 );
        vSurfaceFuelSavrLiveHerb->update( fm[i]->m_savrHerb );
        vSurfaceFuelSavrLiveWood->update( fm[i]->m_savrWood );
		vSurfaceFuelLoadTransferEq->updateItem( fm[i]->m_transfer );

        // Load the equation tree's fuel model into fuel parameter arrays
        FuelBedParms();
		FuelLoadTransferFraction();		// vSurfaceFuelLoadTransferFraction

        // Calculate the fuel bed intermediates
        FuelBedIntermediates();

        // Calculate vSurfaceFireResidenceTime
        FireResidenceTime();			// vSurfaceFireResidenceTime

        // Load life class moistures into the equation tree time-lag classes
        if ( prop->boolean( "surfaceConfMoisLifeCat" ) )
        {
            FuelMoisLifeClass();		// vSurfaceFuelMoisLifeDead, vSurfaceFuelMoisLifeLive
        }
        // or load moisture scenario into the equation tree time-lag classes
        else if ( prop->boolean( "surfaceConfMoisScenario" ) )
        {
            FuelMoisScenarioModel();
        }
        // Load equation tree time-lag classes into fuel parameter arrays
        FuelMoisTimeLag();

        // Calculate heat sink outputs
        FuelBedHeatSink();

        // Calculate vSurfaceFirePropagatingFlux
        FirePropagatingFlux();

        // Calculate and store vSurfaceFireReactionInt
        FireReactionInt();
        rxi[i] = vSurfaceFireReactionInt->m_nativeValue;

        // Calculate vSurfaceFireNoWindRate
        FireNoWindRate();

		// If necessary, calculate wind adjustment factor from canopy and fuel parameters
		if ( prop->boolean( "surfaceConfWindSpeedAt10MCalc" )
		  || prop->boolean( "surfaceConfWindSpeedAt20FtCalc" ) )
		{
			WindAdjFactor();
		}
		waf[i] = vWindAdjFactor->m_nativeValue;

		// If necessary, calculate 20-ft wind speed from 10-m wind speed
		if ( prop->boolean( "surfaceConfWindSpeedAt10M" )
		  || prop->boolean( "surfaceConfWindSpeedAt10MCalc" ) )
		{
			WindSpeedAt20Ft();
		}

		// If necessary, calculate midflame wind speed from 20-ft wind speed and wind adj factor
		if ( ! prop->boolean( "surfaceConfWindSpeedAtMidflame" ) )
		{
			WindSpeedAtMidflame();
		}
		wmf[i] = vWindSpeedAtMidflame->m_nativeValue;

        // Calculate and store fire spread outputs
        FireSpreadAtHead();
        rosh[i] = vSurfaceFireSpreadAtHead->m_nativeValue;
        mxd[i]  = vSurfaceFireMaxDirFromUpslope->m_nativeValue;
        ewsh[i] = vSurfaceFireEffWindAtHead->m_nativeValue;
        wsl[i]  = vSurfaceFireWindSpeedLimit->m_nativeValue;
		wsf[i]  = ( wmf[i] > wsl[i] ) ? 1 : 0;

        // Calculate vSurfaceFireLineIntAtHead
        FireLineIntAtHead();
        flih[i] = vSurfaceFireLineIntAtHead->m_nativeValue;

        // Calculate vSurfaceFireFlameLengAtHead
        FireFlameLengAtHead();
        flh[i] = vSurfaceFireFlameLengAtHead->m_nativeValue;

        // Calculate vSurfaceFireLengthToWidth
        FireLengthToWidth();
		flw[i] = vSurfaceFireLengthToWidth->m_nativeValue;

        // Calculate vSurfaceFireEccentricity
        FireEccentricity();

		// If spread direction is upslope, set vector to 0
	    if ( prop->boolean( "surfaceConfSpreadDirMax" ) )
		{
	        vSurfaceFireVectorBeta->update( 0. );
		}
		else
		{
			FireVectorBeta();
		}

        // Calculate vSurfaceFireSpreadAtVector
        FireSpreadAtBeta();
        rosv[i] = vSurfaceFireSpreadAtVector->m_nativeValue;

        // Calculate vSurfaceFireLineIntAtVector
        FireLineIntAtVector();
        fliv[i] = vSurfaceFireLineIntAtVector->m_nativeValue;

        // Calculate vSurfaceFireFlameLengAtVector
        FireFlameLengAtVector();
        flv[i] = vSurfaceFireFlameLengAtVector->m_nativeValue;

        // Calculate vSurfaceFireEffWindAtVector
        FireEffWindAtVector();
        ewsv[i] = vSurfaceFireEffWindAtVector->m_nativeValue;

        // Calculate and store vSurfaceFireHeatPerUnitArea
        FireHeatPerUnitArea();
        hua[i] = vSurfaceFireHeatPerUnitArea->m_nativeValue;
    }

    //------------------------------------------------
    // Determine and store combined fuel model outputs
    //------------------------------------------------

    // Fire spread rate depends upon the weighting method...
    double wtdh = 0.;	// value at head
    double wtdv = 0.;	// value at vector
    double wtd  = 0.;	// anything
    // If area weighted spread rate ...
    if ( prop->boolean( "surfaceConfFuelAreaWeighted" ) )
    {
        wtdh = ( cov[0] * rosh[0] ) + ( cov[1] * rosh[1] ) ;
        wtdv = ( cov[0] * rosv[0] ) + ( cov[1] * rosv[1] ) ;
    }
    // else if harmonic mean spread rate...
    else if ( prop->boolean( "surfaceConfFuelHarmonicMean" ) )
    {
        wtdv = 0.0;
        wtdh = 0.0;
        if ( rosh[0] > 0.000001 && rosh[1] > 0.000001 )
        {
            wtdh = 1. / ( ( cov[0] / rosh[0] ) + ( cov[1] / rosh[1] ) ) ;
            wtdv = 1. / ( ( cov[0] / rosv[0] ) + ( cov[1] / rosv[1] ) ) ;
        }
    }
    // else if Finney's 2-dimensional spread rate...
    else if ( prop->boolean( "surfaceConfFuel2Dimensional" ) )
    {
        double lbRatio = vSurfaceFireLengthToWidth->m_nativeValue;
        int samples  = prop->integer( "surfaceConfFuel2DSamples" );
        int depth    = prop->integer( "surfaceConfFuel2DDepth" );
        int laterals = prop->integer( "surfaceConfFuel2DLaterals" );
        wtdh = FBL_SurfaceFireExpectedSpreadRate( rosh, cov, 2, lbRatio,
                    samples, depth, laterals );
        wtdv = FBL_SurfaceFireExpectedSpreadRate( rosv, cov, 2, lbRatio,
                    samples, depth, laterals );
    }
    vSurfaceFireSpreadAtHead->update( wtdh );
    vSurfaceFireSpreadAtVector->update( wtdv );

    // The following assignments are based on Pat's rules:
	// If only 1 fuel is present (whether primary or secondary), use its values exclusively
	if ( cov[0] > 0.999 || cov[1] > 0.999)
	{
		int i = ( cov[0] > 0.999 ) ? 0 : 1;
		vSurfaceFireReactionInt->update( rxi[i] );
		vSurfaceFireMaxDirFromUpslope->update( mxd[i] );
		vWindAdjFactor->update( waf[i] );
		vWindSpeedAtMidflame->update( wmf[i] );
	    vSurfaceFireEffWindAtHead->update( ewsh[i] );
		vSurfaceFireEffWindAtVector->update( ewsv[i] );
		vSurfaceFireWindSpeedLimit->update( wsl[i] );
		vSurfaceFireWindSpeedFlag->updateItem( wsf[i] );
		vSurfaceFireLengthToWidth->update( flw[i] );
		vSurfaceFireHeatPerUnitArea->update( hua[i] );
		vSurfaceFireLineIntAtHead->update( flih[i] );
		vSurfaceFireLineIntAtVector->update( fliv[i] );
		vSurfaceFireFlameLengAtHead->update( flh[i] );
	    vSurfaceFireFlameLengAtVector->update( flv[i] );
		vSurfaceFuelBedDepth->update( fm[i]-> m_depth );
	}
	// Otherwise the wtd value depends upon Pat's criteria; could be wtd, min, max, or primary
	else
	{
		// Reaction intensity is the maximum of the two models
		wtd = ( rxi[0] > rxi[1] ) ? rxi[0] : rxi[1];
		vSurfaceFireReactionInt->update( wtd );

		// Direction of maximum spread is for the FIRST (not necessarily dominant) fuel model
		vSurfaceFireMaxDirFromUpslope->update( mxd[0] );

	    // Wind adjustment factor is for the FIRST (not necessarily dominant) fuel model
		vWindAdjFactor->update( waf[0] );

	    // Midflame wind speed is for the FIRST (not necessarily dominant) fuel model
		vWindSpeedAtMidflame->update( wmf[0] );

	    // Effective wind speed is for the FIRST (not necessarily dominant) fuel model
		vSurfaceFireEffWindAtHead->update( ewsh[0] );
		vSurfaceFireEffWindAtVector->update( ewsv[0] );

		// Maximum reliable wind speed is the minimum of the two models
		wtd = ( wsl[0] < wsl[1] ) ? wsl[0] : wsl[1];
		vSurfaceFireWindSpeedLimit->update( wtd );

	    // If either wind limit is exceeded, set the flag
		vSurfaceFireWindSpeedFlag->updateItem( wsf[0] || wsf[1] );

		// Fire length-to-width ratio is for the FIRST (not necessarily dominant) fuel model
		vSurfaceFireLengthToWidth->update( flw[0] );

	    // Heat per unit area is the maximum of the two models
		wtd = ( hua[0] > hua[1] ) ? hua[0] : hua[1];
		vSurfaceFireHeatPerUnitArea->update( wtd );

		// Fireline intensity is the maximum of the two models
		wtdh = ( flih[0] > flih[1] ) ? flih[0] : flih[1];
		wtdv = ( fliv[0] > fliv[1] ) ? fliv[0] : fliv[1];
		vSurfaceFireLineIntAtHead->update( wtdh );
		vSurfaceFireLineIntAtVector->update( wtdv );

	    // Flame length is the maximum of the two models
		wtdh = ( flh[0] > flh[1] ) ? flh[0] : flh[1];
	    wtdv = ( flv[0] > flv[1] ) ? flv[0] : flv[1];
		vSurfaceFireFlameLengAtHead->update( wtdh );
	    vSurfaceFireFlameLengAtVector->update( wtdv );

	    // Fuel bed depth is the maximum of the two fuel bed depths
		wtd = ( fm[0]->m_depth > fm[1]-> m_depth ) ? fm[0]->m_depth : fm[1]->m_depth;
		vSurfaceFuelBedDepth->update( wtd );
	}
    //------------
    // Log results
    //------------

    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelBedWeighted() 6 13\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelBedModel1 %d %s\n", Margin,
            vSurfaceFuelBedModel1->activeItemDataIndex(),
            vSurfaceFuelBedModel1->activeItemName().latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedModel2 %d %s\n", Margin,
            vSurfaceFuelBedModel2->activeItemDataIndex(),
            vSurfaceFuelBedModel2->activeItemName().latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedCoverage1 %g %s\n", Margin,
            vSurfaceFuelBedCoverage1->m_nativeValue,
            vSurfaceFuelBedCoverage1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteSlopeFraction %g %s\n", Margin,
            vSiteSlopeFraction->m_nativeValue,
            vSiteSlopeFraction->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromUpslope %g %s\n", Margin,
            vWindDirFromUpslope->m_nativeValue,
            vWindDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );

        fprintf( m_log, "%s  o vSurfaceFireReactionInt %g %s\n", Margin,
            vSurfaceFireReactionInt->m_nativeValue,
            vSurfaceFireReactionInt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireSpreadAtHead %g %s\n", Margin,
            vSurfaceFireSpreadAtHead->m_nativeValue,
            vSurfaceFireSpreadAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireSpreadAtVector %g %s\n", Margin,
            vSurfaceFireSpreadAtVector->m_nativeValue,
            vSurfaceFireSpreadAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireMaxDirFromUpslope %g %s\n", Margin,
            vSurfaceFireMaxDirFromUpslope->m_nativeValue,
            vSurfaceFireMaxDirFromUpslope->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireEffWindAtHead %g %s\n", Margin,
            vSurfaceFireEffWindAtHead->m_nativeValue,
            vSurfaceFireEffWindAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireEffWindAtVector %g %s\n", Margin,
            vSurfaceFireEffWindAtVector->m_nativeValue,
            vSurfaceFireEffWindAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWindSpeedLimit %g %s\n", Margin,
            vSurfaceFireWindSpeedLimit->m_nativeValue,
            vSurfaceFireWindSpeedLimit->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireWindSpeedFlag %g %s\n", Margin,
            vSurfaceFireWindSpeedFlag->m_nativeValue,
            vSurfaceFireWindSpeedFlag->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireHeatPerUnitArea %g %s\n", Margin,
            vSurfaceFireHeatPerUnitArea->m_nativeValue,
            vSurfaceFireHeatPerUnitArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLineIntAtHead %g %s\n", Margin,
            vSurfaceFireLineIntAtHead->m_nativeValue,
            vSurfaceFireLineIntAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireLineIntAtVector %g %s\n", Margin,
            vSurfaceFireLineIntAtVector->m_nativeValue,
            vSurfaceFireLineIntAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireFlameLengAtHead %g %s\n", Margin,
            vSurfaceFireFlameLengAtHead->m_nativeValue,
            vSurfaceFireFlameLengAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelLoadTransferFraction
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelLoadTransferFraction (fraction)
 *
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelLoadTransferEq (index)
 *      vSurfaceFuelMoisLiveHerb (fraction)
 *
 *  Notes
 *      Implements NFDRS live herbaceous fuel load transfer fraction.
 */

void EqCalc::FuelLoadTransferFraction( void )
{
    // Access current input values
    double herbMois = vSurfaceFuelMoisLiveHerb->m_nativeValue;
    int transferEq  = vSurfaceFuelLoadTransferEq->activeItemDataIndex();

    // Derive herbaceous load transfer fraction
    double fraction = 0.0;
    if ( transferEq != 0 )
    {
        fraction = FBL_HerbaceousFuelLoadCuredFraction( herbMois );
    }

    // Store results
    vSurfaceFuelLoadTransferFraction->update( fraction );
    vSurfaceFuelLoadTransferFraction->m_store.setNum(
        vSurfaceFuelLoadTransferFraction->m_displayValue,
        'f', vSurfaceFuelLoadTransferFraction->m_displayDecimals );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelLoadTransferFraction() 2 6\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelMoisLiveHerb %g %s\n", Margin,
            vSurfaceFuelMoisLiveHerb->m_nativeValue,
            vSurfaceFuelMoisLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelLoadTransferFraction %g %s\n", Margin,
            vSurfaceFuelLoadTransferFraction->m_nativeValue,
            vSurfaceFuelLoadTransferFraction->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelMoisLifeClass
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelMoisDead1 (fraction)
 *      vSurfaceFuelMoisDead10 (fraction)
 *      vSurfaceFuelMoisDead100 (fraction)
 *      vSurfaceFuelMoisDead1000 (fraction)
 *      vSurfaceFuelMoisLiveHerb (fraction)
 *      vSurfaceFuelMoisLiveWood (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelMoisLifeDead (fraction)
 *      vSurfaceFuelMoisLifeLive (fraction)
 *
 *  Notes
 *      Provides a means of assigning fuel moisture to size classes
 *      given generic fuel moisture by life category.
 */

void EqCalc::FuelMoisLifeClass( void )
{
    // Access current input values
    double dead = vSurfaceFuelMoisLifeDead->m_nativeValue;
    double live = vSurfaceFuelMoisLifeLive->m_nativeValue;

    vSurfaceFuelMoisDead1->update( dead );
    vSurfaceFuelMoisDead1->m_store.setNum( vSurfaceFuelMoisDead1->m_displayValue,
        'f', vSurfaceFuelMoisDead1->m_displayDecimals );

    vSurfaceFuelMoisDead10->update( dead );
    vSurfaceFuelMoisDead10->m_store.setNum( vSurfaceFuelMoisDead10->m_displayValue,
        'f', vSurfaceFuelMoisDead10->m_displayDecimals );

    vSurfaceFuelMoisDead100->update( dead );
    vSurfaceFuelMoisDead100->m_store.setNum( vSurfaceFuelMoisDead100->m_displayValue,
        'f', vSurfaceFuelMoisDead100->m_displayDecimals );

    vSurfaceFuelMoisDead1000->update( dead );
    vSurfaceFuelMoisDead1000->m_store.setNum( vSurfaceFuelMoisDead1000->m_displayValue,
        'f', vSurfaceFuelMoisDead1000->m_displayDecimals );

    vSurfaceFuelMoisLiveHerb->update( live );
    vSurfaceFuelMoisLiveHerb->m_store.setNum( vSurfaceFuelMoisLiveHerb->m_displayValue,
        'f', vSurfaceFuelMoisLiveHerb->m_displayDecimals );

    vSurfaceFuelMoisLiveWood->update( live );
    vSurfaceFuelMoisLiveWood->m_store.setNum( vSurfaceFuelMoisLiveWood->m_displayValue,
        'f', vSurfaceFuelMoisLiveWood->m_displayDecimals );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelMoisLifeClass() 2 6\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelMoisLifeDead %g %s\n", Margin,
            vSurfaceFuelMoisLifeDead->m_nativeValue,
            vSurfaceFuelMoisLifeDead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisLifeLive %g %s\n", Margin,
            vSurfaceFuelMoisLifeLive->m_nativeValue,
            vSurfaceFuelMoisLifeLive->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead1 %g %s\n", Margin,
            vSurfaceFuelMoisDead1->m_nativeValue,
            vSurfaceFuelMoisDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead10 %g %s\n", Margin,
            vSurfaceFuelMoisDead10->m_nativeValue,
            vSurfaceFuelMoisDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead100 %g %s\n", Margin,
            vSurfaceFuelMoisDead100->m_nativeValue,
            vSurfaceFuelMoisDead100->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead1000 %g %s\n", Margin,
            vSurfaceFuelMoisDead1000->m_nativeValue,
            vSurfaceFuelMoisDead1000->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisLiveHerb %g %s\n", Margin,
            vSurfaceFuelMoisLiveHerb->m_nativeValue,
            vSurfaceFuelMoisLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisLiveWood %g %s\n", Margin,
            vSurfaceFuelMoisLiveWood->m_nativeValue,
            vSurfaceFuelMoisLiveWood->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelMoisScenarioModel
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelMoisDead1 (fraction)
 *      vSurfaceFuelMoisDead10 (fraction)
 *      vSurfaceFuelMoisDead100 (fraction)
 *      vSurfaceFuelMoisDead1000 (fraction)
 *      vSurfaceFuelMoisLiveHerb (fraction)
 *      vSurfaceFuelMoisLiveWood (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelMoisScenario (item)
 *
 *  Notes
 *      Provides a means of assigning fuel moisture to size classes
 *      given a fuel mositure scenario.
 */

void EqCalc::FuelMoisScenarioModel( void )
{
    // Access current input values
    // Get current moisture scenario item name
    QString name = vSurfaceFuelMoisScenario->activeItemName();

    // Get the MoisScenario class pointer for the item name
    MoisScenario *ms =
        m_eqTree->m_moisScenarioList->moisScenarioByScenarioName( name );
    if ( ! ms )
    // This code block should never be executed!
    {
        QString text("");
        translate( text, "EqCalc:MoisScenarioNotFound",
            vSurfaceFuelMoisScenario->activeItemName() );
        bomb( text );
    }
    // Copy values from the MoisScenario into the EqTree
    vSurfaceFuelMoisDead1->update( ms->m_moisDead1 );
    vSurfaceFuelMoisDead1->m_store.setNum( vSurfaceFuelMoisDead1->m_displayValue,
        'f', vSurfaceFuelMoisDead1->m_displayDecimals );

    vSurfaceFuelMoisDead10->update( ms->m_moisDead10 );
    vSurfaceFuelMoisDead10->m_store.setNum( vSurfaceFuelMoisDead10->m_displayValue,
        'f', vSurfaceFuelMoisDead10->m_displayDecimals );

    vSurfaceFuelMoisDead100->update( ms->m_moisDead100 );
    vSurfaceFuelMoisDead100->m_store.setNum( vSurfaceFuelMoisDead100->m_displayValue,
        'f', vSurfaceFuelMoisDead100->m_displayDecimals );

    vSurfaceFuelMoisDead1000->update( ms->m_moisDead1000 );
    vSurfaceFuelMoisDead1000->m_store.setNum( vSurfaceFuelMoisDead1000->m_displayValue,
        'f', vSurfaceFuelMoisDead1000->m_displayDecimals );

    vSurfaceFuelMoisLiveHerb->update( ms->m_moisLiveHerb );
    vSurfaceFuelMoisLiveHerb->m_store.setNum( vSurfaceFuelMoisLiveHerb->m_displayValue,
        'f', vSurfaceFuelMoisLiveHerb->m_displayDecimals );

    vSurfaceFuelMoisLiveWood->update( ms->m_moisLiveWood );
    vSurfaceFuelMoisLiveWood->m_store.setNum( vSurfaceFuelMoisLiveWood->m_displayValue,
        'f', vSurfaceFuelMoisLiveWood->m_displayDecimals );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelMoisScenarioModel1 6\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelMoisScenario %s\n", Margin,
            name.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead1 %g %s\n", Margin,
            vSurfaceFuelMoisDead1->m_nativeValue,
            vSurfaceFuelMoisDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead10 %g %s\n", Margin,
            vSurfaceFuelMoisDead10->m_nativeValue,
            vSurfaceFuelMoisDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead100 %g %s\n", Margin,
            vSurfaceFuelMoisDead100->m_nativeValue,
            vSurfaceFuelMoisDead100->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisDead1000 %g %s\n", Margin,
            vSurfaceFuelMoisDead1000->m_nativeValue,
            vSurfaceFuelMoisDead1000->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisLiveHerb %g %s\n", Margin,
            vSurfaceFuelMoisLiveHerb->m_nativeValue,
            vSurfaceFuelMoisLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelMoisLiveWood %g %s\n", Margin,
            vSurfaceFuelMoisLiveWood->m_nativeValue,
            vSurfaceFuelMoisLiveWood->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelMoisTimeLag
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelMois# (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelMoisDead1 (fraction)
 *      vSurfaceFuelMoisDead10 (fraction)
 *      vSurfaceFuelMoisDead100 (fraction)
 *      vSurfaceFuelMoisDead1000 (fraction)
 *      vSurfaceFuelMoisLiveHerb (fraction)
 *      vSurfaceFuelMoisLiveWood (fraction)
 *      vSurfaceFuelLife# (category)
 *      vSurfaceFuelSavr# (ft2/ft3)
 *
 *  Notes
 *      Provides a means of assigning fuel moistures to fuel particles
 *      given fuel moistures by size class.
 */

void EqCalc::FuelMoisTimeLag( void )
{
    // Access current input values
    int    life, i;
    double savr, mois;

    for ( i = 0;
          i < MaxParts;
          i++ )
    {
        life = vSurfaceFuelLife[i]->activeItemDataIndex();
        savr = vSurfaceFuelSavr[i]->m_nativeValue;
        // Live herbaceous fuel types (1)
        if ( life == FuelLifeType_LiveHerb )
        {
            mois = vSurfaceFuelMoisLiveHerb->m_nativeValue;
        }
        // Live woody fuel types (2)
        else if ( life == FuelLifeType_LiveWood )
        {
            mois = vSurfaceFuelMoisLiveWood->m_nativeValue;
        }
        // Dead litter fuel types (3)
        else if ( life == FuelLifeType_DeadLitter )
        {
            mois = vSurfaceFuelMoisDead100->m_nativeValue;
        }
        // Dead time lag fuel types (0)
        else if ( savr > 192. )
        {
            mois = vSurfaceFuelMoisDead1->m_nativeValue;
        }
        else if ( savr > 48. )
        {
            mois = vSurfaceFuelMoisDead10->m_nativeValue;
        }
        else if ( savr > 16. )
        {
            mois = vSurfaceFuelMoisDead100->m_nativeValue;
        }
        else
        {
            mois = vSurfaceFuelMoisDead1000->m_nativeValue;
        }
        vSurfaceFuelMois[i]->update( mois );
    }
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelMoisTimeLag() %d %d\n", Margin,
            6+MaxParts, MaxParts );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead1 %g %s\n", Margin,
            vSurfaceFuelMoisDead1->m_nativeValue,
            vSurfaceFuelMoisDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead10 %g %s\n", Margin,
            vSurfaceFuelMoisDead10->m_nativeValue,
            vSurfaceFuelMoisDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead100 %g %s\n", Margin,
            vSurfaceFuelMoisDead100->m_nativeValue,
            vSurfaceFuelMoisDead100->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead1000 %g %s\n", Margin,
            vSurfaceFuelMoisDead1000->m_nativeValue,
            vSurfaceFuelMoisDead1000->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisLiveHerb %g %s\n", Margin,
            vSurfaceFuelMoisLiveHerb->m_nativeValue,
            vSurfaceFuelMoisLiveHerb->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelMoisLiveWood %g %s\n", Margin,
            vSurfaceFuelMoisLiveWood->m_nativeValue,
            vSurfaceFuelMoisLiveWood->m_nativeUnits.latin1() );
        for ( i = 0;
              i < MaxParts;
              i++ )
        {
            fprintf( m_log, "%s  i vSurfaceFuelLife%d %g %s\n", Margin, i,
                vSurfaceFuelLife[i]->m_nativeValue,
                vSurfaceFuelLife[i]->m_nativeUnits.latin1() );
            fprintf( m_log, "%s  i vSurfaceFuelSavr%d %g %s\n", Margin, i,
                vSurfaceFuelSavr[i]->m_nativeValue,
                vSurfaceFuelSavr[i]->m_nativeUnits.latin1() );
        }
        for ( i = 0;
              i < MaxParts;
              i++ )
        {
            fprintf( m_log, "%s  o vSurfaceFuelMois%d %g %s\n", Margin, i,
                vSurfaceFuelMois[i]->m_nativeValue,
                vSurfaceFuelMois[i]->m_nativeUnits.latin1() );
        }
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelPalmettoModel
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelBedDepth (ft)
 *      vSurfaceFuelBedMextDead (lb/lb)
 *      vSurfaceFuelPalmettoLoadDead1 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadDead10 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadDeadFoliage (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLitter (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLive1 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLive10 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLiveFoliage (lb/ft2)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelPalmettoAge (yr)
 *      vSurfaceFuelPalmettoCover (%)
 *      vSurfaceFuelPalmettoHeight (ft)
 *      vSurfaceFuelPalmettoOverstoryBasalArea (ft2/ac)
 */

void EqCalc::FuelPalmettoModel( void )
{
    // Access current input values
    double age   = vSurfaceFuelPalmettoAge->m_nativeValue;
    double cover = vSurfaceFuelPalmettoCover->m_nativeValue;
    double ht    = vSurfaceFuelPalmettoHeight->m_nativeValue;
    double ba    = vSurfaceFuelPalmettoOverstoryBasalArea->m_nativeValue;
    // Calculate
    double dead1  = FBL_PalmettoGallberyDead1HrLoad( age, ht );
    double dead10 = FBL_PalmettoGallberyDead10HrLoad( age, cover );
    double deadf  = FBL_PalmettoGallberyDeadFoliageLoad( age, cover );
    double depth  = FBL_PalmettoGallberyFuelBedDepth( ht );
    double live1  = FBL_PalmettoGallberyLive1HrLoad( age, ht );
    double live10 = FBL_PalmettoGallberyLive10HrLoad( age, ht );
    double livef  = FBL_PalmettoGallberyLiveFoliageLoad( age, cover, ht );
    double litter = FBL_PalmettoGallberyLitterLoad( age, ba ) ;
    // Update
    vSurfaceFuelBedDepth->update( depth );
    vSurfaceFuelBedMextDead->update( 0.40 );
    vSurfaceFuelPalmettoLoadDead1->update( dead1 );
    vSurfaceFuelPalmettoLoadDead10->update( dead10 );
    vSurfaceFuelPalmettoLoadDeadFoliage->update( deadf );
    vSurfaceFuelPalmettoLoadLitter->update( litter );
    vSurfaceFuelPalmettoLoadLive1->update( live1 );
    vSurfaceFuelPalmettoLoadLive10->update( live10 );
    vSurfaceFuelPalmettoLoadLiveFoliage->update( livef );

    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelPalmettoModel() 4 8\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelPalmettoAge %g %s\n", Margin,
            vSurfaceFuelPalmettoAge->m_nativeValue,
            vSurfaceFuelPalmettoAge->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelPalmettoCover %g %s\n", Margin,
            vSurfaceFuelPalmettoCover->m_nativeValue,
            vSurfaceFuelPalmettoCover->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelPalmettoHeight %g %s\n", Margin,
            vSurfaceFuelPalmettoHeight->m_nativeValue,
            vSurfaceFuelPalmettoHeight->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelPalmettoOverstoryBasalArea %g %s\n", Margin,
            vSurfaceFuelPalmettoOverstoryBasalArea->m_nativeValue,
            vSurfaceFuelPalmettoOverstoryBasalArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadDead1 %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadDead1->m_nativeValue,
            vSurfaceFuelPalmettoLoadDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadDead10 %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadDead10->m_nativeValue,
            vSurfaceFuelPalmettoLoadDead10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadDeadFoliage %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadDeadFoliage->m_nativeValue,
            vSurfaceFuelPalmettoLoadDeadFoliage->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadLitter %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadLitter->m_nativeValue,
            vSurfaceFuelPalmettoLoadLitter->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadLive1 %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadLive1->m_nativeValue,
            vSurfaceFuelPalmettoLoadLive1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadLive10 %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadLive10->m_nativeValue,
            vSurfaceFuelPalmettoLoadLive10->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelPalmettoLoadLiveFoliage %g %s\n", Margin,
            vSurfaceFuelPalmettoLoadLiveFoliage->m_nativeValue,
            vSurfaceFuelPalmettoLoadLiveFoliage->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelBedDepth %g %s\n", Margin,
            vSurfaceFuelBedDepth->m_nativeValue,
            vSurfaceFuelBedDepth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelPalmettoParms
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelDens# (lb/ft3)
 *      vSurfaceFuelHeat# (Btu/lb)
 *      vSurfaceFuelLife# (category)
 *      vSurfaceFuelLoad# (lb/ft2)
 *      vSurfaceFuelSavr# (ft2/ft3)
 *      vSurfaceFuelSeff# (fraction)
 *      vSurfaceFuelStot# (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelPalmettoDepth (ft)
 *      vSurfaceFuelPalmettoLoadDead1 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadDead10 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadDeadFoliage (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLitter (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLive1 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLive10 (lb/ft2)
 *      vSurfaceFuelPalmettoLoadLiveFoliage (lb/ft2)
 */

void EqCalc::FuelPalmettoParms( void )
{
    // Assign life values
    vSurfaceFuelLife[0]->updateItem( FuelLifeType_DeadTimeLag ); // Dead 0.0 - 0.25"
    vSurfaceFuelLife[1]->updateItem( FuelLifeType_DeadTimeLag ); // Dead 0.25 - 1.0"
    vSurfaceFuelLife[2]->updateItem( FuelLifeType_DeadTimeLag ); // Dead foliage
    vSurfaceFuelLife[3]->updateItem( FuelLifeType_LiveWood );    // Live 0.0 - 0.25"
    vSurfaceFuelLife[4]->updateItem( FuelLifeType_LiveWood );    // Live 0.25 - 1.0"
    vSurfaceFuelLife[5]->updateItem( FuelLifeType_LiveHerb );    // Live foliage
    vSurfaceFuelLife[6]->updateItem( FuelLifeType_DeadLitter );  // Litter
    vSurfaceFuelLife[7]->updateItem( FuelLifeType_DeadTimeLag ); // Unused
    // Assign load values
    vSurfaceFuelLoad[0]->update( vSurfaceFuelPalmettoLoadDead1->m_nativeValue );
    vSurfaceFuelLoad[1]->update( vSurfaceFuelPalmettoLoadDead10->m_nativeValue );
    vSurfaceFuelLoad[2]->update( vSurfaceFuelPalmettoLoadDeadFoliage->m_nativeValue );
    vSurfaceFuelLoad[3]->update( vSurfaceFuelPalmettoLoadLive1->m_nativeValue );
    vSurfaceFuelLoad[4]->update( vSurfaceFuelPalmettoLoadLive10->m_nativeValue );
    vSurfaceFuelLoad[5]->update( vSurfaceFuelPalmettoLoadLiveFoliage->m_nativeValue );
    vSurfaceFuelLoad[6]->update( vSurfaceFuelPalmettoLoadLitter->m_nativeValue );
    vSurfaceFuelLoad[7]->updateItem( 0 );
    // Assign savr values
    vSurfaceFuelSavr[0]->update( 350.0 );
    vSurfaceFuelSavr[1]->update( 140.0 );
    vSurfaceFuelSavr[2]->update( 2000.0 );
    vSurfaceFuelSavr[3]->update( 350.0 );
    vSurfaceFuelSavr[4]->update( 140.0 );
    vSurfaceFuelSavr[5]->update( 2000.0 );
    // \todo - find appropriate savr for palmetto-gallberry litter
    vSurfaceFuelSavr[6]->update( 2000.0 );
    vSurfaceFuelSavr[7]->update( 1.0 );
    // Assign dens values
    vSurfaceFuelDens[0]->update( 30.0 );
    vSurfaceFuelDens[1]->update( 30.0 );
    vSurfaceFuelDens[2]->update( 30.0 );
    vSurfaceFuelDens[3]->update( 46.0 );
    vSurfaceFuelDens[4]->update( 46.0 );
    vSurfaceFuelDens[5]->update( 46.0 );
    vSurfaceFuelDens[6]->update( 30.0 );
    vSurfaceFuelDens[7]->update( 32.0 );
    // Assign heat and stot values
    for ( int i = 0;
          i < MaxParts;
          i++ )
    {
        vSurfaceFuelHeat[i]->update( 8300. );
        vSurfaceFuelStot[i]->update( 0.030 );
    }
    // Assign seff values
    vSurfaceFuelSeff[0]->update( 0.010 );
    vSurfaceFuelSeff[1]->update( 0.010 );
    vSurfaceFuelSeff[2]->update( 0.010 );
    vSurfaceFuelSeff[3]->update( 0.015 );
    vSurfaceFuelSeff[4]->update( 0.015 );
    vSurfaceFuelSeff[5]->update( 0.015 );
    vSurfaceFuelSeff[6]->update( 0.010 );
    vSurfaceFuelSeff[7]->update( 0.010 );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelPalmettoParms() 0 0\n", Margin );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief FuelTemp
 *
 *  Dependent Variables (Outputs)
 *      vSurfaceFuelTemp (oF)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vSiteSunShading (fraction)
 */

void EqCalc::FuelTemp( void )
{
    // Access current input values
    double airTemp  = vWthrAirTemp->m_nativeValue;
    double sunShade = vSiteSunShading->m_nativeValue;
    // Calculate results
    double fuelTemp = FBL_SurfaceFuelTemperature( airTemp, sunShade );
    // Store results
    vSurfaceFuelTemp->update( fuelTemp );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc FuelTemp() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteSunShading %g %s\n", Margin,
            vSiteSunShading->m_nativeValue,
            vSiteSunShading->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSurfaceFuelTemp %g %s\n", Margin,
            vSurfaceFuelTemp->m_nativeValue,
            vSurfaceFuelTemp->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief IgnitionFirebrandFuelMoisFromDead1Hr
 *
 *  Dependent Variables (Outputs)
 *      vIgnitionFirebrandFuelMois (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelMoisDead1 (fraction)
 */

void EqCalc::IgnitionFirebrandFuelMoisFromDead1Hr( void )
{
    // Access current input values
    double fm = vSurfaceFuelMoisDead1->m_nativeValue;
    // Store results
    vIgnitionFirebrandFuelMois->update( fm );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc IgnitionFirebrandFuelMoisFromDead1Hr() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead1 %g %s\n", Margin,
            vSurfaceFuelMoisDead1->m_nativeValue,
            vSurfaceFuelMoisDead1->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vIgnitionFirebrandFuelMois %g %s\n", Margin,
            vIgnitionFirebrandFuelMois->m_nativeValue,
            vIgnitionFirebrandFuelMois->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief IgnitionFirebrandProb
 *
 *  Dependent Variables (Outputs)
 *      vIgnitionFirebrandProb (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelTemp (oF)
 *      vIgnitionFirebrandFuelMois (fraction)
 */

void EqCalc::IgnitionFirebrandProb( void )
{
    // Access current input values
    double fuelTemp = vSurfaceFuelTemp->m_nativeValue;
    double deadMois = vIgnitionFirebrandFuelMois->m_nativeValue;
    double prob     = FBL_SurfaceFireFirebrandIgnitionProbability(
                        fuelTemp, deadMois );
    // Store results
    vIgnitionFirebrandProb->update( prob );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc IgnitionFirebrandProb() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelTemp %g %s\n", Margin,
            vSurfaceFuelTemp->m_nativeValue,
            vSurfaceFuelTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vIgnitionFirebrandFuelMois %g %s\n", Margin,
            vIgnitionFirebrandFuelMois->m_nativeValue,
            vIgnitionFirebrandFuelMois->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vIgnitionFirebrandProb %g %s\n", Margin,
            vIgnitionFirebrandProb->m_nativeValue,
            vIgnitionFirebrandProb->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief IgnitionLightningFuelMoisFromDead100Hr
 *
 *  Dependent Variables (Outputs)
 *      vIgnitionLightningFuelMois (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFuelMoisDead100 (fraction)
 */

void EqCalc::IgnitionLightningFuelMoisFromDead100Hr( void )
{
    // Access current input values
    double fm = vSurfaceFuelMoisDead100->m_nativeValue;
    // Store results
    vIgnitionLightningFuelMois->update( fm );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc IgnitionLightningFuelMoisFromDead100Hr() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFuelMoisDead100 %g %s\n", Margin,
            vSurfaceFuelMoisDead100->m_nativeValue,
            vSurfaceFuelMoisDead100->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vIgnitionLightningFuelMois %g %s\n", Margin,
            vIgnitionLightningFuelMois->m_nativeValue,
            vIgnitionLightningFuelMois->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief IgnitionLightningProb
 *
 *  Dependent Variables (Outputs)
 *      vIgnitionLightningProb (fraction)
 *
 *  Independent Variables (Inputs)
 *      vIgnitionLightningFuelType (discrete)
 *      vIgnitionLightningDuffDepth (in)
 *      vIgnitionLightningFuelMois (fraction)
 *      vWthrLightningStrikeType (discrete)
 */

void EqCalc::IgnitionLightningProb( void )
{
    // Access current input values
    int    bed    = vIgnitionLightningFuelType->activeItemDataIndex();
    double depth  = vIgnitionLightningDuffDepth->m_nativeValue;
    double mois   = vIgnitionLightningFuelMois->m_nativeValue;
    int    charge = vWthrLightningStrikeType->activeItemDataIndex();
    // Calculate results
    double prob = FBL_SurfaceFireLightningIgnitionProbability(
                    bed, depth, mois, charge );
    // Store results
    vIgnitionLightningProb->update( prob );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc IgnitionLightningProb() 4 1\n", Margin );
        fprintf( m_log, "%s  i vIgnitionLightningFuelType %d %s\n", Margin,
            vIgnitionLightningFuelType->activeItemDataIndex(),
            vIgnitionLightningFuelType->activeItemName().latin1() );
        fprintf( m_log, "%s  i vIgnitionLightningDuffDepth %g %s\n", Margin,
            vIgnitionLightningDuffDepth->m_nativeValue,
            vIgnitionLightningDuffDepth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vIgnitionLightningFuelMois %g %s\n", Margin,
            vIgnitionLightningFuelMois->m_nativeValue,
            vIgnitionLightningFuelMois->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWthrLightningStrikeType %d %s\n", Margin,
            vWthrLightningStrikeType->activeItemDataIndex(),
            vWthrLightningStrikeType->activeItemName().latin1() );
        fprintf( m_log, "%s  o vIgnitionLightningProb %g %s\n", Margin,
            vIgnitionLightningProb->m_nativeValue,
            vIgnitionLightningProb->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief MapScale
 *
 *  Dependent Variables (Outputs)
 *      vMapScale (in/mi)
 *
 *  Independent Variables (Inputs)
 *      vMapFraction (none)
 */

void EqCalc::MapScale( void )
{
    // Access current input values
    double denom = vMapFraction->m_nativeValue;
    // Calculate results
    double scale = ( denom < SMIDGEN )
                 ? ( 0.0 )
                 : ( 63360. / denom );
    // Store results
    vMapScale->update( scale );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc MapScale() 2 1\n", Margin );
        fprintf( m_log, "%s  i vMapFraction %g %s\n", Margin,
            vMapFraction->m_nativeValue,
            vMapFraction->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief MapSlope
 *
 *  Dependent Variables (Outputs)
 *      vSiteSlopeDegrees (degrees)
 *      vMapSlopeRise (ft)
 *      vMapSlopeReach (ft)
 *
 *  Independent Variables (Inputs)
 *      vMapFraction (1:x)
 *      vMapContourCount (none)
 *      vMapContourInterval (ft)
 *      vMapDist (in)
 */

void EqCalc::MapSlope( void )
{
    // Access current input values
    double cIntervalFt = vMapContourInterval->m_nativeValue;
    double cCount      = vMapContourCount->m_nativeValue;
    double mapFraction = vMapFraction->m_nativeValue;
    double mapDistIn   = vMapDist->m_nativeValue;
    // Calculate results
    double riseFt      = cIntervalFt * cCount;
    double reachFt     = mapFraction * mapDistIn / 12.;
    double degrees     = ( reachFt < SMIDGEN )
                       ? ( 0.0 )
                       : ( ( 180. / M_PI ) * atan( riseFt / reachFt ) );
    // Store results
    vSiteSlopeDegrees->update( degrees );
    vSiteSlopeRise->update( riseFt );
    vSiteSlopeReach->update( reachFt );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc MapSlope() 4 3\n", Margin );
        fprintf( m_log, "%s  i vMapFraction %g %s\n", Margin,
            vMapFraction->m_nativeValue,
            vMapFraction->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapDist %g %s\n", Margin,
            vMapDist->m_nativeValue,
            vMapDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapContourCount %g %s\n", Margin,
            vMapContourCount->m_nativeValue,
            vMapContourCount->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapContourInterval %g %s\n", Margin,
            vMapContourInterval->m_nativeValue,
            vMapContourInterval->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSiteSlopeDegrees %g %s\n", Margin,
            vSiteSlopeDegrees->m_nativeValue,
            vSiteSlopeDegrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSiteSlopeRise %g %s\n", Margin,
            vSiteSlopeRise->m_nativeValue,
            vSiteSlopeRise->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSiteSlopeReach %g %s\n", Margin,
            vSiteSlopeReach->m_nativeValue,
            vSiteSlopeReach->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SafetyZoneRadius
 *
 *  Dependent Variables (Outputs)
 *      vSafteyZoneRadius (ft)
 *      vSafetyZoneSize (ac)
 *
 *  Independent Variables (Inputs)
 *      vSafteyZoneSepDist (ft)
 *      vSafetyZoneEquipmentArea (ft2)
 *      vSafetyZoneEquipmentNumber
 *      vSafetyZonePersonnelArea (ft2)
 *      vSafetyZonePersonnelNumber
 */

void EqCalc::SafetyZoneRadius( void )
{
    // Access current input values
    double eArea  = vSafetyZoneEquipmentArea->m_nativeValue;
    double eNumb  = vSafetyZoneEquipmentNumber->m_nativeValue;
    double pArea  = vSafetyZonePersonnelArea->m_nativeValue;
    double pNumb  = vSafetyZonePersonnelNumber->m_nativeValue;
    double sDist  = vSafetyZoneSepDist->m_nativeValue;
    // Calculate results
    double radius = FBL_SafetyZoneRadius( sDist, pNumb, pArea, eNumb, eArea );
    double size   = M_PI * ( radius * radius );
    // Store results
    vSafetyZoneRadius->update( radius );
    vSafetyZoneSize->update( size );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SafetyZoneAtVector() 5 2\n", Margin );
        fprintf( m_log, "%s  i vSafetyZoneSepDist %g %s\n", Margin,
            vSafetyZoneSepDist->m_nativeValue,
            vSafetyZoneSepDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSafetyZoneEquipmentArea %g %s\n", Margin,
            vSafetyZoneEquipmentArea->m_nativeValue,
            vSafetyZoneEquipmentArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSafetyZoneEquipmentNumber %g %s\n", Margin,
            vSafetyZoneEquipmentNumber->m_nativeValue,
            vSafetyZoneEquipmentNumber->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSafetyZonePersonnelArea %g %s\n", Margin,
            vSafetyZonePersonnelArea->m_nativeValue,
            vSafetyZonePersonnelArea->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSafetyZonePersonnelNumber %g %s\n", Margin,
            vSafetyZonePersonnelNumber->m_nativeValue,
            vSafetyZonePersonnelNumber->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSafetyZoneRadius %g %s\n", Margin,
            vSafetyZoneRadius->m_nativeValue,
            vSafetyZoneRadius->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSafetyZoneSize %g %s\n", Margin,
            vSafetyZoneSize->m_nativeValue,
            vSafetyZoneSize->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SafetyZoneSepDist
 *
 *  Dependent Variables (Outputs)
 *      vSafetyZoneSepDist (ft)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireFlameLengAtHead (ft)
 */

void EqCalc::SafetyZoneSepDist( void )
{
    // Access current input values
    double flameLeng = vSurfaceFireFlameLengAtHead->m_nativeValue;
    // Calculate results
    double sepDist = FBL_SafetyZoneSeparationDistance( flameLeng );
    // Store results
    vSafetyZoneSepDist->update( sepDist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SafetyZoneSepDist() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtHead %g %s\n", Margin,
            vSurfaceFireFlameLengAtHead->m_nativeValue,
            vSurfaceFireFlameLengAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSafetyZoneSepDist %g %s\n", Margin,
            vSafetyZoneSepDist->m_nativeValue,
            vSafetyZoneSepDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SiteAspectDirFromNorth
 *
 *  Dependent Variables (Outputs)
 *      vSiteAspectDirFromNorth (degrees clockwise from North)
 *
 *  Independent Variables (Inputs)
 *      vSiteAspectDirFromCompass (compass point name)
 */

void EqCalc::SiteAspectDirFromNorth( void )
{
    // Access current input values
    int id = vSiteAspectDirFromCompass->activeItemDataIndex();
    // Calculate results
    double deg = 22.5 * (double) id;
    // Store results
    vSiteAspectDirFromNorth->update( deg );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SiteAspectDirFromNorth() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSiteAspectDirFromCompass %d %s\n", Margin,
            vSiteAspectDirFromCompass->activeItemDataIndex(),
            vSiteAspectDirFromCompass->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSiteAspectDirFromNorth %g %s\n", Margin,
            vSiteAspectDirFromNorth->m_nativeValue,
            vSiteAspectDirFromNorth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SiteRidgeToValleyDist
 *
 *  Dependent Variables (Outputs)
 *      vSiteRidgeToValleyDist (mi)
 *
 *  Independent Variables (Inputs)
 *      vSiteRidgeToValleyMapDist (in)
 *      vMapScale (in/mi)
 */

void EqCalc::SiteRidgeToValleyDist( void )
{
    // Access current input values
    double md = vSiteRidgeToValleyMapDist->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double mi = ( ms < SMIDGEN )
              ? ( 0. )
              : ( md / ms );
    // Store results
    vSiteRidgeToValleyDist->update( mi );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SiteRidgeToValleyDist() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSiteRidgeToValleyMapDist %g %s\n", Margin,
            vSiteRidgeToValleyMapDist->m_nativeValue,
            vSiteRidgeToValleyMapDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSiteRidgeToValleyDist %g %s\n", Margin,
            vSiteRidgeToValleyDist->m_nativeValue,
            vSiteRidgeToValleyDist->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SiteSlopeFraction
 *
 *  Dependent Variables (Outputs)
 *      vSiteSlopeFraction (rise/reach)
 *
 *  Independent Variables (Inputs)
 *      vSiteSlopeDegrees (degrees)
 */

void EqCalc::SiteSlopeFraction ( void )
{
    // Access current input values
    double d = vSiteSlopeDegrees->m_nativeValue;
    // Calculate results
    double f = tan( d * M_PI / 180. );
    // Store results
    vSiteSlopeFraction->update( f );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SiteSlopeFraction() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSiteSlopeDegrees %g %s\n", Margin,
            vSiteSlopeDegrees->m_nativeValue,
            vSiteSlopeDegrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSiteSlopeFraction %g %s\n", Margin,
            vSiteSlopeFraction->m_nativeValue,
            vSiteSlopeFraction->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SiteUpslopeDirFromNorth
 *
 *  Dependent Variables (Outputs)
 *      vSiteUpslopeDirFromNorth (azimuth degrees)
 *
 *  Independent Variables (Inputs)
 *      vSiteAspectDirFromNorth (azimuth degrees)
 */

void EqCalc::SiteUpslopeDirFromNorth( void )
{
    // Access current input values
    double aspect  = vSiteAspectDirFromNorth->m_nativeValue;
    // Calculate results
    double upslope = ( aspect >= 180. )
                   ? ( aspect - 180. )
                   : ( aspect + 180. );
    // Store results
    vSiteUpslopeDirFromNorth->update( upslope );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SiteUpslopeDirFromNorth() 1 1\n", Margin );
        fprintf( m_log, "%s  i vSiteAspectDirFromNorth %g %s\n", Margin,
            vSiteAspectDirFromNorth->m_nativeValue,
            vSiteAspectDirFromNorth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSiteUpslopeDirFromNorth %g %s\n", Margin,
            vSiteUpslopeDirFromNorth->m_nativeValue,
            vSiteUpslopeDirFromNorth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SpotDistBurningPile
 *
 *  Dependent Variables (Outputs)
 *      vSpotCoverHtBurningPile (ft)
 *      vSpotDistBurningPile (mi)
 *      vSpotFirebrandHtBurningPile (ft)
 *      vSpotFlatDistBurningPile (mi)
 *
 *  Independent Variables (Inputs)
 *      vSpotFireSource (0-3)
 *      vSiteRidgeToValleyDist (mi)
 *      vSiteRidgeToValleyElev (ft)
 *      vTreeCoverHtDownwind (ft)
 *      vWindSpeedAt20Ft (mi/h)
 *      vSurfaceFireFlameHtPile (ft)
 */

void EqCalc::SpotDistBurningPile( void )
{
    // Access current input values
    int    source  = vSpotFireSource->activeItemDataIndex();
    double coverHt = vTreeCoverHtDownwind->m_nativeValue;
    double flameHt = vSurfaceFireFlameHtPile->m_nativeValue;
    double rvElev  = vSiteRidgeToValleyElev->m_nativeValue;
    double rvHorz  = vSiteRidgeToValleyDist->m_nativeValue;
    double wind    = vWindSpeedAt20Ft->m_nativeValue;
    // Calculate results
    double htUsed, firebrandHt, flatDist;
    double spotDist = FBL_SpotDistanceFromBurningPile(
        source, rvHorz, rvElev, coverHt, wind, flameHt,
        &htUsed, &firebrandHt, &flatDist );
    // Store results
    vSpotCoverHtBurningPile->update( htUsed );
    vSpotDistBurningPile->update( spotDist );
    vSpotFirebrandHtBurningPile->update( firebrandHt );
    vSpotFlatDistBurningPile->update( flatDist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SpotDistBurningPile() 6 4\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireFlameHtPile %g %s\n", Margin,
            vSurfaceFireFlameHtPile->m_nativeValue,
            vSurfaceFireFlameHtPile->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteRidgeToValleyDist %g %s\n", Margin,
            vSiteRidgeToValleyDist->m_nativeValue,
            vSiteRidgeToValleyDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteRidgeToValleyElev %g %s\n", Margin,
            vSiteRidgeToValleyElev->m_nativeValue,
            vSiteRidgeToValleyElev->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHtDownwind %g %s\n", Margin,
            vTreeCoverHtDownwind->m_nativeValue,
            vTreeCoverHtDownwind->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSpotFireSource %d %s\n", Margin,
            vSpotFireSource->activeItemDataIndex(),
            vSpotFireSource->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSpotDistBurningPile %g %s\n", Margin,
            vSpotDistBurningPile->m_nativeValue,
            vSpotDistBurningPile->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotCoverHtBurningPile %g %s\n", Margin,
            vSpotCoverHtBurningPile->m_nativeValue,
            vSpotCoverHtBurningPile->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFirebrandHtBurningPile %g %s\n", Margin,
            vSpotFirebrandHtBurningPile->m_nativeValue,
            vSpotFirebrandHtBurningPile->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFlatDistBurningPile %g %s\n", Margin,
            vSpotFlatDistBurningPile->m_nativeValue,
            vSpotFlatDistBurningPile->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SpotDistSurfaceFire
 *
 *  Dependent Variables (Outputs)
 *      vSpotCoverHtSurfaceFire (ft)
 *      vSpotDistSurfaceFire (mi)
 *      vSpotFirebrandDriftSurfaceFire (mi)
 *      vSpotFirebrandHtSurfaceFire (ft)
 *      vSpotFlatDistSurfaceFire (mi)
 *
 *  Independent Variables (Inputs)
 *      vSpotFireSource (0-3)
 *      vSiteRidgeToValleyDist (mi)
 *      vSiteRidgeToValleyElev (ft)
 *      vTreeCoverHtDownwind (ft)
 *      vWindSpeedAt20Ft (mi/h)
 *      vSurfaceFireFlameLengAtHead (ft)
 */

void EqCalc::SpotDistSurfaceFire( void )
{
    // Access current input values
    double coverHt  = vTreeCoverHtDownwind->m_nativeValue;
    double flameLen = vSurfaceFireFlameLengAtHead->m_nativeValue;
    double rvElev   = vSiteRidgeToValleyElev->m_nativeValue;
    double rvHorz   = vSiteRidgeToValleyDist->m_nativeValue;
    int    source   = vSpotFireSource->activeItemDataIndex();
    double wind     = vWindSpeedAt20Ft->m_nativeValue;
    // Calculate results
    double htUsed, firebrandHt, flatDist, firebrandDrift;
    double spotDist = FBL_SpotDistanceFromSurfaceFire(
        source, rvHorz, rvElev, coverHt, wind, flameLen,
        &htUsed, &firebrandHt, &firebrandDrift, &flatDist );
    // Store results
    vSpotCoverHtSurfaceFire->update( htUsed );
    vSpotDistSurfaceFire->update( spotDist );
    vSpotFirebrandDriftSurfaceFire->update( firebrandDrift );
    vSpotFirebrandHtSurfaceFire->update( firebrandHt );
    vSpotFlatDistSurfaceFire->update( flatDist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SpotDistSurfaceFire() 6 5\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtHead %g %s\n", Margin,
            vSurfaceFireFlameLengAtHead->m_nativeValue,
            vSurfaceFireFlameLengAtHead->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteRidgeToValleyDist %g %s\n", Margin,
            vSiteRidgeToValleyDist->m_nativeValue,
            vSiteRidgeToValleyDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteRidgeToValleyElev %g %s\n", Margin,
            vSiteRidgeToValleyElev->m_nativeValue,
            vSiteRidgeToValleyElev->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHtDownwind %g %s\n", Margin,
            vTreeCoverHtDownwind->m_nativeValue,
            vTreeCoverHtDownwind->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSpotFireSource %d %s\n", Margin,
            vSpotFireSource->activeItemDataIndex(),
            vSpotFireSource->activeItemName().latin1() );
        fprintf( m_log, "%s  o vSpotDistSurfaceFire %g %s\n", Margin,
            vSpotDistSurfaceFire->m_nativeValue,
            vSpotDistSurfaceFire->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotCoverHtSurfaceFire %g %s\n", Margin,
            vSpotCoverHtSurfaceFire->m_nativeValue,
            vSpotCoverHtSurfaceFire->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFirebrandDriftSurfaceFire %g %s\n", Margin,
            vSpotFirebrandDriftSurfaceFire->m_nativeValue,
            vSpotFirebrandDriftSurfaceFire->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFirebrandHtSurfaceFire %g %s\n", Margin,
            vSpotFirebrandHtSurfaceFire->m_nativeValue,
            vSpotFirebrandHtSurfaceFire->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFlatDistSurfaceFire %g %s\n", Margin,
            vSpotFlatDistSurfaceFire->m_nativeValue,
            vSpotFlatDistSurfaceFire->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SpotDistTorchingTrees
 *
 *  Dependent Variables (Outputs)
 *      vSpotCoverHtTorchingTrees (ft)
 *      vSpotDistTorchingTrees (mi)
 *      vSpotFirebrandHtTorchingTrees (ft)
 *      vSpotFlameDurTorchingTrees (ratio)
 *      vSpotFlameHtTorchingTrees (ft)
 *      vSpotFlameRatioTorchingTrees (ratio)
 *      vSpotFlatDistTorchingTrees (mi)
 *
 *  Independent Variables (Inputs)
 *      vSpotFireSource (0-3)
 *      vSiteRidgeToValleyDist (mi)
 *      vSiteRidgeToValleyElev (ft)
 *      vTreeCoverHtDownwind (ft)
 *      vWindSpeedAt20Ft (mi/h)
 *      vSpotTorchingTrees (none)
 *      vTreeDbh (in)
 *      vTreeHt (ft)
 *      vTreeSpeciesSpot (category)
 */

void EqCalc::SpotDistTorchingTrees( void )
{
    // Access current input values
    double coverHt  = vTreeCoverHtDownwind->m_nativeValue;
    double rvElev   = vSiteRidgeToValleyElev->m_nativeValue;
    double rvHorz   = vSiteRidgeToValleyDist->m_nativeValue;
    int    source   = vSpotFireSource->activeItemDataIndex();
    double wind     = vWindSpeedAt20Ft->m_nativeValue;
    double tnum     = vSpotTorchingTrees->m_nativeValue;
    double dbh      = vTreeDbh->m_nativeValue;
    double treeHt   = vTreeHt->m_nativeValue;
    int    spp      = vTreeSpeciesSpot->activeItemDataIndex();
    // Catch species errors
    if ( spp < 0 || spp >= 14 )
    // This code block should never be executed!
    {
        QString text("");
        translate( text, "EqCalc:TreeSpeciesNotFound",
            QString( "%1" ).arg( spp ) );
        bomb( text );
    }
    // Calculate results
    double htUsed, firebrandHt, flatDist, flameHt, flameDur, flameRatio;
    double spotDist = FBL_SpotDistanceFromTorchingTrees(
        source, rvHorz, rvElev, coverHt, wind, tnum, dbh, treeHt, spp,
        &htUsed, &flameHt, &flameRatio, &flameDur, &firebrandHt, &flatDist );
    // Store results
    vSpotCoverHtTorchingTrees->update( htUsed );
    vSpotDistTorchingTrees->update( spotDist );
    vSpotFirebrandHtTorchingTrees->update( firebrandHt );
    vSpotFlameDurTorchingTrees->update( flameDur );
    vSpotFlameHtTorchingTrees->update( flameHt );
    vSpotFlameRatioTorchingTrees->update( flameRatio );
    vSpotFlatDistTorchingTrees->update( flatDist );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SpotDistTorchingTrees() 9 7\n", Margin );
        fprintf( m_log, "%s  i vSiteRidgeToValleyDist %g %s\n", Margin,
            vSiteRidgeToValleyDist->m_nativeValue,
            vSiteRidgeToValleyDist->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteRidgeToValleyElev %g %s\n", Margin,
            vSiteRidgeToValleyElev->m_nativeValue,
            vSiteRidgeToValleyElev->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSpotFireSource %d %s\n", Margin,
            vSpotFireSource->activeItemDataIndex(),
            vSpotFireSource->activeItemName().latin1() );
        fprintf( m_log, "%s  i vSpotTorchingTrees %g %s\n", Margin,
            vSpotTorchingTrees->m_nativeValue,
            vSpotTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHtDownwind %g %s\n", Margin,
            vTreeCoverHtDownwind->m_nativeValue,
            vTreeCoverHtDownwind->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeHt %g %s\n", Margin,
            vTreeHt->m_nativeValue,
            vTreeHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeDbh %g %s\n", Margin,
            vTreeDbh->m_nativeValue,
            vTreeDbh->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeSpeciesSpot %d %s\n", Margin,
            vTreeSpeciesSpot->activeItemDataIndex(),
            vTreeSpeciesSpot->activeItemName().latin1() );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotDistTorchingTrees %g %s\n", Margin,
            vSpotDistTorchingTrees->m_nativeValue,
            vSpotDistTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotCoverHtTorchingTrees %g %s\n", Margin,
            vSpotCoverHtTorchingTrees->m_nativeValue,
            vSpotCoverHtTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFirebrandHtTorchingTrees %g %s\n", Margin,
            vSpotFirebrandHtTorchingTrees->m_nativeValue,
            vSpotFirebrandHtTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFlameDurTorchingTrees %g %s\n", Margin,
            vSpotFlameDurTorchingTrees->m_nativeValue,
            vSpotFlameDurTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFlameHtTorchingTrees %g %s\n", Margin,
            vSpotFlameHtTorchingTrees->m_nativeValue,
            vSpotFlameHtTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFlameRatioTorchingTrees %g %s\n", Margin,
            vSpotFlameRatioTorchingTrees->m_nativeValue,
            vSpotFlameRatioTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotFlatDistTorchingTrees %g %s\n", Margin,
            vSpotFlatDistTorchingTrees->m_nativeValue,
            vSpotFlatDistTorchingTrees->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SpotMapDistBurningPile
 *
 *  Dependent Variables (Outputs)
 *      vSpotMapDistBurningPile (in)
 *
 *  Independent Variables (Inputs)
 *      vSpotDistBurningPile (mi)
 *      vMapScale (in/mi)
 */

void EqCalc::SpotMapDistBurningPile( void )
{
    // Access current input values
    double bd = vSpotDistBurningPile->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * bd;
    // Store results
    vSpotMapDistBurningPile->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SpotMapDistBurningPile() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSpotDistBurningPile %g %s\n", Margin,
            vSpotDistBurningPile->m_nativeValue,
            vSpotDistBurningPile->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotMapDistBurningPile %g %s\n", Margin,
            vSpotMapDistBurningPile->m_nativeValue,
            vSpotMapDistBurningPile->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SpotMapDistSurfaceFire
 *
 *  Dependent Variables (Outputs)
 *      vSpotMapDistSurfaceFire (in)
 *
 *  Independent Variables (Inputs)
 *      vSpotDistSurfaceFire (mi)
 *      vMapScale (in/mi)
 */

void EqCalc::SpotMapDistSurfaceFire( void )
{
    // Access current input values
    double bd = vSpotDistSurfaceFire->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * bd;
    // Store results
    vSpotMapDistSurfaceFire->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SpotMapDistSurfaceFire() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSpotDistSurfaceFire %g %s\n", Margin,
            vSpotDistSurfaceFire->m_nativeValue,
            vSpotDistSurfaceFire->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotMapDistSurfaceFire %g %s\n", Margin,
            vSpotMapDistSurfaceFire->m_nativeValue,
            vSpotMapDistSurfaceFire->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief SpotMapDistTorchingTrees
 *
 *  Dependent Variables (Outputs)
 *      vSpotMapDistTorchingTrees (in)
 *
 *  Independent Variables (Inputs)
 *      vSpotDistTorchingTrees (mi)
 *      vMapScale (in/mi)
 */

void EqCalc::SpotMapDistTorchingTrees( void )
{
    // Access current input values
    double bd = vSpotDistTorchingTrees->m_nativeValue;
    double ms = vMapScale->m_nativeValue;
    // Calculate results
    double md = ms * bd;
    // Store results
    vSpotMapDistTorchingTrees->update( md );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc SpotMapDistTorchingTree() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSpotDistTorchingTrees %g %s\n", Margin,
            vSpotDistTorchingTrees->m_nativeValue,
            vSpotDistTorchingTrees->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vMapScale %g %s\n", Margin,
            vMapScale->m_nativeValue,
            vMapScale->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vSpotMapDistTorchingTrees %g %s\n", Margin,
            vSpotMapDistTorchingTrees->m_nativeValue,
            vSpotMapDistTorchingTrees->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TimeJulianDate
 *
 *  Dependent Variables (Outputs)
 *      vTimeJulianDate (days)
 *      Determines the modified Julian date.
 *      Valid for any date since January 1, 4713 B.C.
 *      Julian calendar is used up to 1582 October 4.
 *      Gregorian calendar is used  after 1582 October 15.
 *      Mjd is JD - 2400000.5, and the day changes at midnight
 *      and not at noon like the Julian Date.
 *
 *  Independent Variables (Inputs)
 *      vTimeIntegerDate (days)
 */

void EqCalc::TimeJulianDate( void )
{
    // Access current input values
    double a = vTimeIntegerDate->m_nativeValue;
    // Calculate results
    int idate = (int) a;
    int year  = idate / 10000;
    int temp  = idate - 10000 * year;
    int month = temp / 100;
    int day   = temp - 100 * month;
    int hour  = (int) (24. * a - (double) idate);
    double jd = CDT_JulianDate( year, month, day, hour, 0, 0, 0 );
    double mjd = CDT_ModifiedJulianDate( jd );
    // Store results
    vTimeJulianDate->update( mjd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TimeJulianDate() 1 1\n", Margin );
        fprintf( m_log, "%s  i vTimeIntegerDate %g %s\n", Margin,
            vTimeIntegerDate->m_nativeValue,
            vTimeIntegerDate->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTimeJulianDate %g %s\n", Margin,
            vTimeJulianDate->m_nativeValue,
            vTimeJulianDate->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeBarkThicknessBehave (Depricated, historical)
 *
 *  Dependent Variables (Outputs)
 *      treeBarkThickness (in)
 *
 *  Independent Variables (Inputs)
 *      vTreeDbh (in)
 *      vTreeSpecies (index)
 */

void EqCalc::TreeBarkThicknessBehave( void )
{
    // Access current input values
    int    spp = vTreeSpecies->activeItemDataIndex();
    double dbh = vTreeDbh->m_nativeValue;
    // Calculate results
    double bt  = FBL_TreeBarkThicknessBehave( spp, dbh );
    // Store results
    vTreeBarkThickness->update( bt );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeBarkThickness() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeDbh %g %s\n", Margin,
            vTreeDbh->m_nativeValue,
            vTreeDbh->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeSpecies %d %s\n", Margin,
            vTreeSpecies->activeItemDataIndex(),
            vTreeSpecies->activeItemName().latin1() );
        fprintf( m_log, "%s  o vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeBarkThicknessFofem
 *
 *  Dependent Variables (Outputs)
 *      treeBarkThickness (in)
 *
 *  Independent Variables (Inputs)
 *      vTreeDbh (in)
 *      vTreeSpecies (index)
 */

void EqCalc::TreeBarkThicknessFofem( void )
{
    // Access current input values
    int    spp = vTreeSpecies->activeItemDataIndex();
    double dbh = vTreeDbh->m_nativeValue;
    // Calculate results
    // The mortality model always limits dbh lower bound to 1.0 inch.
    if ( dbh < 1.0 )
    {
        dbh = 1.0;
    }
    double bt  = FBL_TreeBarkThicknessFofem( spp, dbh );
    // Store results
    vTreeBarkThickness->update( bt );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeBarkThickness() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeDbh %g %s\n", Margin,
            vTreeDbh->m_nativeValue,
            vTreeDbh->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeSpecies %d %s\n", Margin,
            vTreeSpecies->activeItemDataIndex(),
            vTreeSpecies->activeItemName().latin1() );
        fprintf( m_log, "%s  o vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeCrownBaseHt
 *
 *  Dependent Variables (Outputs)
 *      vTreeCrownBaseHt (ft)
 *
 *  Independent Variables (Inputs)
 *      vTreeCrownRatio (fraction)
 *      vTreeHt (ft)
 */

void EqCalc::TreeCrownBaseHt( void )
{
    // Access current input values
    double cr = vTreeCrownRatio->m_nativeValue;
    double ht = vTreeHt->m_nativeValue;
    // Calculate results
    double bh = FBL_TreeCrownBaseHeight( cr, ht );
    // Store results
    vTreeCrownBaseHt->update( bh );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeCrownBaseHt() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeCrownRatio %g %s\n", Margin,
            vTreeCrownRatio->m_nativeValue,
            vTreeCrownRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeHt %g %s\n", Margin,
            vTreeHt->m_nativeValue,
            vTreeHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeCrownBaseHt %g %s\n", Margin,
            vTreeCrownBaseHt->m_nativeValue,
            vTreeCrownBaseHt->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeCrownRatio
 *
 *  Dependent Variables (Outputs)
 *      vTreeCrownRatio (fraction)
 *
 *  Independent Variables (Inputs)
 *      vTreeCrownBaseHt (ft)
 *      vTreeCoverHt (ft)
 */

void EqCalc::TreeCrownRatio( void )
{
    // Access current input values
    double bh = vTreeCrownBaseHt->m_nativeValue;
    double ht = vTreeCoverHt->m_nativeValue;
    // Calculate results
    double cr = FBL_TreeCrownRatio( bh, ht );
    // Store results
    vTreeCrownRatio->update( cr );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeCrownRatio() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeCrownBaseHt %g %s\n", Margin,
            vTreeCrownBaseHt->m_nativeValue,
            vTreeCrownBaseHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHt %g %s\n", Margin,
            vTreeCoverHt->m_nativeValue,
            vTreeCoverHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeCrownRatio %g %s\n", Margin,
            vTreeCrownRatio->m_nativeValue,
            vTreeCrownRatio->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeCrownVolScorchedAtVector
 *
 *  Dependent Variables (Outputs)
 *      vTreeCrownLengScorchedAtVector (ft)
 *      vTreeCrownLengFractionScorchedAtVector (fraction)
 *      vTreeCrownVolScorchedAtVector (fraction)
 *
 *  Independent Variables (Inputs)
 *      vSurfaceFireScorchHtAtVector (ft)
 *      vTreeCrownRatio (fraction)
 *      vTreeCoverHt (ft)
 */

void EqCalc::TreeCrownVolScorchedAtVector( void )
{
    // Access current input values
    double crownRatio = vTreeCrownRatio->m_nativeValue;
    double scorchHt   = vSurfaceFireScorchHtAtVector->m_nativeValue;
    double coverHt    = vTreeCoverHt->m_nativeValue;
    // Calculate results
    double scorchLeng, scorchFrac, scorchVol;
    scorchVol = FBL_TreeCrownScorch( coverHt, crownRatio, scorchHt,
        &scorchLeng, &scorchFrac );
    // Store results
    vTreeCrownLengScorchedAtVector->update( scorchLeng );
    vTreeCrownLengFractionScorchedAtVector->update( scorchFrac );
    vTreeCrownVolScorchedAtVector->update( scorchVol );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeCrownVolScorchedAtVector() 3 3\n", Margin );
        fprintf( m_log, "%s  i vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownRatio %g %s\n", Margin,
            vTreeCrownRatio->m_nativeValue,
            vTreeCrownRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHt %g %s\n", Margin,
            vTreeCoverHt->m_nativeValue,
            vTreeCoverHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeCrownVolScorchedAtVector %g %s\n", Margin,
            vTreeCrownVolScorchedAtVector->m_nativeValue,
            vTreeCrownVolScorchedAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeCrownLengFractionScorchedAtVector %g %s\n", Margin,
            vTreeCrownLengFractionScorchedAtVector->m_nativeValue,
            vTreeCrownLengFractionScorchedAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeCrownLengScorchedAtVector %g %s\n", Margin,
            vTreeCrownLengScorchedAtVector->m_nativeValue,
            vTreeCrownLengScorchedAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityCountAtVector
 *
 *  Dependent Variables (Outputs)
 *      VTreeMortalityCountAtVector (per ac)
 *
 *  Independent Variables (Inputs)
 *      VTreeCount (per ac)
 *      vTreeMortalityRateAtVector (fraction)
 */

void EqCalc::TreeMortalityCountAtVector( void )
{
    // Access current input values
    double mr = vTreeMortalityRateAtVector->m_nativeValue;
    double tc = vTreeCount->m_nativeValue;
    // Calculate results
    double mc = mr * tc;
    // Store results
    vTreeMortalityCountAtVector->update( mc );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityCountAtVector() 2 1\n", Margin );
        fprintf( m_log, "%s  i vTreeMortalityRateAtVector %g %s\n", Margin,
            vTreeMortalityRateAtVector->m_nativeValue,
            vTreeMortalityRateAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCount %g %s\n", Margin,
            vTreeCount->m_nativeValue,
            vTreeCount->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityCountAtVector %g %s\n", Margin,
            vTreeMortalityCountAtVector->m_nativeValue,
            vTreeMortalityCountAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityRateAspenAtVector
 *
 *  Dependent Variables (Outputs)
 *      VTreeMortalityRateAs[enAtVector (fraction)
 *
 *  Independent Variables (Inputs)
 *      vTreeDbh (in)
 *      vSurfaceFireFlameLengAtVector (ft)
 *      vSurfaceFireSeverityAspen (discrete)
 */

void EqCalc::TreeMortalityRateAspenAtVector( void )
{
    // Access current input values
    double dbh = vTreeDbh->m_nativeValue;
    double flame = vSurfaceFireFlameLengAtVector->m_nativeValue;
    int    severity = vSurfaceFireSeverityAspen->activeItemDataIndex();
    // Calculate results
    double mr = FBL_AspenMortality( severity, flame, dbh );
    // Store results
    vTreeMortalityRateAspenAtVector->update( mr );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityRateAspenAtVector() 3 1\n", Margin );
        fprintf( m_log, "%s  i vTreeDbh %g %s\n", Margin,
            vTreeDbh->m_nativeValue,
            vTreeDbh->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireSeverityAspen %d %s\n", Margin,
            vSurfaceFireSeverityAspen->activeItemDataIndex(),
            vSurfaceFireSeverityAspen->activeItemName().latin1() );
        fprintf( m_log, "%s  i vSurfaceFireFlameLengAtVector %g %s\n", Margin,
            vSurfaceFireFlameLengAtVector->m_nativeValue,
            vSurfaceFireFlameLengAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityRateAspenAtVector %g %s\n", Margin,
            vTreeMortalityRateAspenAtVector->m_nativeValue,
            vTreeMortalityRateAspenAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityRateBehaveAtVector
 *
 *  Dependent Variables (Outputs)
 *      VTreeMortalityRateAtVector (fraction)
 *
 *  Independent Variables (Inputs)
 *      vTreeBarkThickness (in)
 *      vTreeCrownVolScorchedAtVector (fraction)
 *      vSurfaceFireScorchHtAtVector (ft)
 */

void EqCalc::TreeMortalityRateBehaveAtVector( void )
{
    // Access current input values
    double barkThickness = vTreeBarkThickness->m_nativeValue;
    double scorchHt = vSurfaceFireScorchHtAtVector->m_nativeValue;
    double crownVolScorched = vTreeCrownVolScorchedAtVector->m_nativeValue;
    // Calculate results
    double mr = FBL_TreeMortalityBehave( barkThickness, scorchHt,
        crownVolScorched ) ;
    // Store results
    vTreeMortalityRateAtVector->update( mr );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityRateBehaveAtVector() 3 1\n", Margin );
        fprintf( m_log, "%s  i vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownVolScorchedAtVector %g %s\n", Margin,
            vTreeCrownVolScorchedAtVector->m_nativeValue,
            vTreeCrownVolScorchedAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityRateAtVector %g %s\n", Margin,
            vTreeMortalityRateAtVector->m_nativeValue,
            vTreeMortalityRateAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityRateFofemAtVector - FOFEM Version
 *      This does not include Equation 3 for Populus tremuloides,
 *      nor does it handles cases where dbh < 1".
 *
 *  Dependent Variables (Outputs)
 *      vTreeMortalityRateAtVector (fraction)
 *
 *  Independent Variables (Inputs)
 *      vTreeBarkThickness (in)
 *      vTreeCrownVolScorchedAtVector (fraction)
 *      vTreeSpecies (index)
 *      vSurfaceFireScorchHtAtVector
 */

void EqCalc::TreeMortalityRateFofemAtVector( void )
{
    // Access current input values
    int    speciesIndex  = vTreeSpecies->activeItemDataIndex();
    double barkThickness = vTreeBarkThickness->m_nativeValue;
    double crownVolScorched = vTreeCrownVolScorchedAtVector->m_nativeValue;
    double scorchHt = vSurfaceFireScorchHtAtVector->m_nativeValue;

    // Calculate results
    double mr = FBL_TreeMortalityFofem( speciesIndex, barkThickness,
        crownVolScorched, scorchHt );
    // Store results
    vTreeMortalityRateAtVector->update( mr );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityRateFofemAtVector() 4 1\n", Margin );
        fprintf( m_log, "%s  i vTreeSpecies %d %s\n", Margin,
            vTreeSpecies->activeItemDataIndex(),
            vTreeSpecies->activeItemName().latin1() );
        fprintf( m_log, "%s  i vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownVolScorchedAtVector %g %s\n", Margin,
            vTreeCrownVolScorchedAtVector->m_nativeValue,
            vTreeCrownVolScorchedAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityRateAtVector %g %s\n", Margin,
            vTreeMortalityRateAtVector->m_nativeValue,
            vTreeMortalityRateAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityRateFofem2AtVector - FOFEM Version
 *      This does not include Equation 3 for Populus tremuloides,
 *      nor does it handles cases where dbh < 1".
 *
 *  Dependent Variables (Outputs)
 *      vTreeMortalityRateAtVector (fraction)
 *
 *  Independent Variables (Inputs)
 *      vTreeBarkThickness (in)
 *      vTreeCrownVolScorchedAtVector (fraction)
 *      vTreeSpeciesMortality (index)
 *      vSurfaceFireScorchHtAtVector
 */

void EqCalc::TreeMortalityRateFofem2AtVector( void )
{
    // Access current input values
    int    speciesIndex  = vTreeSpeciesMortality->activeItemDataIndex();
    double barkThickness = vTreeBarkThickness->m_nativeValue;
    double crownVolScorched = vTreeCrownVolScorchedAtVector->m_nativeValue;
    double scorchHt = vSurfaceFireScorchHtAtVector->m_nativeValue;

    // 0==nonspruce, 1=spruce
    speciesIndex = ( speciesIndex == 0 ) ? 0 : 105;

    // Calculate results
    double mr = FBL_TreeMortalityFofem( speciesIndex, barkThickness,
        crownVolScorched, scorchHt );
    // Store results
    vTreeMortalityRateAtVector->update( mr );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityRateFofem2AtVector() 4 1\n", Margin );
        fprintf( m_log, "%s  i vTreeSpeciesMortality %d %s\n", Margin,
            vTreeSpeciesMortality->activeItemDataIndex(),
            vTreeSpeciesMortality->activeItemName().latin1() );
        fprintf( m_log, "%s  i vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownVolScorchedAtVector %g %s\n", Margin,
            vTreeCrownVolScorchedAtVector->m_nativeValue,
            vTreeCrownVolScorchedAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityRateAtVector %g %s\n", Margin,
            vTreeMortalityRateAtVector->m_nativeValue,
            vTreeMortalityRateAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief TreeMortalityRateFofemHoodAtVector
 *
 *  Combines the FOFEM and Hood models in a single entry point.
 *
 *  Dependent Variables (Outputs)
 *      vTreeMortalityRateAtVector (fraction)
 *
 *  Independent Variables (Inputs)
 *      vTreeBarkThickness (in)
 *      vTreeCrownLengScorchedAtVector (fraction)
 *      vTreeCrownVolScorchedAtVector (fraction)
 *      vTreeSpecies (index)
 *      vSurfaceFireScorchHtAtVector
 */

void EqCalc::TreeMortalityRateFofemHoodAtVector( void )
{
    // Access current input values
    int    spp = vTreeSpecies->activeItemDataIndex();			/* index */
    double dbh = vTreeDbh->m_nativeValue;						/* inches */
	double cls = vTreeCrownLengFractionScorchedAtVector->m_nativeValue;	/* fraction */
    double cvs = vTreeCrownVolScorchedAtVector->m_nativeValue;	/* fraction */
    double brk = vTreeBarkThickness->m_nativeValue;				/* inches */
    double sht = vSurfaceFireScorchHtAtVector->m_nativeValue;	/* ft */
	double mr  = 0.;

    // Determine whether to use FOFEM or Hood based on species index
    int hood = -1;
    if ( spp == 2 ) hood = 0;                       // ABICON
    else if ( spp == 3 || spp == 4 ) hood = 1;      // ABIGRA & ABILAS
    else if ( spp == 5 ) hood =2;                   // ABIMAG
    else if ( spp == 78 ) hood = 3;                 // LIBDEC
    else if ( spp == 77) hood = 4;                  // LAROCC
    else if ( spp == 110 || spp == 114 ) hood = 5;  // PINALB & PINCON
    else if ( spp == 103 ) hood = 6;                // PICENG
    else if ( spp == 120 ) hood = 7;                // PINLAM
    else if ( spp == 123 || spp == 119 ) hood = 8;  // PINPON & PINJEF
    else if ( spp == 147 ) hood = 9;                // PSEMEN

    // If using Hood ...
    if ( hood >= 0 )
    {
        mr = FBL_TreeMortalityHood( hood, ( 2.54 * dbh ), cls, cvs );
    }
    // ... otherwise using FOFEM
    else
    {
        mr = FBL_TreeMortalityFofem( spp, brk, cvs, sht );
    }
    // Store results
    vTreeMortalityRateAtVector->update( mr );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc TreeMortalityRateFofemHoodAtVector() 6 1\n", Margin );
        fprintf( m_log, "%s  i vTreeSpecies %d %s\n", Margin,
            vTreeSpecies->activeItemDataIndex(),
            vTreeSpecies->activeItemName().latin1() );
        fprintf( m_log, "%s  i vTreeDbh %g %s\n", Margin,
            vTreeDbh->m_nativeValue,
            vTreeDbh->m_nativeUnits.latin1() );
		fprintf( m_log, "%s  i vTreeBarkThickness %g %s\n", Margin,
            vTreeBarkThickness->m_nativeValue,
            vTreeBarkThickness->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownVolScorchedAtVector %g %s\n", Margin,
            vTreeCrownVolScorchedAtVector->m_nativeValue,
            vTreeCrownVolScorchedAtVector->m_nativeUnits.latin1() );
		fprintf( m_log, "%s  i vTreeCrownLengFractionScorchedAtVector %g %s\n", Margin,
            vTreeCrownLengFractionScorchedAtVector->m_nativeValue,
            vTreeCrownLengFractionScorchedAtVector->m_nativeUnits.latin1() );
 		fprintf( m_log, "%s  i vSurfaceFireScorchHtAtVector %g %s\n", Margin,
            vSurfaceFireScorchHtAtVector->m_nativeValue,
            vSurfaceFireScorchHtAtVector->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vTreeMortalityRateAtVector %g %s\n", Margin,
            vTreeMortalityRateAtVector->m_nativeValue,
            vTreeMortalityRateAtVector->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WindAdjFactor
 *
 *  Dependent Variables (Outputs)
 *      vTreeCanopyCrownFraction (ft3/ft3).
 *      vWindAdjFactor (dl)
 *      vWindAdjMethod (discrete)
 *
 *  Independent Variables (Inputs)
 *      vTreeCanopyCover (fraction)
 *      vTreeCoverHt (ft)
 *      vTreeCrownRatio (fraction)
 *      vSurfaceFuelBedDepth (ft)
 */

void EqCalc::WindAdjFactor( void )
{
    // Access current input values
    double cc = vTreeCanopyCover->m_nativeValue;
    double ch = vTreeCoverHt->m_nativeValue;
    double cr = vTreeCrownRatio->m_nativeValue;
    double fd = vSurfaceFuelBedDepth->m_nativeValue;
    // Calculate results
    int method = 0;
    double fraction = 0.0;
    double waf = FBL_WindAdjustmentFactor( cc, ch, cr, fd, &fraction, &method );
    // Store results
    vWindAdjFactor->update( waf );
    vWindAdjMethod->updateItem( method );
    vTreeCanopyCrownFraction->update( fraction );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WindAdjFactor() 4 1\n", Margin );
        fprintf( m_log, "%s  i vTreeCanopyCover %d %s\n", Margin,
            vTreeCanopyCover->activeItemDataIndex(),
            vTreeCanopyCover->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCoverHt %d %s\n", Margin,
            vTreeCoverHt->activeItemDataIndex(),
            vTreeCoverHt->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vTreeCrownRatio %d %s\n", Margin,
            vTreeCrownRatio->activeItemDataIndex(),
            vTreeCrownRatio->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSurfaceFuelBedDepth %d %s\n", Margin,
            vSurfaceFuelBedDepth->activeItemDataIndex(),
            vSurfaceFuelBedDepth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWindAdjFactor %d %s\n", Margin,
            vWindAdjFactor->activeItemDataIndex(),
            vWindAdjFactor->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWindAdjMethod %d %s\n", Margin,
            vWindAdjMethod->activeItemDataIndex(),
            vWindAdjMethod->activeItemName().latin1() );
        fprintf( m_log, "%s  o vTreeCanopyCrownFraction %d %s\n", Margin,
            vTreeCanopyCrownFraction->activeItemDataIndex(),
            vTreeCanopyCrownFraction->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WindDirFromNorth
 *
 *  Dependent Variables (Outputs)
 *      vWindDirFromNorth (degrees clockwise from North)
 *
 *  Independent Variables (Inputs)
 *      vWindDirFromCompass (compass point name)
 */

void EqCalc::WindDirFromNorth( void )
{
    // Access current input values
    int id = vWindDirFromCompass->activeItemDataIndex();
    // Calculate results
    double dd = 22.5 * (double) id;
    // Store results
    vWindDirFromNorth->update( dd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WindDirFromNorth() 1 1\n", Margin );
        fprintf( m_log, "%s  i vWindDirFromCompass %d %s\n", Margin,
            vWindDirFromCompass->activeItemDataIndex(),
            vWindDirFromCompass->activeItemName().latin1() );
        fprintf( m_log, "%s  o vWindDirFromNorth %g %s\n", Margin,
            vWindDirFromNorth->m_nativeValue,
            vWindDirFromNorth->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WindDirFromUpslope
 *
 *  Dependent Variables (Outputs)
 *      vWindDirFromUpslope (VECTOR degrees clockwise from upslope)
 *
 *  Independent Variables (Inputs)
 *      vSiteUpslopeDirFromNorth
 *      vWindDirFromNorth (SOURCE degrees clockwise from north)
 */

void EqCalc::WindDirFromUpslope( void )
{
    // Access current input values
    double sd = vSiteUpslopeDirFromNorth->m_nativeValue;
    double wd = vWindDirFromNorth->m_nativeValue;
    // Calculate results
    // Convert from source wind to wind vector
    if ( (wd -= 180.) < 0. )
    {
        wd += 360.;
    }
    // Get the angle between slope and wind vector
    double dd = wd - sd;
    if ( dd < 0. )
    {
        dd += 360.;
    }
    if ( fabs( dd ) < 0.5 )
    {
        dd = 0.0;
    }
    // Store results
    vWindDirFromUpslope->update( dd );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WindDirFromUpslope() 2 1\n", Margin );
        fprintf( m_log, "%s  i vSiteUpslopeDirFromNorth %g %s\n", Margin,
            vSiteUpslopeDirFromNorth->m_nativeValue,
            vSiteUpslopeDirFromNorth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindDirFromNorth %g %s\n", Margin,
            vWindDirFromNorth->m_nativeValue,
            vWindDirFromNorth->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWindDirFromUpslope %g %s\n", Margin,
            vWindDirFromUpslope->m_nativeValue,
            vWindDirFromUpslope->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WindSpeedAt20Ft
 *
 *  Dependent Variables (Outputs)
 *      vWindSpeedAt20Ft (mi/h)
 *
 *  Independent Variables (Inputs)
 *      vWindSpeedAt10M (mi/h)
 */

void EqCalc::WindSpeedAt20Ft ( void )
{
    // Access current input values
    double w10m = vWindSpeedAt10M->m_nativeValue;
    // Calculate results
    double w20ft = FBL_WindSpeedAt20Ft( w10m );
    // Store results
    vWindSpeedAt20Ft->update( w20ft );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WindSpeedAt20Ft() 1 1\n", Margin );
        fprintf( m_log, "%s  i vWindSpeedAt10M %g %s\n", Margin,
            vWindSpeedAt10M->m_nativeValue,
            vWindSpeedAt10M->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WindSpeedAtMidflame
 *
 *  Dependent Variables (Outputs)
 *      vWindSpeedAtMidflame (mi/h)
 *
 *  Independent Variables (Inputs)
 *      vWindSpeedAt20Ft (mi/h)
 *      vWindAdjFactor (fraction)
 */

void EqCalc::WindSpeedAtMidflame ( void )
{
    // Access current input values
    double ws = vWindSpeedAt20Ft->m_nativeValue;
    double rf = vWindAdjFactor->m_nativeValue;
    // Calculate results
    double wm = ws * rf;
    // Store results
    vWindSpeedAtMidflame->update( wm );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WindSpeedAtMidflame() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWindSpeedAt20Ft %g %s\n", Margin,
            vWindSpeedAt20Ft->m_nativeValue,
            vWindSpeedAt20Ft->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindAdjFactor %g %s\n", Margin,
            vWindAdjFactor->m_nativeValue,
            vWindAdjFactor->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WthrCumulusBaseHt
 *      From http://www.usatoday.com/weather/whumcalc.htm
 *
 *  Dependent Variables (Outputs)
 *      vWthrCumulusBaseHt (ft)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vWthrDewPointTemp (oF)
 */

void EqCalc::WthrCumulusBaseHt( void )
{
    // Access current input values
    double Tf = vWthrAirTemp->m_nativeValue;
    double Tdf = vWthrDewPointTemp->m_nativeValue;
    // Calculate results
    double ht = 222. * (Tf - Tdf);
    if ( ht < 0. )
    {
        ht = 0.0;
    }
    // Store results
    vWthrCumulusBaseHt->update( ht );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WthrCumulusBaseHt() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWthrDewPointTemp %g %s\n", Margin,
            vWthrDewPointTemp->m_nativeValue,
            vWthrDewPointTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWthrCumulusBaseHt %g %s\n", Margin,
            vWthrCumulusBaseHt->m_nativeValue,
            vWthrCumulusBaseHt->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WthrDewPointTemp
 *
 *  Dependent Variables (Outputs)
 *      vWthrDewPointTemp (oF)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vWthrWetBulbTemp (oF)
 *      vSiteElevation (ft)
 */

void EqCalc::WthrDewPointTemp( void )
{
    // Access current input values
    double dbulb = vWthrAirTemp->m_nativeValue;
    double wbulb = vWthrWetBulbTemp->m_nativeValue;
    double elev  = vSiteElevation->m_nativeValue;
    // Calculate results
    double dewpoint = FBL_DewPointTemperature( dbulb, wbulb, elev );
    // Store results
    vWthrDewPointTemp->update( dewpoint );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WthrDewPointTemp() 3 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWthrWetBulbTemp %g %s\n", Margin,
            vWthrWetBulbTemp->m_nativeValue,
            vWthrWetBulbTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vSiteElevation %g %s\n", Margin,
            vSiteElevation->m_nativeValue,
            vSiteElevation->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWthrDewPointTemp %g %s\n", Margin,
            vWthrDewPointTemp->m_nativeValue,
            vWthrDewPointTemp->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WthrHeatIndex
 *      From http://www.usatoday.com/weather/whumcalc.htm
 *
 *  Dependent Variables (Outputs)
 *      vWthrHeatIndex (dl)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vWthrRelativeHumidity (%)
 */

void EqCalc::WthrHeatIndex( void )
{
    // Access current input values
    double at = vWthrAirTemp->m_nativeValue;
    double rh = vWthrRelativeHumidity->m_nativeValue;
    // Calculate results
    double hi = FBL_HeatIndex1( at, rh );
    // Store results
    vWthrHeatIndex->update( hi );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WthrHeatIndex() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWthrRelativeHumidity %g %s\n", Margin,
            vWthrRelativeHumidity->m_nativeValue,
            vWthrRelativeHumidity->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWthrHeatIndex %g %s\n", Margin,
            vWthrHeatIndex->m_nativeValue,
            vWthrHeatIndex->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WthrRelativeHumidity
 *
 *  Dependent Variables (Outputs)
 *      vWthrRelativeHumidity (percent)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vWthrDewPointTemp (oF)
 */

void EqCalc::WthrRelativeHumidity( void )
{
    // Access current input values
    double dbulb = vWthrAirTemp->m_nativeValue;
    double dewpt = vWthrDewPointTemp->m_nativeValue;
    // Calculate results
    double rh    = 100. * FBL_RelativeHumidity( dbulb, dewpt );
    // Store results
    vWthrRelativeHumidity->update( rh );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WthrRelativeHumidity() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWthrDewPointTemp %g %s\n", Margin,
            vWthrDewPointTemp->m_nativeValue,
            vWthrDewPointTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWthrRelativeHumidity %g %s\n", Margin,
            vWthrRelativeHumidity->m_nativeValue,
            vWthrRelativeHumidity->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WthrSummerSimmerIndex
 *      From http://www.usatoday.com/weather/whumcalc.htm
 *
 *  Dependent Variables (Outputs)
 *      vWthrSummerSimmerIndex (dl)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vWthrRelativeHumidity (%)
 */

void EqCalc::WthrSummerSimmerIndex( void )
{
    // Access current input values
    double at = vWthrAirTemp->m_nativeValue;
    double rh = vWthrRelativeHumidity->m_nativeValue;
    // Calculate results
    double ssi = FBL_SummerSimmerIndex( at, rh );
    // Store results
    vWthrSummerSimmerIndex->update( ssi );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WthrSummerSimmerIndex() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWthrRelativeHumidity %g %s\n", Margin,
            vWthrRelativeHumidity->m_nativeValue,
            vWthrRelativeHumidity->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWthrSummerSimmerIndex %g %s\n", Margin,
            vWthrSummerSimmerIndex->m_nativeValue,
            vWthrSummerSimmerIndex->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief WthrWindChillTemp
 *
 *  Dependent Variables (Outputs)
 *      vWthrWindChillTemp (oF)
 *
 *  Independent Variables (Inputs)
 *      vWthrAirTemp (oF)
 *      vWindSpeedAtMidflame (mi/h)
 */

void EqCalc::WthrWindChillTemp( void )
{
    // Access current input values
    double at = vWthrAirTemp->m_nativeValue;
    double ws = vWindSpeedAtMidflame->m_nativeValue;
    // Calculate results
    double wc = FBL_WindChillTemperature( at, ws );
    // Store results
    vWthrWindChillTemp->update( wc );
    // Log results
    if( m_log )
    {
        fprintf( m_log, "%sbegin proc WthrWindChill() 2 1\n", Margin );
        fprintf( m_log, "%s  i vWthrAirTemp %g %s\n", Margin,
            vWthrAirTemp->m_nativeValue,
            vWthrAirTemp->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  i vWindSpeedAtMidflame %g %s\n", Margin,
            vWindSpeedAtMidflame->m_nativeValue,
            vWindSpeedAtMidflame->m_nativeUnits.latin1() );
        fprintf( m_log, "%s  o vWthrWindChillTemp %g %s\n", Margin,
            vWthrWindChillTemp->m_nativeValue,
            vWthrWindChillTemp->m_nativeUnits.latin1() );
    }
    return;
}

//------------------------------------------------------------------------------
/*! \brief EqCalc constructor.
 */

EqCalc::EqCalc( EqTree *eqTree ) :
    m_eqTree(eqTree),
    m_log(0)
{
    vContainAttackBack       = m_eqTree->getVarPtr( "vContainAttackBack" );
    vContainAttackDist       = m_eqTree->getVarPtr( "vContainAttackDist" );
    vContainAttackHead       = m_eqTree->getVarPtr( "vContainAttackHead" );
    vContainAttackPerimeter  = m_eqTree->getVarPtr( "vContainAttackPerimeter" );
    vContainAttackSize       = m_eqTree->getVarPtr( "vContainAttackSize" );
    vContainAttackTactic     = m_eqTree->getVarPtr( "vContainAttackTactic" );
    vContainCost             = m_eqTree->getVarPtr( "vContainCost" );
    vContainDiagram          = m_eqTree->getVarPtr( "vContainDiagram" );
    vContainLimitDist        = m_eqTree->getVarPtr( "vContainLimitDist" );
    vContainLine             = m_eqTree->getVarPtr( "vContainLine" );
    vContainPoints           = m_eqTree->getVarPtr( "vContainPoints" );
    vContainReportBack       = m_eqTree->getVarPtr( "vContainReportBack" );
    vContainReportHead       = m_eqTree->getVarPtr( "vContainReportHead" );
    vContainReportRatio      = m_eqTree->getVarPtr( "vContainReportRatio" );
    vContainReportSize       = m_eqTree->getVarPtr( "vContainReportSize" );
    vContainReportSpread     = m_eqTree->getVarPtr( "vContainReportSpread" );
    vContainResourceArrival  = m_eqTree->getVarPtr( "vContainResourceArrival" );
    vContainResourceBaseCost = m_eqTree->getVarPtr( "vContainResourceBaseCost" );
    vContainResourceDuration = m_eqTree->getVarPtr( "vContainResourceDuration" );
    vContainResourceHourCost = m_eqTree->getVarPtr( "vContainResourceHourCost" );
    vContainResourceName     = m_eqTree->getVarPtr( "vContainResourceName" );
    vContainResourceProd     = m_eqTree->getVarPtr( "vContainResourceProd" );
    vContainResourcesUsed    = m_eqTree->getVarPtr( "vContainResourcesUsed" );
    vContainSize             = m_eqTree->getVarPtr( "vContainSize" );
    vContainStatus           = m_eqTree->getVarPtr( "vContainStatus" );
    vContainTime             = m_eqTree->getVarPtr( "vContainTime" );
    vContainXMax             = m_eqTree->getVarPtr( "vContainXMax" );
    vContainXMin             = m_eqTree->getVarPtr( "vContainXMin" );
    vContainYMax             = m_eqTree->getVarPtr( "vContainYMax" );

    vCrownFireActiveCrown         = m_eqTree->getVarPtr( "vCrownFireActiveCrown" );
    vCrownFireActiveRatio         = m_eqTree->getVarPtr( "vCrownFireActiveRatio" );
    vCrownFireArea                = m_eqTree->getVarPtr( "vCrownFireArea" );
    vCrownFireCritCrownSpreadRate = m_eqTree->getVarPtr( "vCrownFireCritCrownSpreadRate" );
    vCrownFireCritSurfFireInt     = m_eqTree->getVarPtr( "vCrownFireCritSurfFireInt" );
    vCrownFireCritSurfFlameLeng   = m_eqTree->getVarPtr( "vCrownFireCritSurfFlameLeng" );
    vCrownFireFlameLeng           = m_eqTree->getVarPtr( "vCrownFireFlameLeng" );
    vCrownFireFuelLoad            = m_eqTree->getVarPtr( "vCrownFireFuelLoad" );
    vCrownFireHeatPerUnitArea     = m_eqTree->getVarPtr( "vCrownFireHeatPerUnitArea" );
    vCrownFireHeatPerUnitAreaCanopy = m_eqTree->getVarPtr( "vCrownFireHeatPerUnitAreaCanopy" );
    vCrownFireLengthToWidth       = m_eqTree->getVarPtr( "vCrownFireLengthToWidth" );
    vCrownFireLineInt             = m_eqTree->getVarPtr( "vCrownFireLineInt" );
    vCrownFirePerimeter           = m_eqTree->getVarPtr( "vCrownFirePerimeter" );
    vCrownFirePowerOfFire         = m_eqTree->getVarPtr( "vCrownFirePowerOfFire" );
    vCrownFirePowerOfWind         = m_eqTree->getVarPtr( "vCrownFirePowerOfWind" );
    vCrownFirePowerRatio          = m_eqTree->getVarPtr( "vCrownFirePowerRatio" );
    vCrownFireSpreadDist          = m_eqTree->getVarPtr( "vCrownFireSpreadDist" );
    vCrownFireSpreadMapDist       = m_eqTree->getVarPtr( "vCrownFireSpreadMapDist" );
    vCrownFireSpreadRate          = m_eqTree->getVarPtr( "vCrownFireSpreadRate" );
    vCrownFireTransRatio          = m_eqTree->getVarPtr( "vCrownFireTransRatio" );
    vCrownFireTransToCrown        = m_eqTree->getVarPtr( "vCrownFireTransToCrown" );
    vCrownFireType                = m_eqTree->getVarPtr( "vCrownFireType" );
    vCrownFireWindDriven          = m_eqTree->getVarPtr( "vCrownFireWindDriven" );

    vDocDescription      = m_eqTree->getVarPtr( "vDocDescription" );
    vDocFireAnalyst      = m_eqTree->getVarPtr( "vDocFireAnalyst" );
    vDocFireName         = m_eqTree->getVarPtr( "vDocFireName" );
    vDocFirePeriod       = m_eqTree->getVarPtr( "vDocFirePeriod" );
    vDocRxAdminUnit      = m_eqTree->getVarPtr( "vDocRxAdminUnit" );
    vDocRxName           = m_eqTree->getVarPtr( "vDocRxName" );
    vDocRxPreparedBy     = m_eqTree->getVarPtr( "vDocRxPreparedBy" );
    vDocTrainingCourse   = m_eqTree->getVarPtr( "vDocTrainingCourse" );
    vDocTrainingExercise = m_eqTree->getVarPtr( "vDocTrainingExercise" );
    vDocTrainingTrainee  = m_eqTree->getVarPtr( "vDocTrainingTrainee" );

    vIgnitionFirebrandFuelMois  = m_eqTree->getVarPtr( "vIgnitionFirebrandFuelMois" );
    vIgnitionFirebrandProb      = m_eqTree->getVarPtr( "vIgnitionFirebrandProb" );
    vIgnitionLightningDuffDepth = m_eqTree->getVarPtr( "vIgnitionLightningDuffDepth" );
    vIgnitionLightningFuelMois  = m_eqTree->getVarPtr( "vIgnitionLightningFuelMois" );
    vIgnitionLightningFuelType  = m_eqTree->getVarPtr( "vIgnitionLightningFuelType" );
    vIgnitionLightningProb      = m_eqTree->getVarPtr( "vIgnitionLightningProb" );

    vMapContourCount    = m_eqTree->getVarPtr( "vMapContourCount" );
    vMapContourInterval = m_eqTree->getVarPtr( "vMapContourInterval" );
    vMapDist            = m_eqTree->getVarPtr( "vMapDist" );
    vMapFraction        = m_eqTree->getVarPtr( "vMapFraction" );
    vMapScale           = m_eqTree->getVarPtr( "vMapScale" );

    vSafetyZoneEquipmentArea   = m_eqTree->getVarPtr( "vSafetyZoneEquipmentArea" );
    vSafetyZoneEquipmentNumber = m_eqTree->getVarPtr( "vSafetyZoneEquipmentNumber" );
    vSafetyZonePersonnelArea   = m_eqTree->getVarPtr( "vSafetyZonePersonnelArea" );
    vSafetyZonePersonnelNumber = m_eqTree->getVarPtr( "vSafetyZonePersonnelNumber" );
    vSafetyZoneRadius          = m_eqTree->getVarPtr( "vSafetyZoneRadius" );
    vSafetyZoneSepDist         = m_eqTree->getVarPtr( "vSafetyZoneSepDist" );
    vSafetyZoneSize            = m_eqTree->getVarPtr( "vSafetyZoneSize" );

    vSiteAspectDirFromCompass = m_eqTree->getVarPtr( "vSiteAspectDirFromCompass" );
    vSiteAspectDirFromNorth   = m_eqTree->getVarPtr( "vSiteAspectDirFromNorth" );
    vSiteElevation            = m_eqTree->getVarPtr( "vSiteElevation" );
    vSiteLatitude             = m_eqTree->getVarPtr( "vSiteLatitude" );
    vSiteLongitude            = m_eqTree->getVarPtr( "vSiteLongitude" );
    vSiteRidgeToValleyDist    = m_eqTree->getVarPtr( "vSiteRidgeToValleyDist" );
    vSiteRidgeToValleyElev    = m_eqTree->getVarPtr( "vSiteRidgeToValleyElev" );
    vSiteRidgeToValleyMapDist = m_eqTree->getVarPtr( "vSiteRidgeToValleyMapDist" );
    vSiteSlopeDegrees         = m_eqTree->getVarPtr( "vSiteSlopeDegrees" );
    vSiteSlopeFraction        = m_eqTree->getVarPtr( "vSiteSlopeFraction" );
    vSiteSlopeReach           = m_eqTree->getVarPtr( "vSiteSlopeReach" );
    vSiteSlopeRise            = m_eqTree->getVarPtr( "vSiteSlopeRise" );
    vSiteSunShading           = m_eqTree->getVarPtr( "vSiteSunShading" );
    vSiteTimeZone             = m_eqTree->getVarPtr( "vSiteTimeZone" );
    vSiteUpslopeDirFromNorth  = m_eqTree->getVarPtr( "vSiteUpslopeDirFromNorth" );

    vSpotCoverHtBurningPile        = m_eqTree->getVarPtr( "vSpotCoverHtBurningPile" );
    vSpotCoverHtSurfaceFire        = m_eqTree->getVarPtr( "vSpotCoverHtSurfaceFire" );
    vSpotCoverHtTorchingTrees      = m_eqTree->getVarPtr( "vSpotCoverHtTorchingTrees" );
    vSpotDistBurningPile           = m_eqTree->getVarPtr( "vSpotDistBurningPile" );
    vSpotDistSurfaceFire           = m_eqTree->getVarPtr( "vSpotDistSurfaceFire" );
    vSpotDistTorchingTrees         = m_eqTree->getVarPtr( "vSpotDistTorchingTrees" );
    vSpotFirebrandDriftSurfaceFire = m_eqTree->getVarPtr( "vSpotFirebrandDriftSurfaceFire" );
    vSpotFirebrandHtBurningPile    = m_eqTree->getVarPtr( "vSpotFirebrandHtBurningPile" );
    vSpotFirebrandHtSurfaceFire    = m_eqTree->getVarPtr( "vSpotFirebrandHtSurfaceFire" );
    vSpotFirebrandHtTorchingTrees  = m_eqTree->getVarPtr( "vSpotFirebrandHtTorchingTrees" );
    vSpotFlameDurTorchingTrees     = m_eqTree->getVarPtr( "vSpotFlameDurTorchingTrees" );
    vSpotFlameHtTorchingTrees      = m_eqTree->getVarPtr( "vSpotFlameHtTorchingTrees" );
    vSpotFlameRatioTorchingTrees   = m_eqTree->getVarPtr( "vSpotFlameRatioTorchingTrees" );
    vSpotFlatDistBurningPile       = m_eqTree->getVarPtr( "vSpotFlatDistBurningPile" );
    vSpotFlatDistSurfaceFire       = m_eqTree->getVarPtr( "vSpotFlatDistSurfaceFire" );
    vSpotFlatDistTorchingTrees     = m_eqTree->getVarPtr( "vSpotFlatDistTorchingTrees" );
    vSpotFireSource                = m_eqTree->getVarPtr( "vSpotFireSource" );
    vSpotMapDistBurningPile        = m_eqTree->getVarPtr( "vSpotMapDistBurningPile" );
    vSpotMapDistSurfaceFire        = m_eqTree->getVarPtr( "vSpotMapDistSurfaceFire" );
    vSpotMapDistTorchingTrees      = m_eqTree->getVarPtr( "vSpotMapDistTorchingTrees" );
    vSpotTorchingTrees             = m_eqTree->getVarPtr( "vSpotTorchingTrees" );

    vSurfaceFireArea                 = m_eqTree->getVarPtr( "vSurfaceFireArea" );
    vSurfaceFireCharacteristicsDiagram = m_eqTree->getVarPtr( "vSurfaceFireCharacteristicsDiagram" );
    vSurfaceFireDistAtBack           = m_eqTree->getVarPtr( "vSurfaceFireDistAtBack" );
    vSurfaceFireDistAtHead           = m_eqTree->getVarPtr( "vSurfaceFireDistAtHead" );
    vSurfaceFireDistAtVector         = m_eqTree->getVarPtr( "vSurfaceFireDistAtVector" );
    vSurfaceFireEccentricity         = m_eqTree->getVarPtr( "vSurfaceFireEccentricity" );
    vSurfaceFireEffWindAtHead        = m_eqTree->getVarPtr( "vSurfaceFireEffWindAtHead" );
    vSurfaceFireEffWindAtVector      = m_eqTree->getVarPtr( "vSurfaceFireEffWindAtVector" );
    vSurfaceFireElapsedTime          = m_eqTree->getVarPtr( "vSurfaceFireElapsedTime" );
    vSurfaceFireFlameAngleAtVector   = m_eqTree->getVarPtr( "vSurfaceFireFlameAngleAtVector" );
    vSurfaceFireFlameHtAtVector      = m_eqTree->getVarPtr( "vSurfaceFireFlameHtAtVector" );
    vSurfaceFireFlameHtPile          = m_eqTree->getVarPtr( "vSurfaceFireFlameHtPile" );
    vSurfaceFireFlameLengAtHead      = m_eqTree->getVarPtr( "vSurfaceFireFlameLengAtHead" );
    vSurfaceFireFlameLengAtVector    = m_eqTree->getVarPtr( "vSurfaceFireFlameLengAtVector" );
    vSurfaceFireHeatPerUnitArea      = m_eqTree->getVarPtr( "vSurfaceFireHeatPerUnitArea" );
    vSurfaceFireHeatSource           = m_eqTree->getVarPtr( "vSurfaceFireHeatSource" );
    vSurfaceFireLengDist             = m_eqTree->getVarPtr( "vSurfaceFireLengDist" );
    vSurfaceFireLengMapDist          = m_eqTree->getVarPtr( "vSurfaceFireLengMapDist" );
    vSurfaceFireLengthToWidth        = m_eqTree->getVarPtr( "vSurfaceFireLengthToWidth" );
    vSurfaceFireLineIntAtHead        = m_eqTree->getVarPtr( "vSurfaceFireLineIntAtHead" );
    vSurfaceFireLineIntAtVector      = m_eqTree->getVarPtr( "vSurfaceFireLineIntAtVector" );
    vSurfaceFireMapDistAtBack        = m_eqTree->getVarPtr( "vSurfaceFireMapDistAtBack" );
    vSurfaceFireMapDistAtHead        = m_eqTree->getVarPtr( "vSurfaceFireMapDistAtHead" );
    vSurfaceFireMapDistAtVector      = m_eqTree->getVarPtr( "vSurfaceFireMapDistAtVector" );
    vSurfaceFireMaxDirDiagram        = m_eqTree->getVarPtr( "vSurfaceFireMaxDirDiagram" );
    vSurfaceFireMaxDirFromNorth      = m_eqTree->getVarPtr( "vSurfaceFireMaxDirFromNorth" );
    vSurfaceFireMaxDirFromUpslope    = m_eqTree->getVarPtr( "vSurfaceFireMaxDirFromUpslope" );
    vSurfaceFireNoWindRate           = m_eqTree->getVarPtr( "vSurfaceFireNoWindRate" );
    vSurfaceFirePerimeter            = m_eqTree->getVarPtr( "vSurfaceFirePerimeter" );
    vSurfaceFirePropagatingFlux      = m_eqTree->getVarPtr( "vSurfaceFirePropagatingFlux" );
    vSurfaceFireReactionInt          = m_eqTree->getVarPtr( "vSurfaceFireReactionInt" );
    vSurfaceFireReactionIntDead      = m_eqTree->getVarPtr( "vSurfaceFireReactionIntDead" );
    vSurfaceFireReactionIntLive      = m_eqTree->getVarPtr( "vSurfaceFireReactionIntLive" );
    vSurfaceFireResidenceTime        = m_eqTree->getVarPtr( "vSurfaceFireResidenceTime" );
    vSurfaceFireScorchHtAtVector     = m_eqTree->getVarPtr( "vSurfaceFireScorchHtAtVector" );
    vSurfaceFireSeverityAspen        = m_eqTree->getVarPtr( "vSurfaceFireSeverityAspen" );
    vSurfaceFireShapeDiagram         = m_eqTree->getVarPtr( "vSurfaceFireShapeDiagram" );
    vSurfaceFireSlopeFactor          = m_eqTree->getVarPtr( "vSurfaceFireSlopeFactor" );
    vSurfaceFireSpreadAtBack         = m_eqTree->getVarPtr( "vSurfaceFireSpreadAtBack" );
    vSurfaceFireSpreadAtHead         = m_eqTree->getVarPtr( "vSurfaceFireSpreadAtHead" );
    vSurfaceFireSpreadAtVector       = m_eqTree->getVarPtr( "vSurfaceFireSpreadAtVector" );
    vSurfaceFireVectorBeta           = m_eqTree->getVarPtr( "vSurfaceFireVectorBeta" );
    vSurfaceFireVectorDirFromCompass = m_eqTree->getVarPtr( "vSurfaceFireVectorDirFromCompass" );
    vSurfaceFireVectorDirFromNorth   = m_eqTree->getVarPtr( "vSurfaceFireVectorDirFromNorth" );
    vSurfaceFireVectorDirFromUpslope = m_eqTree->getVarPtr( "vSurfaceFireVectorDirFromUpslope" );
    vSurfaceFireWidthDist            = m_eqTree->getVarPtr( "vSurfaceFireWidthDist" );
    vSurfaceFireWidthMapDist         = m_eqTree->getVarPtr( "vSurfaceFireWidthMapDist" );
    vSurfaceFireWindFactor           = m_eqTree->getVarPtr( "vSurfaceFireWindFactor" );
    vSurfaceFireWindSpeedFlag        = m_eqTree->getVarPtr( "vSurfaceFireWindSpeedFlag" );
    vSurfaceFireWindSpeedLimit       = m_eqTree->getVarPtr( "vSurfaceFireWindSpeedLimit" );

    vSurfaceFuelAspenCuring          = m_eqTree->getVarPtr( "vSurfaceFuelAspenCuring" );
    vSurfaceFuelAspenLoadDead1       = m_eqTree->getVarPtr( "vSurfaceFuelAspenLoadDead1" );
    vSurfaceFuelAspenLoadDead10      = m_eqTree->getVarPtr( "vSurfaceFuelAspenLoadDead10" );
    vSurfaceFuelAspenLoadLiveHerb    = m_eqTree->getVarPtr( "vSurfaceFuelAspenLoadLiveHerb" );
    vSurfaceFuelAspenLoadLiveWoody   = m_eqTree->getVarPtr( "vSurfaceFuelAspenLoadLiveWoody" );
    vSurfaceFuelAspenSavrDead1       = m_eqTree->getVarPtr( "vSurfaceFuelAspenSavrDead1" );
    vSurfaceFuelAspenSavrDead10      = m_eqTree->getVarPtr( "vSurfaceFuelAspenSavrDead10" );
    vSurfaceFuelAspenSavrLiveHerb    = m_eqTree->getVarPtr( "vSurfaceFuelAspenSavrLiveHerb" );
    vSurfaceFuelAspenSavrLiveWoody   = m_eqTree->getVarPtr( "vSurfaceFuelAspenSavrLiveWoody" );
    vSurfaceFuelAspenType            = m_eqTree->getVarPtr( "vSurfaceFuelAspenType" );

    vSurfaceFuelBedBetaRatio    = m_eqTree->getVarPtr( "vSurfaceFuelBedBetaRatio" );
    vSurfaceFuelBedBulkDensity  = m_eqTree->getVarPtr( "vSurfaceFuelBedBulkDensity" );
    vSurfaceFuelBedCoverage1    = m_eqTree->getVarPtr( "vSurfaceFuelBedCoverage1" );
    vSurfaceFuelBedDeadFraction = m_eqTree->getVarPtr( "vSurfaceFuelBedDeadFraction" );
    vSurfaceFuelBedLiveFraction = m_eqTree->getVarPtr( "vSurfaceFuelBedLiveFraction" );
    vSurfaceFuelBedDepth        = m_eqTree->getVarPtr( "vSurfaceFuelBedDepth" );
    vSurfaceFuelBedHeatSink     = m_eqTree->getVarPtr( "vSurfaceFuelBedHeatSink" );
    vSurfaceFuelBedMextDead     = m_eqTree->getVarPtr( "vSurfaceFuelBedMextDead" );
    vSurfaceFuelBedMextLive     = m_eqTree->getVarPtr( "vSurfaceFuelBedMextLive" );
    vSurfaceFuelBedModel        = m_eqTree->getVarPtr( "vSurfaceFuelBedModel" );
    vSurfaceFuelBedModel1       = m_eqTree->getVarPtr( "vSurfaceFuelBedModel1" );
    vSurfaceFuelBedModel2       = m_eqTree->getVarPtr( "vSurfaceFuelBedModel2" );
    vSurfaceFuelBedMoisDead     = m_eqTree->getVarPtr( "vSurfaceFuelBedMoisDead" );
    vSurfaceFuelBedMoisLive     = m_eqTree->getVarPtr( "vSurfaceFuelBedMoisLive" );
    vSurfaceFuelBedPackingRatio = m_eqTree->getVarPtr( "vSurfaceFuelBedPackingRatio" );
    vSurfaceFuelBedSigma        = m_eqTree->getVarPtr( "vSurfaceFuelBedSigma" );

    vSurfaceFuelDens[0] = m_eqTree->getVarPtr( "vSurfaceFuelDens0" );
    vSurfaceFuelDens[1] = m_eqTree->getVarPtr( "vSurfaceFuelDens1" );
    vSurfaceFuelDens[2] = m_eqTree->getVarPtr( "vSurfaceFuelDens2" );
    vSurfaceFuelDens[3] = m_eqTree->getVarPtr( "vSurfaceFuelDens3" );
    vSurfaceFuelDens[4] = m_eqTree->getVarPtr( "vSurfaceFuelDens4" );
    vSurfaceFuelDens[5] = m_eqTree->getVarPtr( "vSurfaceFuelDens5" );
    vSurfaceFuelDens[6] = m_eqTree->getVarPtr( "vSurfaceFuelDens6" );
    vSurfaceFuelDens[7] = m_eqTree->getVarPtr( "vSurfaceFuelDens7" );

    vSurfaceFuelHeat[0] = m_eqTree->getVarPtr( "vSurfaceFuelHeat0" );
    vSurfaceFuelHeat[1] = m_eqTree->getVarPtr( "vSurfaceFuelHeat1" );
    vSurfaceFuelHeat[2] = m_eqTree->getVarPtr( "vSurfaceFuelHeat2" );
    vSurfaceFuelHeat[3] = m_eqTree->getVarPtr( "vSurfaceFuelHeat3" );
    vSurfaceFuelHeat[4] = m_eqTree->getVarPtr( "vSurfaceFuelHeat4" );
    vSurfaceFuelHeat[5] = m_eqTree->getVarPtr( "vSurfaceFuelHeat5" );
    vSurfaceFuelHeat[6] = m_eqTree->getVarPtr( "vSurfaceFuelHeat6" );
    vSurfaceFuelHeat[7] = m_eqTree->getVarPtr( "vSurfaceFuelHeat7" );
    vSurfaceFuelHeatDead = m_eqTree->getVarPtr( "vSurfaceFuelHeatDead" );
    vSurfaceFuelHeatLive = m_eqTree->getVarPtr( "vSurfaceFuelHeatLive" );

    vSurfaceFuelLife[0] = m_eqTree->getVarPtr( "vSurfaceFuelLife0" );
    vSurfaceFuelLife[1] = m_eqTree->getVarPtr( "vSurfaceFuelLife1" );
    vSurfaceFuelLife[2] = m_eqTree->getVarPtr( "vSurfaceFuelLife2" );
    vSurfaceFuelLife[3] = m_eqTree->getVarPtr( "vSurfaceFuelLife3" );
    vSurfaceFuelLife[4] = m_eqTree->getVarPtr( "vSurfaceFuelLife4" );
    vSurfaceFuelLife[5] = m_eqTree->getVarPtr( "vSurfaceFuelLife5" );
    vSurfaceFuelLife[6] = m_eqTree->getVarPtr( "vSurfaceFuelLife6" );
    vSurfaceFuelLife[7] = m_eqTree->getVarPtr( "vSurfaceFuelLife7" );

    vSurfaceFuelLoad[0] = m_eqTree->getVarPtr( "vSurfaceFuelLoad0" );
    vSurfaceFuelLoad[1] = m_eqTree->getVarPtr( "vSurfaceFuelLoad1" );
    vSurfaceFuelLoad[2] = m_eqTree->getVarPtr( "vSurfaceFuelLoad2" );
    vSurfaceFuelLoad[3] = m_eqTree->getVarPtr( "vSurfaceFuelLoad3" );
    vSurfaceFuelLoad[4] = m_eqTree->getVarPtr( "vSurfaceFuelLoad4" );
    vSurfaceFuelLoad[5] = m_eqTree->getVarPtr( "vSurfaceFuelLoad5" );
    vSurfaceFuelLoad[6] = m_eqTree->getVarPtr( "vSurfaceFuelLoad6" );
    vSurfaceFuelLoad[7] = m_eqTree->getVarPtr( "vSurfaceFuelLoad7" );

    vSurfaceFuelLoadDead       = m_eqTree->getVarPtr( "vSurfaceFuelLoadDead" );
    vSurfaceFuelLoadDead1      = m_eqTree->getVarPtr( "vSurfaceFuelLoadDead1" );
    vSurfaceFuelLoadDead10     = m_eqTree->getVarPtr( "vSurfaceFuelLoadDead10" );
    vSurfaceFuelLoadDead100    = m_eqTree->getVarPtr( "vSurfaceFuelLoadDead100" );
    vSurfaceFuelLoadDeadHerb   = m_eqTree->getVarPtr( "vSurfaceFuelLoadDeadHerb" );
    vSurfaceFuelLoadLive       = m_eqTree->getVarPtr( "vSurfaceFuelLoadLive" );
    vSurfaceFuelLoadLiveHerb   = m_eqTree->getVarPtr( "vSurfaceFuelLoadLiveHerb" );
    vSurfaceFuelLoadLiveWood   = m_eqTree->getVarPtr( "vSurfaceFuelLoadLiveWood" );
    vSurfaceFuelLoadTransferEq = m_eqTree->getVarPtr( "vSurfaceFuelLoadTransferEq" );
    vSurfaceFuelLoadTransferFraction = m_eqTree->getVarPtr( "vSurfaceFuelLoadTransferFraction" );
    vSurfaceFuelLoadUndeadHerb = m_eqTree->getVarPtr( "vSurfaceFuelLoadUndeadHerb" );

    vSurfaceFuelMois[0] = m_eqTree->getVarPtr( "vSurfaceFuelMois0" );
    vSurfaceFuelMois[1] = m_eqTree->getVarPtr( "vSurfaceFuelMois1" );
    vSurfaceFuelMois[2] = m_eqTree->getVarPtr( "vSurfaceFuelMois2" );
    vSurfaceFuelMois[3] = m_eqTree->getVarPtr( "vSurfaceFuelMois3" );
    vSurfaceFuelMois[4] = m_eqTree->getVarPtr( "vSurfaceFuelMois4" );
    vSurfaceFuelMois[5] = m_eqTree->getVarPtr( "vSurfaceFuelMois5" );
    vSurfaceFuelMois[6] = m_eqTree->getVarPtr( "vSurfaceFuelMois6" );
    vSurfaceFuelMois[7] = m_eqTree->getVarPtr( "vSurfaceFuelMois7" );

    vSurfaceFuelMoisDead1    = m_eqTree->getVarPtr( "vSurfaceFuelMoisDead1" );
    vSurfaceFuelMoisDead10   = m_eqTree->getVarPtr( "vSurfaceFuelMoisDead10" );
    vSurfaceFuelMoisDead100  = m_eqTree->getVarPtr( "vSurfaceFuelMoisDead100" );
    vSurfaceFuelMoisDead1000 = m_eqTree->getVarPtr( "vSurfaceFuelMoisDead1000" );
    vSurfaceFuelMoisLifeDead = m_eqTree->getVarPtr( "vSurfaceFuelMoisLifeDead" );
    vSurfaceFuelMoisLifeLive = m_eqTree->getVarPtr( "vSurfaceFuelMoisLifeLive" );
    vSurfaceFuelMoisLiveHerb = m_eqTree->getVarPtr( "vSurfaceFuelMoisLiveHerb" );
    vSurfaceFuelMoisLiveWood = m_eqTree->getVarPtr( "vSurfaceFuelMoisLiveWood" );
    vSurfaceFuelMoisScenario = m_eqTree->getVarPtr( "vSurfaceFuelMoisScenario" );

    vSurfaceFuelPalmettoAge             = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoAge" );
    vSurfaceFuelPalmettoCover           = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoCover" );
    vSurfaceFuelPalmettoHeight          = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoHeight" );
    vSurfaceFuelPalmettoLoadDead1       = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadDead1" );
    vSurfaceFuelPalmettoLoadDead10      = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadDead10" );
    vSurfaceFuelPalmettoLoadDeadFoliage = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadDeadFoliage" );
    vSurfaceFuelPalmettoLoadLitter      = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadLitter" );
    vSurfaceFuelPalmettoLoadLive1       = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadLive1" );
    vSurfaceFuelPalmettoLoadLive10      = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadLive10" );
    vSurfaceFuelPalmettoLoadLiveFoliage = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoLoadLiveFoliage" );
    vSurfaceFuelPalmettoOverstoryBasalArea = m_eqTree->getVarPtr( "vSurfaceFuelPalmettoOverstoryBasalArea" );

    vSurfaceFuelSavr[0] = m_eqTree->getVarPtr( "vSurfaceFuelSavr0" );
    vSurfaceFuelSavr[1] = m_eqTree->getVarPtr( "vSurfaceFuelSavr1" );
    vSurfaceFuelSavr[2] = m_eqTree->getVarPtr( "vSurfaceFuelSavr2" );
    vSurfaceFuelSavr[3] = m_eqTree->getVarPtr( "vSurfaceFuelSavr3" );
    vSurfaceFuelSavr[4] = m_eqTree->getVarPtr( "vSurfaceFuelSavr4" );
    vSurfaceFuelSavr[5] = m_eqTree->getVarPtr( "vSurfaceFuelSavr5" );
    vSurfaceFuelSavr[6] = m_eqTree->getVarPtr( "vSurfaceFuelSavr6" );
    vSurfaceFuelSavr[7] = m_eqTree->getVarPtr( "vSurfaceFuelSavr7" );

    vSurfaceFuelSavrDead1    = m_eqTree->getVarPtr( "vSurfaceFuelSavrDead1" );
    vSurfaceFuelSavrLiveHerb = m_eqTree->getVarPtr( "vSurfaceFuelSavrLiveHerb" );
    vSurfaceFuelSavrLiveWood = m_eqTree->getVarPtr( "vSurfaceFuelSavrLiveWood" );

    vSurfaceFuelSeff[0] = m_eqTree->getVarPtr( "vSurfaceFuelSeff0" );
    vSurfaceFuelSeff[1] = m_eqTree->getVarPtr( "vSurfaceFuelSeff1" );
    vSurfaceFuelSeff[2] = m_eqTree->getVarPtr( "vSurfaceFuelSeff2" );
    vSurfaceFuelSeff[3] = m_eqTree->getVarPtr( "vSurfaceFuelSeff3" );
    vSurfaceFuelSeff[4] = m_eqTree->getVarPtr( "vSurfaceFuelSeff4" );
    vSurfaceFuelSeff[5] = m_eqTree->getVarPtr( "vSurfaceFuelSeff5" );
    vSurfaceFuelSeff[6] = m_eqTree->getVarPtr( "vSurfaceFuelSeff6" );
    vSurfaceFuelSeff[7] = m_eqTree->getVarPtr( "vSurfaceFuelSeff7" );
    vSurfaceFuelStot[0] = m_eqTree->getVarPtr( "vSurfaceFuelStot0" );
    vSurfaceFuelStot[1] = m_eqTree->getVarPtr( "vSurfaceFuelStot1" );
    vSurfaceFuelStot[2] = m_eqTree->getVarPtr( "vSurfaceFuelStot2" );
    vSurfaceFuelStot[3] = m_eqTree->getVarPtr( "vSurfaceFuelStot3" );
    vSurfaceFuelStot[4] = m_eqTree->getVarPtr( "vSurfaceFuelStot4" );
    vSurfaceFuelStot[5] = m_eqTree->getVarPtr( "vSurfaceFuelStot5" );
    vSurfaceFuelStot[6] = m_eqTree->getVarPtr( "vSurfaceFuelStot6" );
    vSurfaceFuelStot[7] = m_eqTree->getVarPtr( "vSurfaceFuelStot7" );
    vSurfaceFuelTemp    = m_eqTree->getVarPtr( "vSurfaceFuelTemp" );

    vTimeIntegerDate = m_eqTree->getVarPtr( "vTimeIntegerDate" );
    vTimeJulianDate  = m_eqTree->getVarPtr( "vTimeJulianDate" );

    vTreeBarkThickness            = m_eqTree->getVarPtr( "vTreeBarkThickness" );
    vTreeCanopyBulkDens           = m_eqTree->getVarPtr( "vTreeCanopyBulkDens" );
    vTreeCanopyCrownFraction      = m_eqTree->getVarPtr( "vTreeCanopyCrownFraction" );
    vTreeCanopyCover              = m_eqTree->getVarPtr( "vTreeCanopyCover" );
    vTreeCount                    = m_eqTree->getVarPtr( "vTreeCount" );
    vTreeCoverHt                  = m_eqTree->getVarPtr( "vTreeCoverHt" );
    vTreeCoverHtDownwind          = m_eqTree->getVarPtr( "vTreeCoverHtDownwind" );
    vTreeCrownBaseHt              = m_eqTree->getVarPtr( "vTreeCrownBaseHt" );
    vTreeCrownLengFractionScorchedAtVector = m_eqTree->getVarPtr( "vTreeCrownLengFractionScorchedAtVector" );
    vTreeCrownLengScorchedAtVector= m_eqTree->getVarPtr( "vTreeCrownLengScorchedAtVector" );
    vTreeCrownRatio               = m_eqTree->getVarPtr( "vTreeCrownRatio" );
    vTreeCrownVolScorchedAtVector = m_eqTree->getVarPtr( "vTreeCrownVolScorchedAtVector" );
    vTreeDbh                      = m_eqTree->getVarPtr( "vTreeDbh" );
    vTreeFoliarMois               = m_eqTree->getVarPtr( "vTreeFoliarMois" );
    vTreeHt                       = m_eqTree->getVarPtr( "vTreeHt" );
    vTreeMortalityCountAtVector   = m_eqTree->getVarPtr( "vTreeMortalityCountAtVector" );
    vTreeMortalityRateAspenAtVector = m_eqTree->getVarPtr( "vTreeMortalityRateAspenAtVector" );
    vTreeMortalityRateAtVector    = m_eqTree->getVarPtr( "vTreeMortalityRateAtVector" );
    vTreeSpecies                  = m_eqTree->getVarPtr( "vTreeSpecies" );
    vTreeSpeciesMortality         = m_eqTree->getVarPtr( "vTreeSpeciesMortality" );
    vTreeSpeciesSpot              = m_eqTree->getVarPtr( "vTreeSpeciesSpot" );

    vWindAdjFactor       = m_eqTree->getVarPtr( "vWindAdjFactor" );
    vWindAdjMethod       = m_eqTree->getVarPtr( "vWindAdjMethod" );
    vWindDirFromCompass  = m_eqTree->getVarPtr( "vWindDirFromCompass" );
    vWindDirFromNorth    = m_eqTree->getVarPtr( "vWindDirFromNorth" );
    vWindDirFromUpslope  = m_eqTree->getVarPtr( "vWindDirFromUpslope" );
    vWindSpeedAt10M      = m_eqTree->getVarPtr( "vWindSpeedAt10M" );
    vWindSpeedAt20Ft     = m_eqTree->getVarPtr( "vWindSpeedAt20Ft" );
    vWindSpeedAtMidflame = m_eqTree->getVarPtr( "vWindSpeedAtMidflame" );

    vWthrAirTemp             = m_eqTree->getVarPtr( "vWthrAirTemp" );
    vWthrCumulusBaseHt       = m_eqTree->getVarPtr( "vWthrCumulusBaseHt" );
    vWthrDewPointTemp        = m_eqTree->getVarPtr( "vWthrDewPointTemp" );
    vWthrHeatIndex           = m_eqTree->getVarPtr( "vWthrHeatIndex" );
    vWthrLightningStrikeType = m_eqTree->getVarPtr( "vWthrLightningStrikeType" );
    vWthrRelativeHumidity    = m_eqTree->getVarPtr( "vWthrRelativeHumidity" );
    vWthrSummerSimmerIndex   = m_eqTree->getVarPtr( "vWthrSummerSimmerIndex" );
    vWthrWetBulbTemp         = m_eqTree->getVarPtr( "vWthrWetBulbTemp" );
    vWthrWindChillTemp       = m_eqTree->getVarPtr( "vWthrWindChillTemp" );

    fContainFF = m_eqTree->setEqFunAddress( "fContainFF", &EqCalc::ContainFF );
    fContainFFReportRatio = m_eqTree->setEqFunAddress( "fContainFFReportRatio", &EqCalc::ContainFFReportRatio );
    fContainFFReportSize = m_eqTree->setEqFunAddress( "fContainFFReportSize", &EqCalc::ContainFFReportSize );
    fContainFFReportSpread = m_eqTree->setEqFunAddress( "fContainFFReportSpread", &EqCalc::ContainFFReportSpread );
    fContainFFSingle = m_eqTree->setEqFunAddress( "fContainFFSingle", &EqCalc::ContainFFSingle );

    fCrownFireActiveCrown = m_eqTree->setEqFunAddress( "fCrownFireActiveCrown", &EqCalc::CrownFireActiveCrown );
    fCrownFireActiveRatio = m_eqTree->setEqFunAddress( "fCrownFireActiveRatio", &EqCalc::CrownFireActiveRatio );
    fCrownFireArea = m_eqTree->setEqFunAddress( "fCrownFireArea", &EqCalc::CrownFireArea );
    fCrownFireCritCrownSpreadRate = m_eqTree->setEqFunAddress( "fCrownFireCritCrownSpreadRate", &EqCalc::CrownFireCritCrownSpreadRate );
    fCrownFireCritSurfFireInt = m_eqTree->setEqFunAddress( "fCrownFireCritSurfFireInt", &EqCalc::CrownFireCritSurfFireInt );
    fCrownFireCritSurfFlameLeng = m_eqTree->setEqFunAddress( "fCrownFireCritSurfFlameLeng", &EqCalc::CrownFireCritSurfFlameLeng );
    fCrownFireFlameLeng = m_eqTree->setEqFunAddress( "fCrownFireFlameLeng", &EqCalc::CrownFireFlameLength );
    fCrownFireFuelLoad = m_eqTree->setEqFunAddress( "fCrownFireFuelLoad", &EqCalc::CrownFireFuelLoad );
    fCrownFireHeatPerUnitArea = m_eqTree->setEqFunAddress( "fCrownFireHeatPerUnitArea", &EqCalc::CrownFireHeatPerUnitArea );
    fCrownFireHeatPerUnitAreaCanopy = m_eqTree->setEqFunAddress( "fCrownFireHeatPerUnitAreaCanopy", &EqCalc::CrownFireHeatPerUnitAreaCanopy );
    fCrownFireLengthToWidth = m_eqTree->setEqFunAddress( "fCrownFireLengthToWidth", &EqCalc::CrownFireLengthToWidth );
    fCrownFireLineInt = m_eqTree->setEqFunAddress( "fCrownFireLineInt", &EqCalc::CrownFireLineIntensity );
    fCrownFirePerimeter = m_eqTree->setEqFunAddress( "fCrownFirePerimeter", &EqCalc::CrownFirePerimeter );
    fCrownFirePowerOfFire = m_eqTree->setEqFunAddress( "fCrownFirePowerOfFire", &EqCalc::CrownFirePowerOfFire );
    fCrownFirePowerOfWind = m_eqTree->setEqFunAddress( "fCrownFirePowerOfWind", &EqCalc::CrownFirePowerOfWind );
    fCrownFirePowerRatio = m_eqTree->setEqFunAddress( "fCrownFirePowerRatio", &EqCalc::CrownFirePowerRatio );
    fCrownFireSpreadDist = m_eqTree->setEqFunAddress( "fCrownFireSpreadDist", &EqCalc::CrownFireSpreadDist );
    fCrownFireSpreadMapDist = m_eqTree->setEqFunAddress( "fCrownFireSpreadMapDist", &EqCalc::CrownFireSpreadMapDist );
    fCrownFireSpreadRate = m_eqTree->setEqFunAddress( "fCrownFireSpreadRate", &EqCalc::CrownFireSpreadRate );
    fCrownFireTransRatioFromFireIntAtVector = m_eqTree->setEqFunAddress( "fCrownFireTransRatioFromFireIntAtVector", &EqCalc::CrownFireTransRatioFromFireIntAtVector );
    fCrownFireTransRatioFromFlameLengAtVector = m_eqTree->setEqFunAddress( "fCrownFireTransRatioFromFlameLengAtVector", &EqCalc::CrownFireTransRatioFromFlameLengAtVector );
    fCrownFireTransToCrown = m_eqTree->setEqFunAddress( "fCrownFireTransToCrown", &EqCalc::CrownFireTransToCrown );
    fCrownFireType = m_eqTree->setEqFunAddress( "fCrownFireType", &EqCalc::CrownFireType );
    fCrownFireWindDriven = m_eqTree->setEqFunAddress( "fCrownFireWindDriven", &EqCalc::CrownFireWindDriven );

    fIgnitionFirebrandFuelMoisFromDead1Hr = m_eqTree->setEqFunAddress( "fIgnitionFirebrandFuelMoisFromDead1Hr", &EqCalc::IgnitionFirebrandFuelMoisFromDead1Hr );
    fIgnitionFirebrandProb = m_eqTree->setEqFunAddress( "fIgnitionFirebrandProb", &EqCalc::IgnitionFirebrandProb );
    fIgnitionLightningFuelMoisFromDead100Hr = m_eqTree->setEqFunAddress( "fIgnitionLightningFuelMoisFromDead100Hr", &EqCalc::IgnitionLightningFuelMoisFromDead100Hr );
    fIgnitionLightningProb = m_eqTree->setEqFunAddress( "fIgnitionLightningProb", &EqCalc::IgnitionLightningProb );

    fMapScale = m_eqTree->setEqFunAddress( "fMapScale", &EqCalc::MapScale );
    fMapSlope = m_eqTree->setEqFunAddress( "fMapSlope",  &EqCalc::MapSlope );

    fSafetyZoneRadius = m_eqTree->setEqFunAddress( "fSafetyZoneRadius",  &EqCalc::SafetyZoneRadius );
    fSafetyZoneSepDist = m_eqTree->setEqFunAddress( "fSafetyZoneSepDist",  &EqCalc::SafetyZoneSepDist );

    fSiteAspectDirFromNorth = m_eqTree->setEqFunAddress( "fSiteAspectDirFromNorth",  &EqCalc::SiteAspectDirFromNorth );
    fSiteRidgeToValleyDist = m_eqTree->setEqFunAddress( "fSiteRidgeToValleyDist",  &EqCalc::SiteRidgeToValleyDist );
    fSiteSlopeFraction = m_eqTree->setEqFunAddress( "fSiteSlopeFraction", &EqCalc::SiteSlopeFraction );
    fSiteUpslopeDirFromNorth = m_eqTree->setEqFunAddress( "fSiteUpslopeDirFromNorth", &EqCalc::SiteUpslopeDirFromNorth );

    fSpotDistBurningPile = m_eqTree->setEqFunAddress( "fSpotDistBurningPile", &EqCalc::SpotDistBurningPile );
    fSpotDistSurfaceFire = m_eqTree->setEqFunAddress( "fSpotDistSurfaceFire", &EqCalc::SpotDistSurfaceFire );
    fSpotDistTorchingTrees = m_eqTree->setEqFunAddress( "fSpotDistTorchingTrees", &EqCalc::SpotDistTorchingTrees );
    fSpotMapDistBurningPile = m_eqTree->setEqFunAddress( "fSpotMapDistBurningPile", &EqCalc::SpotMapDistBurningPile );
    fSpotMapDistSurfaceFire = m_eqTree->setEqFunAddress( "fSpotMapDistSurfaceFire", &EqCalc::SpotMapDistSurfaceFire );
    fSpotMapDistTorchingTrees = m_eqTree->setEqFunAddress( "fSpotMapDistTorchingTrees", &EqCalc::SpotMapDistTorchingTrees );

    fSurfaceFireArea = m_eqTree->setEqFunAddress( "fSurfaceFireArea", &EqCalc::FireArea );
    fSurfaceFireCharacteristicsDiagram = m_eqTree->setEqFunAddress( "fSurfaceFireCharacteristicsDiagram", &EqCalc::FireCharacteristicsDiagram );
    fSurfaceFireDistAtBack = m_eqTree->setEqFunAddress( "fSurfaceFireDistAtBack", &EqCalc::FireDistAtBack );
    fSurfaceFireDistAtHead = m_eqTree->setEqFunAddress( "fSurfaceFireDistAtHead", &EqCalc::FireDistAtHead );
    fSurfaceFireDistAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireDistAtVector", &EqCalc::FireDistAtVector );
    fSurfaceFireEccentricity = m_eqTree->setEqFunAddress( "fSurfaceFireEccentricity", &EqCalc::FireEccentricity );
    fSurfaceFireEffWindAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireEffWindAtVector", &EqCalc::FireEffWindAtVector );
    fSurfaceFireFlameHtAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireFlameHtAtVector", &EqCalc::FireFlameHtAtVector );
    fSurfaceFireFlameLengAtHead = m_eqTree->setEqFunAddress( "fSurfaceFireFlameLengAtHead", &EqCalc::FireFlameLengAtHead );
    fSurfaceFireFlameLengAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireFlameLengAtVector", &EqCalc::FireFlameLengAtVector );
    fSurfaceFireHeatPerUnitArea = m_eqTree->setEqFunAddress( "fSurfaceFireHeatPerUnitArea", &EqCalc::FireHeatPerUnitArea );
    fSurfaceFireHeatSource = m_eqTree->setEqFunAddress( "fSurfaceFireHeatSource", &EqCalc::FireHeatSource );
    fSurfaceFireLengDist = m_eqTree->setEqFunAddress( "fSurfaceFireLengDist", &EqCalc::FireLengDist );
    fSurfaceFireLengMapDist = m_eqTree->setEqFunAddress( "fSurfaceFireLengMapDist", &EqCalc::FireLengMapDist );
    fSurfaceFireLengthToWidth = m_eqTree->setEqFunAddress( "fSurfaceFireLengthToWidth", &EqCalc::FireLengthToWidth );
    fSurfaceFireLineIntAtHead = m_eqTree->setEqFunAddress( "fSurfaceFireLineIntAtHead", &EqCalc::FireLineIntAtHead );
    fSurfaceFireLineIntAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireLineIntAtVector", &EqCalc::FireLineIntAtVector );
    fSurfaceFireMapDistAtBack = m_eqTree->setEqFunAddress( "fSurfaceFireMapDistAtBack", &EqCalc::FireMapDistAtBack );
    fSurfaceFireMapDistAtHead = m_eqTree->setEqFunAddress( "fSurfaceFireMapDistAtHead", &EqCalc::FireMapDistAtHead );
    fSurfaceFireMapDistAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireMapDistAtVector", &EqCalc::FireMapDistAtVector );
    fSurfaceFireMaxDirFromNorth = m_eqTree->setEqFunAddress( "fSurfaceFireMaxDirFromNorth", &EqCalc::FireMaxDirFromNorth );
    fSurfaceFireMaxDirDiagram = m_eqTree->setEqFunAddress( "fSurfaceFireMaxDirDiagram", &EqCalc::FireMaxDirDiagram );
    fSurfaceFireNoWindRate = m_eqTree->setEqFunAddress( "fSurfaceFireNoWindRate", &EqCalc::FireNoWindRate );
    fSurfaceFirePerimeter = m_eqTree->setEqFunAddress( "fSurfaceFirePerimeter", &EqCalc::FirePerimeter );
    fSurfaceFirePropagatingFlux = m_eqTree->setEqFunAddress( "fSurfaceFirePropagatingFlux", &EqCalc::FirePropagatingFlux );
    fSurfaceFireReactionInt = m_eqTree->setEqFunAddress( "fSurfaceFireReactionInt", &EqCalc::FireReactionInt );
    fSurfaceFireResidenceTime = m_eqTree->setEqFunAddress( "fSurfaceFireResidenceTime", &EqCalc::FireResidenceTime );
    fSurfaceFireScorchHtFromFliAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireScorchHtFromFliAtVector", &EqCalc::FireScorchHtFromFliAtVector );
    fSurfaceFireScorchHtFromFlameLengAtVector = m_eqTree->setEqFunAddress( "fSurfaceFireScorchHtFromFlameLengAtVector", &EqCalc::FireScorchHtFromFlameLengAtVector );
    fSurfaceFireShapeDiagram = m_eqTree->setEqFunAddress( "fSurfaceFireShapeDiagram", &EqCalc::FireShapeDiagram );
    fSurfaceFireSpreadAtBack = m_eqTree->setEqFunAddress( "fSurfaceFireSpreadAtBack", &EqCalc::FireSpreadAtBack );
    fSurfaceFireSpreadAtBeta = m_eqTree->setEqFunAddress( "fSurfaceFireSpreadAtBeta", &EqCalc::FireSpreadAtBeta );
    fSurfaceFireSpreadAtHead = m_eqTree->setEqFunAddress( "fSurfaceFireSpreadAtHead", &EqCalc::FireSpreadAtHead );
    fSurfaceFireVectorBeta = m_eqTree->setEqFunAddress( "fSurfaceFireVectorBeta", &EqCalc::FireVectorBeta );
    fSurfaceFireVectorDirFromNorth = m_eqTree->setEqFunAddress( "fSurfaceFireVectorDirFromNorth", &EqCalc::FireVectorDirFromNorth );
    fSurfaceFireVectorDirFromUpslope = m_eqTree->setEqFunAddress( "fSurfaceFireVectorDirFromUpslope", &EqCalc::FireVectorDirFromUpslope );
    fSurfaceFireWidthDist = m_eqTree->setEqFunAddress( "fSurfaceFireWidthDist", &EqCalc::FireWidthDist );
    fSurfaceFireWidthMapDist = m_eqTree->setEqFunAddress( "fSurfaceFireWidthMapDist", &EqCalc::FireWidthMapDist );

    fSurfaceFuelAspenModel = m_eqTree->setEqFunAddress( "fSurfaceFuelAspenModel", &EqCalc::FuelAspenModel );
    fSurfaceFuelAspenParms = m_eqTree->setEqFunAddress( "fSurfaceFuelAspenParms", &EqCalc::FuelAspenParms );
    fSurfaceFuelBedHeatSink = m_eqTree->setEqFunAddress( "fSurfaceFuelBedHeatSink", &EqCalc::FuelBedHeatSink );
    fSurfaceFuelBedIntermediates = m_eqTree->setEqFunAddress( "fSurfaceFuelBedIntermediates", &EqCalc::FuelBedIntermediates );
    fSurfaceFuelBedModel = m_eqTree->setEqFunAddress( "fSurfaceFuelBedModel", &EqCalc::FuelBedModel );
    fSurfaceFuelBedParms = m_eqTree->setEqFunAddress( "fSurfaceFuelBedParms", &EqCalc::FuelBedParms );
    fSurfaceFuelBedWeighted = m_eqTree->setEqFunAddress( "fSurfaceFuelBedWeighted", &EqCalc::FuelBedWeighted );
    fSurfaceFuelLoadTransferFraction = m_eqTree->setEqFunAddress( "fSurfaceFuelLoadTransferFraction", &EqCalc::FuelLoadTransferFraction );
    fSurfaceFuelMoisLifeClass = m_eqTree->setEqFunAddress( "fSurfaceFuelMoisLifeClass", &EqCalc::FuelMoisLifeClass );
    fSurfaceFuelMoisScenarioModel = m_eqTree->setEqFunAddress( "fSurfaceFuelMoisScenarioModel", &EqCalc::FuelMoisScenarioModel );
    fSurfaceFuelMoisTimeLag = m_eqTree->setEqFunAddress( "fSurfaceFuelMoisTimeLag", &EqCalc::FuelMoisTimeLag );
    fSurfaceFuelPalmettoModel = m_eqTree->setEqFunAddress( "fSurfaceFuelPalmettoModel", &EqCalc::FuelPalmettoModel );
    fSurfaceFuelPalmettoParms = m_eqTree->setEqFunAddress( "fSurfaceFuelPalmettoParms", &EqCalc::FuelPalmettoParms );
    fSurfaceFuelTemp = m_eqTree->setEqFunAddress( "fSurfaceFuelTemp", &EqCalc::FuelTemp );

    fTimeJulianDate = m_eqTree->setEqFunAddress( "fTimeJulianDate", &EqCalc::TimeJulianDate );

    fTreeBarkThicknessFofem = m_eqTree->setEqFunAddress( "fTreeBarkThicknessFofem", &EqCalc::TreeBarkThicknessFofem );
    fTreeCrownBaseHt = m_eqTree->setEqFunAddress( "fTreeCrownBaseHt", &EqCalc::TreeCrownBaseHt );
    fTreeCrownRatio  = m_eqTree->setEqFunAddress( "fTreeCrownRatio", &EqCalc::TreeCrownRatio );
    fTreeCrownVolScorchedAtVector = m_eqTree->setEqFunAddress( "fTreeCrownVolScorchedAtVector", &EqCalc::TreeCrownVolScorchedAtVector );
    fTreeMortalityCountAtVector = m_eqTree->setEqFunAddress( "fTreeMortalityCountAtVector", &EqCalc::TreeMortalityCountAtVector );
    fTreeMortalityRateAspenAtVector = m_eqTree->setEqFunAddress( "fTreeMortalityRateAspenAtVector", &EqCalc::TreeMortalityRateAspenAtVector );
    fTreeMortalityRateFofemAtVector = m_eqTree->setEqFunAddress( "fTreeMortalityRateFofemAtVector", &EqCalc::TreeMortalityRateFofemAtVector );
    fTreeMortalityRateFofem2AtVector = m_eqTree->setEqFunAddress( "fTreeMortalityRateFofem2AtVector", &EqCalc::TreeMortalityRateFofem2AtVector );
    fTreeMortalityRateFofemHoodAtVector = m_eqTree->setEqFunAddress( "fTreeMortalityRateFofemHoodAtVector", &EqCalc::TreeMortalityRateFofemHoodAtVector );

    fWindAdjFactor = m_eqTree->setEqFunAddress( "fWindAdjFactor", &EqCalc::WindAdjFactor );
    fWindSpeedAt20Ft = m_eqTree->setEqFunAddress( "fWindSpeedAt20Ft", &EqCalc::WindSpeedAt20Ft );
    fWindSpeedAtMidflame = m_eqTree->setEqFunAddress( "fWindSpeedAtMidflame", &EqCalc::WindSpeedAtMidflame );
    fWindDirFromNorth = m_eqTree->setEqFunAddress( "fWindDirFromNorth", &EqCalc::WindDirFromNorth );
    fWindDirFromUpslope = m_eqTree->setEqFunAddress( "fWindDirFromUpslope", &EqCalc::WindDirFromUpslope );

    fWthrCumulusBaseHt = m_eqTree->setEqFunAddress( "fWthrCumulusBaseHt", &EqCalc::WthrCumulusBaseHt );
    fWthrDewPointTemp = m_eqTree->setEqFunAddress( "fWthrDewPointTemp", &EqCalc::WthrDewPointTemp );
    fWthrHeatIndex = m_eqTree->setEqFunAddress( "fWthrHeatIndex", &EqCalc::WthrHeatIndex );
    fWthrRelativeHumidity = m_eqTree->setEqFunAddress( "fWthrRelativeHumidity", &EqCalc::WthrRelativeHumidity );
    fWthrSummerSimmerIndex = m_eqTree->setEqFunAddress( "fWthrSummerSimmerIndex", &EqCalc::WthrSummerSimmerIndex );
    fWthrWindChillTemp = m_eqTree->setEqFunAddress( "fWthrWindChillTemp", &EqCalc::WthrWindChillTemp );
    return;
}

//------------------------------------------------------------------------------
//  End of xeqcalc.cpp
//------------------------------------------------------------------------------