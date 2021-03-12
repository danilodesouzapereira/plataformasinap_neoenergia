#ifndef TScg_polyconicH
#define TScg_polyconicH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TScg.h"

//---------------------------------------------------------------------------
class TScg_polyconic : public TScg
{
public:
            __fastcall  TScg_polyconic(VTApl *apl, int empresa_id, AnsiString empresa_nome);
            __fastcall ~TScg_polyconic(void);
    bool    __fastcall  NorthEast_to_LatLon
                        (double north, double east, double &lat, double &lon);
    bool    __fastcall LatLon_to_NorthEast(double lat, double lon, double &north, double &east);

protected: //métodos acessados via property
    AnsiString __fastcall PM_GetZonaUtm(void);
    void       __fastcall PM_SetZonaUtm(AnsiString zona);

private: //métodos
//      void     __fastcall DefineConstantes(void);
//      void     __fastcall DefineConstantes(int empresa_id);
    bool    __fastcall DefineConstantes(AnsiString empresa_nome);
    double  __fastcall floatEq(double x, double v, double epsilon);
    void    __fastcall Inicia(void);
    double  __fastcall polyCoeffTimesSine(double coeff, double x, double latit);
    double  __fastcall polyM(double c0lat, double c1s2lat, double c2s4lat, double c3s6lat);

private: //dados locais
//jss20191014
//  substituídos por parâmetros do ProjCS
//
//    double falseEasting;
//    double falseNorthing;
//    double centralMeridian;
//    double originLatitude;
//    double ellipsoidSemiMajorAxis;
//    double ellipsoidFlattening;

    /* Ellipsoid Parameters, default to WGS 84 */
    double es2; /* Eccentricity (0.08181919084262188000) squared */
    double es4; /* es2 * es2 */
    double es6; /* es4 * es2 */
    double M0;
    double c0; /* 1 - es2 / 4.0 - 3.0 * es4 / 64.0 - 5.0 * es6 / 256.0 */
    double c1; /* 3.0 * es2 / 8.0 + 3.0 * es4 / 32.0 + 45.0 * es6 /	1024.0 */
    double c2; /* 15.0 * es4 / 256.0 + 45.0 * es6 / 1024.0 */
    double c3; /* 35.0 * es6 / 3072.0 */

    /* Polyconic projection Parameters */
//jss20191014
//  substituídos por parâmetros do ProjCS
//
    double Poly_origin_lat; /* Latitude of origin in radians */
    double Poly_origin_lon; /* Longitude of origin in radians */
//    double Poly_False_Northing; /* False northing in meters */
//    double Poly_False_Easting; /* False easting in meters */

    /* Maximum variance for easting and northing values for WGS 84.	*/
    double Poly_Max_Easting;
    double Poly_Max_Northing;
    double Poly_Min_Easting;
    double Poly_Min_Northing;

//jss20191014
//  substituídos por parâmetros do ProjCS
//
//    double semiMajorAxis;
//    double flattening;

    //offset p/ corrigir defasagem das coordendas
    struct
    {
        double dx, dy;
    } offset;
};

//---------------------------------------------------------------------------
#endif
//eof

