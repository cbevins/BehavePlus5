//------------------------------------------------------------------------------
/*! \file xeqcalc.h
 *  \version BehavePlus4
 *  \author Copyright (C) 2002-2009 by Collin D. Bevins.  All rights reserved.
 *
 *  \brief EqTree calculator interface.
 */

#ifndef _XEQCALC_H_
/*! \def _XEQCALC_H_
    \brief Prevent redundant includes.
 */
#define _XEQCALC_H_ 1

// Custom class references
class BpDocument;
class EqFun;
class EqTree;
class EqVar;
class FuelModel;
class PropertyDict;

// Qt include files
#include <qstring.h>

// Standard include files
#include <stdio.h>

//------------------------------------------------------------------------------
/*! \class EqCalc xeqcalc.h
 *
 *  \brief The EqCalc Class provides the interface between the generic EqTree
 *  and a specific implementation.  All variables and functions needed by the
 *  implementation are contained in this class.
 */

class EqCalc
{
// Public methods
public:
    EqCalc( EqTree *eqTree ) ;
    bool conflict1( void ) const ;
    bool conflict2( void ) const ;
    FuelModel *currentFuelModel( int id ) ;
    QString &docDescriptionStore( void ) const ;
    QString &docDescriptionStore( const QString &newStore ) ;
    void initFuelInputs( FuelModel *fmPtr ) ;
    bool isFuelModelVariable( EqVar *varPtr ) const ;
    bool isMoisScenarioVariable( EqVar *varPtr ) const ;
    void maskInputs( void ) ;
    void maskInputs( EqVar *varPtr ) ;
    void reconfigure( int release );
    void reconfigureDocumentation( PropertyDict *prop, int release ) ;
    void reconfigureContainModule( PropertyDict *prop, int release ) ;
    void reconfigureCrownModule( PropertyDict *prop, int release ) ;
    void reconfigureIgnitionModule( PropertyDict *prop, int release ) ;
    void reconfigureMortalityModule( PropertyDict *prop, int release ) ;
    void reconfigureSafetyModule( PropertyDict *prop, int release ) ;
    void reconfigureScorchModule( PropertyDict *prop, int release ) ;
    void reconfigureSizeModule( PropertyDict *prop, int release ) ;
    void reconfigureSpotModule( PropertyDict *prop, int release ) ;
    void reconfigureSurfaceModule( PropertyDict *prop, int release ) ;
    void reconfigureWeatherModule( PropertyDict *prop, int release ) ;
    bool showInitFromFuelModelButton( void ) const ;
    void unmaskCrownInputs( void ) ;
    void unmaskFuelModelInputs( bool twoFuelModels=false ) ;
    void unmaskFuelParmInputs( void ) ;
    void unmaskIgniteInputs( void ) ;
    void unmaskMortalityInputs( void ) ;
    void unmaskSafetyInputs( void ) ;
    void unmaskSpotInputs( void ) ;
    void unmaskWafInputs( void ) ;
    bool validateInputs( void ) const ;

// Public data
public:
    EqTree *m_eqTree;   //!< Pointer to the parent EqTree
    FILE   *m_log;      //!< Log file stream pointer

// Declare all EqVar pointers here.
    EqVar *vContainAttackBack;
    EqVar *vContainAttackDist;
    EqVar *vContainAttackPerimeter;
    EqVar *vContainAttackSize;
    EqVar *vContainAttackHead;
    EqVar *vContainAttackTactic;
    EqVar *vContainCost;
    EqVar *vContainDiagram;
    EqVar *vContainLimitDist;
    EqVar *vContainLine;
    EqVar *vContainPoints;
    EqVar *vContainResourceArrival;
    EqVar *vContainResourceBaseCost;
    EqVar *vContainResourceDuration;
    EqVar *vContainResourceHourCost;
    EqVar *vContainResourceName;
    EqVar *vContainResourceProd;
    EqVar *vContainResourcesUsed;
    EqVar *vContainReportBack;
    EqVar *vContainReportHead;
    EqVar *vContainReportRatio;
    EqVar *vContainReportSize;
    EqVar *vContainReportSpread;
    EqVar *vContainSize;
    EqVar *vContainStatus;
    EqVar *vContainTime;
    EqVar *vContainXMax;
    EqVar *vContainXMin;
    EqVar *vContainYMax;
    EqVar *vCrownFireActiveCrown;
    EqVar *vCrownFireActiveRatio;
    EqVar *vCrownFireArea;
    EqVar *vCrownFireCritCrownSpreadRate;
    EqVar *vCrownFireCritSurfFireInt;
    EqVar *vCrownFireCritSurfFlameLeng;
    EqVar *vCrownFireFlameLeng;
    EqVar *vCrownFireFuelLoad;
    EqVar *vCrownFireHeatPerUnitArea;
    EqVar *vCrownFireHeatPerUnitAreaCanopy;
    EqVar *vCrownFireLengthToWidth;
    EqVar *vCrownFireLineInt;
    EqVar *vCrownFirePerimeter;
    EqVar *vCrownFirePowerOfFire;
    EqVar *vCrownFirePowerOfWind;
    EqVar *vCrownFirePowerRatio;
    EqVar *vCrownFireSpreadDist;
    EqVar *vCrownFireSpreadMapDist;
    EqVar *vCrownFireSpreadRate;
    EqVar *vCrownFireTransRatio;
    EqVar *vCrownFireTransToCrown;
    EqVar *vCrownFireType;
    EqVar *vCrownFireWindDriven;
    EqVar *vDocDescription;
    EqVar *vDocFireAnalyst;
    EqVar *vDocFireName;
    EqVar *vDocFirePeriod;
    EqVar *vDocRxAdminUnit;
    EqVar *vDocRxName;
    EqVar *vDocRxPreparedBy;
    EqVar *vDocTrainingCourse;
    EqVar *vDocTrainingExercise;
    EqVar *vDocTrainingTrainee;
    EqVar *vIgnitionFirebrandFuelMois;
    EqVar *vIgnitionFirebrandProb;
    EqVar *vIgnitionLightningDuffDepth;
    EqVar *vIgnitionLightningFuelMois;
    EqVar *vIgnitionLightningFuelType;
    EqVar *vIgnitionLightningProb;
    EqVar *vMapContourCount;
    EqVar *vMapContourInterval;
    EqVar *vMapDist;
    EqVar *vMapFraction;
    EqVar *vMapScale;
    EqVar *vSafetyZoneEquipmentArea;
    EqVar *vSafetyZoneEquipmentNumber;
    EqVar *vSafetyZonePersonnelArea;
    EqVar *vSafetyZonePersonnelNumber;
    EqVar *vSafetyZoneRadius;
    EqVar *vSafetyZoneSepDist;
    EqVar *vSafetyZoneSize;
    EqVar *vSiteAspectDirFromCompass;
    EqVar *vSiteAspectDirFromNorth;
    EqVar *vSiteElevation;
    EqVar *vSiteLatitude;
    EqVar *vSiteLongitude;
    EqVar *vSiteRidgeToValleyDist;
    EqVar *vSiteRidgeToValleyElev;
    EqVar *vSiteRidgeToValleyMapDist;
    EqVar *vSiteSlopeDegrees;
    EqVar *vSiteSlopeFraction;
    EqVar *vSiteSlopeReach;
    EqVar *vSiteSlopeRise;
    EqVar *vSiteSunShading;
    EqVar *vSiteTimeZone;
    EqVar *vSiteUpslopeDirFromNorth;
    EqVar *vSpotCoverHtBurningPile;
    EqVar *vSpotCoverHtSurfaceFire;
    EqVar *vSpotCoverHtTorchingTrees;
    EqVar *vSpotDistBurningPile;
    EqVar *vSpotDistSurfaceFire;
    EqVar *vSpotDistTorchingTrees;
    EqVar *vSpotFirebrandDriftSurfaceFire;
    EqVar *vSpotFirebrandHtBurningPile;
    EqVar *vSpotFirebrandHtSurfaceFire;
    EqVar *vSpotFirebrandHtTorchingTrees;
    EqVar *vSpotFlameDurTorchingTrees;
    EqVar *vSpotFlameHtTorchingTrees;
    EqVar *vSpotFlameRatioTorchingTrees;
    EqVar *vSpotFlatDistBurningPile;
    EqVar *vSpotFlatDistSurfaceFire;
    EqVar *vSpotFlatDistTorchingTrees;
    EqVar *vSpotFireSource;
    EqVar *vSpotMapDistBurningPile;
    EqVar *vSpotMapDistSurfaceFire;
    EqVar *vSpotMapDistTorchingTrees;
    EqVar *vSpotTorchingTrees;
    EqVar *vSurfaceFireArea;
    EqVar *vSurfaceFireCharacteristicsDiagram;
    EqVar *vSurfaceFireDistAtBack;
    EqVar *vSurfaceFireDistAtHead;
    EqVar *vSurfaceFireDistAtVector;
    EqVar *vSurfaceFireEccentricity;
    EqVar *vSurfaceFireEffWindAtHead;
    EqVar *vSurfaceFireEffWindAtVector;
    EqVar *vSurfaceFireElapsedTime;
    EqVar *vSurfaceFireFlameAngleAtVector;
    EqVar *vSurfaceFireFlameHtAtVector;
    EqVar *vSurfaceFireFlameHtPile;
    EqVar *vSurfaceFireFlameLengAtHead;
    EqVar *vSurfaceFireFlameLengAtVector;
    EqVar *vSurfaceFireHeatPerUnitArea;
    EqVar *vSurfaceFireHeatSource;
    EqVar *vSurfaceFireLengDist;
    EqVar *vSurfaceFireLengMapDist;
    EqVar *vSurfaceFireLengthToWidth;
    EqVar *vSurfaceFireLineIntAtHead;
    EqVar *vSurfaceFireLineIntAtVector;
    EqVar *vSurfaceFireMapDistAtBack;
    EqVar *vSurfaceFireMapDistAtHead;
    EqVar *vSurfaceFireMapDistAtVector;
    EqVar *vSurfaceFireMaxDirDiagram;
    EqVar *vSurfaceFireMaxDirFromNorth;
    EqVar *vSurfaceFireMaxDirFromUpslope;
    EqVar *vSurfaceFireNoWindRate;
    EqVar *vSurfaceFirePerimeter;
    EqVar *vSurfaceFirePropagatingFlux;
    EqVar *vSurfaceFireReactionInt;
    EqVar *vSurfaceFireReactionIntDead;
    EqVar *vSurfaceFireReactionIntLive;
    EqVar *vSurfaceFireResidenceTime;
    EqVar *vSurfaceFireScorchHtAtVector;
    EqVar *vSurfaceFireSeverityAspen;
    EqVar *vSurfaceFireShapeDiagram;
    EqVar *vSurfaceFireSlopeFactor;
    EqVar *vSurfaceFireSpreadAtBack;
    EqVar *vSurfaceFireSpreadAtHead;
    EqVar *vSurfaceFireSpreadAtVector;
    EqVar *vSurfaceFireVectorBeta;
    EqVar *vSurfaceFireVectorDirFromCompass;
    EqVar *vSurfaceFireVectorDirFromNorth;
    EqVar *vSurfaceFireVectorDirFromUpslope;
    EqVar *vSurfaceFireWidthDist;
    EqVar *vSurfaceFireWidthMapDist;
    EqVar *vSurfaceFireWindFactor;
    EqVar *vSurfaceFireWindSpeedFlag;
    EqVar *vSurfaceFireWindSpeedLimit;
    EqVar *vSurfaceFuelAspenCuring;
    EqVar *vSurfaceFuelAspenLoadDead1;
    EqVar *vSurfaceFuelAspenLoadDead10;
    EqVar *vSurfaceFuelAspenLoadLiveHerb;
    EqVar *vSurfaceFuelAspenLoadLiveWoody;
    EqVar *vSurfaceFuelAspenSavrDead1;
    EqVar *vSurfaceFuelAspenSavrDead10;
    EqVar *vSurfaceFuelAspenSavrLiveHerb;
    EqVar *vSurfaceFuelAspenSavrLiveWoody;
    EqVar *vSurfaceFuelAspenType;
    EqVar *vSurfaceFuelBedBetaRatio;
    EqVar *vSurfaceFuelBedBulkDensity;
    EqVar *vSurfaceFuelBedCoverage1;
    EqVar *vSurfaceFuelBedDeadFraction;
    EqVar *vSurfaceFuelBedLiveFraction;
    EqVar *vSurfaceFuelBedDepth;
    EqVar *vSurfaceFuelBedHeatSink;
    EqVar *vSurfaceFuelBedMextDead;
    EqVar *vSurfaceFuelBedMextLive;
    EqVar *vSurfaceFuelBedModel;
    EqVar *vSurfaceFuelBedModel1;
    EqVar *vSurfaceFuelBedModel2;
    EqVar *vSurfaceFuelBedMoisDead;
    EqVar *vSurfaceFuelBedMoisLive;
    EqVar *vSurfaceFuelBedPackingRatio;
    EqVar *vSurfaceFuelBedSigma;
    EqVar *vSurfaceFuelDens[8];
    EqVar *vSurfaceFuelHeat[8];
    EqVar *vSurfaceFuelHeatDead;
    EqVar *vSurfaceFuelHeatLive;
    EqVar *vSurfaceFuelLife[8];
    EqVar *vSurfaceFuelLoad[8];
    EqVar *vSurfaceFuelLoadDead;
    EqVar *vSurfaceFuelLoadDead1;
    EqVar *vSurfaceFuelLoadDead10;
    EqVar *vSurfaceFuelLoadDead100;
    EqVar *vSurfaceFuelLoadDeadHerb;
    EqVar *vSurfaceFuelLoadLive;
    EqVar *vSurfaceFuelLoadLiveHerb;
    EqVar *vSurfaceFuelLoadLiveWood;
    EqVar *vSurfaceFuelLoadTransferEq;
    EqVar *vSurfaceFuelLoadTransferFraction;
    EqVar *vSurfaceFuelLoadUndeadHerb;
    EqVar *vSurfaceFuelMois[8];
    EqVar *vSurfaceFuelMoisDead1;
    EqVar *vSurfaceFuelMoisDead10;
    EqVar *vSurfaceFuelMoisDead100;
    EqVar *vSurfaceFuelMoisDead1000;
    EqVar *vSurfaceFuelMoisLifeDead;
    EqVar *vSurfaceFuelMoisLifeLive;
    EqVar *vSurfaceFuelMoisLiveHerb;
    EqVar *vSurfaceFuelMoisLiveWood;
    EqVar *vSurfaceFuelMoisScenario;
    EqVar *vSurfaceFuelPalmettoAge;
    EqVar *vSurfaceFuelPalmettoCover;
    EqVar *vSurfaceFuelPalmettoHeight;
    EqVar *vSurfaceFuelPalmettoLoadDead1;
    EqVar *vSurfaceFuelPalmettoLoadDead10;
    EqVar *vSurfaceFuelPalmettoLoadDeadFoliage;
    EqVar *vSurfaceFuelPalmettoLoadLitter;
    EqVar *vSurfaceFuelPalmettoLoadLive1;
    EqVar *vSurfaceFuelPalmettoLoadLive10;
    EqVar *vSurfaceFuelPalmettoLoadLiveFoliage;
    EqVar *vSurfaceFuelPalmettoOverstoryBasalArea;
    EqVar *vSurfaceFuelSavr[8];
    EqVar *vSurfaceFuelSavrDead1;
    EqVar *vSurfaceFuelSavrLiveHerb;
    EqVar *vSurfaceFuelSavrLiveWood;
    EqVar *vSurfaceFuelSeff[8];
    EqVar *vSurfaceFuelStot[8];
    EqVar *vSurfaceFuelTemp;
    EqVar *vTimeIntegerDate;
    EqVar *vTimeJulianDate;
    EqVar *vTreeBarkThickness;
    EqVar *vTreeCanopyBulkDens;
    EqVar *vTreeCanopyCover;
    EqVar *vTreeCanopyCrownFraction;
    EqVar *vTreeCount;
    EqVar *vTreeCoverHt;
    EqVar *vTreeCoverHtDownwind;
    EqVar *vTreeCrownBaseHt;
    EqVar *vTreeCrownLengFractionScorchedAtVector;
    EqVar *vTreeCrownLengScorchedAtVector;
    EqVar *vTreeCrownRatio;
    EqVar *vTreeCrownVolScorchedAtVector;
    EqVar *vTreeDbh;
    EqVar *vTreeFoliarMois;
    EqVar *vTreeHt;
    EqVar *vTreeMortalityCountAtVector;
    EqVar *vTreeMortalityRateAspenAtVector;
    EqVar *vTreeMortalityRateAtVector;
    EqVar *vTreeSpecies;
    EqVar *vTreeSpeciesMortality;
    EqVar *vTreeSpeciesSpot;
    EqVar *vWindAdjFactor;
    EqVar *vWindAdjMethod;
    EqVar *vWindDirFromCompass;
    EqVar *vWindDirFromNorth;
    EqVar *vWindDirFromUpslope;
    EqVar *vWindSpeedAt10M;
    EqVar *vWindSpeedAt20Ft;
    EqVar *vWindSpeedAtMidflame;
    EqVar *vWthrAirTemp;
    EqVar *vWthrCumulusBaseHt;
    EqVar *vWthrDewPointTemp;
    EqVar *vWthrHeatIndex;
    EqVar *vWthrLightningStrikeType;
    EqVar *vWthrRelativeHumidity;
    EqVar *vWthrSummerSimmerIndex;
    EqVar *vWthrWetBulbTemp;
    EqVar *vWthrWindChillTemp;

// Declare all EqFun pointers here.
    EqFun *fContainFF;
    EqFun *fContainFFReportRatio;
    EqFun *fContainFFReportSize;
    EqFun *fContainFFReportSpread;
    EqFun *fContainFFSingle;
    EqFun *fCrownFireActiveCrown;
    EqFun *fCrownFireActiveRatio;
    EqFun *fCrownFireArea;
    EqFun *fCrownFireCritCrownSpreadRate;
    EqFun *fCrownFireCritSurfFireInt;
    EqFun *fCrownFireCritSurfFlameLeng;
    EqFun *fCrownFireFlameLeng;
    EqFun *fCrownFireFuelLoad;
    EqFun *fCrownFireHeatPerUnitArea;
    EqFun *fCrownFireHeatPerUnitAreaCanopy;
    EqFun *fCrownFireLengthToWidth;
    EqFun *fCrownFireLineInt;
    EqFun *fCrownFirePerimeter;
    EqFun *fCrownFirePowerOfFire;
    EqFun *fCrownFirePowerOfWind;
    EqFun *fCrownFirePowerRatio;
    EqFun *fCrownFireSpreadDist;
    EqFun *fCrownFireSpreadMapDist;
    EqFun *fCrownFireSpreadRate;
    EqFun *fCrownFireTransRatioFromFireIntAtVector;
    EqFun *fCrownFireTransRatioFromFlameLengAtVector;
    EqFun *fCrownFireTransToCrown;
    EqFun *fCrownFireType;
    EqFun *fCrownFireWindDriven;
    EqFun *fIgnitionFirebrandFuelMoisFromDead1Hr;
    EqFun *fIgnitionFirebrandProb;
    EqFun *fIgnitionLightningFuelMoisFromDead100Hr;
    EqFun *fIgnitionLightningProb;
    EqFun *fMapScale;
    EqFun *fMapSlope;
    EqFun *fSafetyZoneRadius;
    EqFun *fSafetyZoneSepDist;
    EqFun *fSiteAspectDirFromNorth;
    EqFun *fSiteRidgeToValleyDist;
    EqFun *fSiteSlopeFraction;
    EqFun *fSiteUpslopeDirFromNorth;
    EqFun *fSpotDistBurningPile;
    EqFun *fSpotDistSurfaceFire;
    EqFun *fSpotDistTorchingTrees;
    EqFun *fSpotMapDistBurningPile;
    EqFun *fSpotMapDistSurfaceFire;
    EqFun *fSpotMapDistTorchingTrees;
    EqFun *fSurfaceFireArea;
    EqFun *fSurfaceFireCharacteristicsDiagram;
    EqFun *fSurfaceFireDistAtBack;
    EqFun *fSurfaceFireDistAtHead;
    EqFun *fSurfaceFireDistAtVector;
    EqFun *fSurfaceFireEccentricity;
    EqFun *fSurfaceFireEffWindAtVector;
    EqFun *fSurfaceFireFlameAngle;
    EqFun *fSurfaceFireFlameHtAtVector;
    EqFun *fSurfaceFireFlameLengAtHead;
    EqFun *fSurfaceFireFlameLengAtVector;
    EqFun *fSurfaceFireHeatPerUnitArea;
    EqFun *fSurfaceFireHeatSource;
    EqFun *fSurfaceFireLengDist;
    EqFun *fSurfaceFireLengMapDist;
    EqFun *fSurfaceFireLengthToWidth;
    EqFun *fSurfaceFireLineIntAtHead;
    EqFun *fSurfaceFireLineIntAtVector;
    EqFun *fSurfaceFireMaxDirFromNorth;
    EqFun *fSurfaceFireMaxDirDiagram;
    EqFun *fSurfaceFireMapDistAtBack;
    EqFun *fSurfaceFireMapDistAtHead;
    EqFun *fSurfaceFireMapDistAtVector;
    EqFun *fSurfaceFireNoWindRate;
    EqFun *fSurfaceFirePerimeter;
    EqFun *fSurfaceFirePropagatingFlux;
    EqFun *fSurfaceFireReactionInt;
    EqFun *fSurfaceFireResidenceTime;
    EqFun *fSurfaceFireScorchHtFromFliAtVector;
    EqFun *fSurfaceFireScorchHtFromFlameLengAtVector;
    EqFun *fSurfaceFireShapeDiagram;
    EqFun *fSurfaceFireSpreadAtBack;
    EqFun *fSurfaceFireSpreadAtBeta;
    EqFun *fSurfaceFireSpreadAtHead;
    EqFun *fSurfaceFireVectorBeta;
    EqFun *fSurfaceFireVectorDirFromNorth;
    EqFun *fSurfaceFireVectorDirFromUpslope;
    EqFun *fSurfaceFireWidthDist;
    EqFun *fSurfaceFireWidthMapDist;
    EqFun *fSurfaceFuelAspenModel;
    EqFun *fSurfaceFuelAspenParms;
    EqFun *fSurfaceFuelBedHeatSink;
    EqFun *fSurfaceFuelBedIntermediates;
    EqFun *fSurfaceFuelBedModel;
    EqFun *fSurfaceFuelBedParms;
    EqFun *fSurfaceFuelBedWeighted;
    EqFun *fSurfaceFuelLoadTransferFraction;
    EqFun *fSurfaceFuelMoisLifeClass;
    EqFun *fSurfaceFuelMoisScenarioModel;
    EqFun *fSurfaceFuelMoisTimeLag;
    EqFun *fSurfaceFuelPalmettoModel;
    EqFun *fSurfaceFuelPalmettoParms;
    EqFun *fSurfaceFuelTemp;
    EqFun *fTimeJulianDate;
    EqFun *fTreeBarkThicknessFofem;
    EqFun *fTreeCrownBaseHt;
    EqFun *fTreeCrownRatio;
    EqFun *fTreeCrownVolScorchedAtVector;
    EqFun *fTreeMortalityCountAtVector;
    EqFun *fTreeMortalityRateAspenAtVector;
    EqFun *fTreeMortalityRateFofemAtVector;
    EqFun *fTreeMortalityRateFofem2AtVector;
    EqFun *fTreeMortalityRateFofemHoodAtVector;
    EqFun *fWindAdjFactor;
    EqFun *fWindSpeedAt20Ft;
    EqFun *fWindSpeedAtMidflame;
    EqFun *fWindDirFromNorth;
    EqFun *fWindDirFromUpslope;
    EqFun *fWthrCumulusBaseHt;
    EqFun *fWthrDewPointTemp;
    EqFun *fWthrHeatIndex;
    EqFun *fWthrRelativeHumidity;
    EqFun *fWthrSummerSimmerIndex;
    EqFun *fWthrWindChillTemp;

