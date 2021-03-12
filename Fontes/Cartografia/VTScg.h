//---------------------------------------------------------------------------
#ifndef VTScgH
#define VTScgH

//---------------------------------------------------------------------------
enum datumTYPE {datumSAD69=19, datumWGS84=23};

//---------------------------------------------------------------------------
struct  TProjCS
{                                       // examples:
// provided by customer
// '*' marks required fields
    AnsiString      company_name;       //   Celpe
    AnsiString      datum_code;         //   GCS_SAD_1969
    AnsiString      datum_name;         // * D_South_American_1969
    AnsiString      spheroid_name;      //   GRS_1967_Truncated
    double          semi_eixo;          // * in meters, 6 378 160.0
    double          invflat;            // * in meters, 298.25
    double          greenwich;          //   in degrees, 0.0
    double          deg2rad;            // * PI/ 180.0
    AnsiString      projection_name;    //   Transverse_Mercator
    double          false_easting;      // *    500 000.0
    double          false_northing;     // * 10 000 000.0
    double          central_meridian;   // * in degress, -38.0
    AnsiString      zone_name;          // * 23K
    double          k0;                 // * scale factor, 0.9996 (= 1 - 1 / 2500.0)
    double          origin_lat;         // * in degrees, 0.0
    AnsiString      unit_name;          //   Meter
    double          unit_value;         //   1.0
    int             tipo_coord;         // * coordUTM
    double          dx;                 // * conversão de datum, coord x, -67.35
    double          dy;                 // * conversão de datum, coord y,   3.88
    double          dz;                 // * conversão de datum, coord z, -38.22
// not provided by customer
    int             empresa;            //   72 (Celpe)
    int             datum;              //   SAD69
    double          achat;              //   1 / invflat
    double          equad;              //   2 * achat - achat * achat
    double          elinquad;           //   equad / (1 - equad)
    double          e1;                 //   (1 - sqrt(1 - equad)) /
                                        //   (1 + sqrt(1 - equad))
    double          rad2deg;            //   1 / deg2rad
    int             zone_number;        //   23
    AnsiString      zone_letter;        //   K
};  // Projected Coordinate System
    // Sistema de projeção de coordenadas

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEllipsoid.h"
#include "TDatum.h"

//---------------------------------------------------------------------------
enum tipoCOORD {coordNONE=-1, coordUTM, coordPOLYC, coordLATLON};

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTScg : public TObject
{
public:  //property
    __property AnsiString ZonaUTM   = {read=PM_GetZonaUtm,   write=PM_SetZonaUtm };
    __property int           TipoCoord = {read=tipo_coordenada, write=tipo_coordenada};

    int     empresa,
            EllipsoId;
    TDatum  *datum;

public:
                        __fastcall  VTScg(void) {};
                        __fastcall ~VTScg(void) {};
    virtual bool        __fastcall  ConverteUTMmesmaZona(double &x1, double &x2) = 0;
    virtual bool        __fastcall  NorthEast_to_LatLon
                                    (double  north, double  east,
                                     double &lat,   double &lon) = 0;
//                                    (double north, double east, double &lat, double &lon,
//                                     int tipo_coordenada=coordUTM) = 0;
    virtual bool        __fastcall  LatLon_to_NorthEast(double lat, double lon, double &north, double &east) = 0;
    virtual bool        __fastcall  LatLon_to_FakeLatLon(double &FakeLat, double &FakeLon,
                                                         double &lat, double &lon)=0;
    virtual void        __fastcall DatumConvert
                                    (double  LatIn, double  LongIn, double  HtIn,
                                     double &LatTo, double &LongTo, double &HtTo)=0;

protected: //métodos acessados via property
    virtual AnsiString  __fastcall PM_GetZonaUtm(void) = 0;
    virtual void        __fastcall PM_SetZonaUtm(AnsiString zona) = 0;

protected:
    int tipo_coordenada;

public:
//jss20191014
    TProjCS     ggProjCS;   // ProjCS do Google
    TProjCS     ProjCS;
};

//---------------------------------------------------------------------------
VTScg* __fastcall NewObjScg(VTApl *apl, int empresa_id, AnsiString empresa_nome);
VTScg* __fastcall NewObjScg_antigo(VTApl *apl, int empresa_id, AnsiString empresa_nome);

//---------------------------------------------------------------------------
#endif
//eof
