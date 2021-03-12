#ifndef TScgH
#define TScgH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTScg.h"
#include <Fontes\ImportaRedeGis\VTImportaRedeGIS.h>
// ---------------------------------------------------------------------------
class TEllipsoid;
class TDatum;
class VTImportaBDGD;
// ---------------------------------------------------------------------------
class TScg : public VTScg
{
public:
				__fastcall TScg(VTApl *apl, int empresa_id);
                        __fastcall ~TScg(void);
            bool        __fastcall ConverteUTMmesmaZona(double &x1, double &x2);
    virtual bool        __fastcall NorthEast_to_LatLon
                                    (double  north, double  east,
                                     double &lat,   double &lon) {};
    virtual bool        __fastcall LatLon_to_NorthEast
                                    (double  lat,   double  lon,
                                     double &north, double &east) {};
            bool        __fastcall LatLon_to_FakeLatLon
                                    (double &FakeLat, double &FakeLon,
                                     double &lat,     double &lon);
            void        __fastcall DatumConvert
                                    (double  LatIn, double  LongIn, double  HtIn,
									 double &LatTo, double &LongTo, double &HtTo);

protected: //m�todos acessados via property
            AnsiString  __fastcall PM_GetZonaUtm(void) = 0;
            void        __fastcall PM_SetZonaUtm(AnsiString zona) = 0;

protected: // objetos externos
	VTApl *apl;

protected: // dados locais
	TEllipsoid ellipsoid[25];
	VTImportaRedeGIS *importaBDGD;

};

// ---------------------------------------------------------------------------
#endif
// eof