    // Declare all FUNCTIONS here
    void ContainFF( void ) ;
    void ContainFFReportRatio( void ) ;
    void ContainFFReportSize( void ) ;
    void ContainFFReportSpread( void ) ;
    void ContainFFSingle( void ) ;
    void CrownFireActiveCrown( void );
    void CrownFireActiveRatio( void );
    void CrownFireArea( void );
    void CrownFireCritCrownSpreadRate( void );
    void CrownFireCritSurfFireInt( void );
    void CrownFireCritSurfFlameLeng( void );
    void CrownFireFlameLength( void );
    void CrownFireFuelLoad( void );
    void CrownFireHeatPerUnitArea( void );
    void CrownFireHeatPerUnitAreaCanopy( void );
    void CrownFireLengthToWidth( void );
    void CrownFireLineIntensity( void );
    void CrownFirePerimeter( void );
    void CrownFirePowerOfFire( void );
    void CrownFirePowerOfWind( void );
    void CrownFirePowerRatio( void );
    void CrownFireTransRatioFromFireIntAtVector( void );
    void CrownFireTransRatioFromFlameLengAtVector( void );
    void CrownFireSpreadDist( void );
    void CrownFireSpreadMapDist( void );
    void CrownFireSpreadRate( void );
    void CrownFireTransToCrown( void );
    void CrownFireType( void );
    void CrownFireWindDriven( void );
    void FireArea( void );
    void FireCharacteristicsDiagram( void );
    void FireDistAtBack( void );
    void FireDistAtHead( void );
    void FireDistAtVector( void );
    void FireEccentricity( void );
    void FireEffWindAtVector( void );
    void FireFlameHtAtVector( void );
    void FireFlameLengAtHead( void );
    void FireFlameLengAtVector( void );
    void FireHeatPerUnitArea( void );
    void FireHeatSource( void );
    void FireLengDist( void );
    void FireLengMapDist( void );
    void FireLengthToWidth( void );
    void FireLineIntAtHead( void );
    void FireLineIntAtVector( void );
    void FireMaxDirFromNorth( void );
    void FireMaxDirDiagram( void );
    void FireMapDistAtBack( void );
    void FireMapDistAtHead( void );
    void FireMapDistAtVector( void );
    void FireNoWindRate( void );
    void FirePerimeter( void );
    void FirePropagatingFlux( void );
    void FireReactionInt( void );
    void FireResidenceTime( void );
    void FireScorchHtFromFliAtVector( void );
    void FireScorchHtFromFlameLengAtVector( void );
    void FireShapeDiagram( void );
    void FireSpreadAtBack( void );
    void FireSpreadAtBeta( void );
    void FireSpreadAtHead( void );
    void FireSpreadAtVector( void );
    void FireVectorBeta( void );
    void FireVectorDirFromNorth( void );
    void FireVectorDirFromUpslope( void );
    void FireWidthDist( void );
    void FireWidthMapDist( void );
    void FuelAspenModel( void );
    void FuelAspenParms( void );
    void FuelBedHeatSink( void );
    void FuelBedIntermediates( void );
    void FuelBedModel( void );
    void FuelBedParms( void );
    void FuelBedWeighted( void );
    void FuelLoadTransferFraction( void );
    void FuelMoisLifeClass( void );
    void FuelMoisScenarioModel( void );
    void FuelMoisTimeLag( void );
    void FuelPalmettoModel( void );
    void FuelPalmettoParms( void );
    void FuelTemp( void );
    void IgnitionFirebrandFuelMoisFromDead1Hr( void );
    void IgnitionLightningProb( void );
    void IgnitionLightningFuelMoisFromDead100Hr( void );
    void IgnitionFirebrandProb( void );
    void MapScale( void );
    void MapSlope( void );
    void SafetyZoneRadius( void );
    void SafetyZoneSepDist( void );
    void SiteAspectDirFromNorth( void );
    void SiteRidgeToValleyDist( void );
    void SiteSlopeFraction( void );
    void SiteUpslopeDirFromNorth( void );
    void SpotDistBurningPile( void );
    void SpotDistSurfaceFire( void );
    void SpotDistTorchingTrees( void );
    void SpotMapDistBurningPile( void );
    void SpotMapDistSurfaceFire( void );
    void SpotMapDistTorchingTrees( void );
    void TimeJulianDate( void );
    void TreeBarkThicknessBehave( void ) ;          //!< Depricated, historical
    void TreeBarkThicknessFofem( void );
    void TreeCrownBaseHt( void );
    void TreeCrownRatio( void );
    void TreeCrownVolScorchedAtVector( void );
    void TreeMortalityCountAtVector( void );
    void TreeMortalityRateAspenAtVector( void );
    void TreeMortalityRateBehaveAtVector( void );   //!< Deprecated, historical
    void TreeMortalityRateFofemAtVector( void );
    void TreeMortalityRateFofem2AtVector( void );
    void TreeMortalityRateFofemHoodAtVector( void );
    void WindDirFromNorth( void );
    void WindDirFromUpslope( void );
    void WindAdjFactor( void );
    void WindSpeedAt20Ft( void );
    void WindSpeedAtMidflame( void );
    void WthrCumulusBaseHt( void );
    void WthrDewPointTemp( void );
    void WthrHeatIndex( void );
    void WthrRelativeHumidity( void );
    void WthrSummerSimmerIndex( void );
    void WthrWindChillTemp( void );
};

#endif

//------------------------------------------------------------------------------
//  End of xeqcalc.h
//------------------------------------------------------------------------------

