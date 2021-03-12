//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TEllipsoid.h"
#include "TParseZona.h"
#include "TScg_polyconic.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
__fastcall TScg_polyconic::TScg_polyconic(VTApl *apl, int empresa_id, AnsiString empresa_nome)
    :TScg(apl, empresa_id)
{
    //salva ponteiro p/ objetos
    this->apl = apl;

//jss20191014
    // ProjCS (Projected Coordinate System) ou
    // Sistema de Projecão de Coordenadas ou
    // Sistema de Coordenadas Geográficas (scg)
    // para os mapas do Google (GG).
    // Valores fixos.
    ggProjCS.semi_eixo        = 6378137.0;
    ggProjCS.invflat          = 298.257223563;
    ggProjCS.deg2rad          = 0.0174532925199433;
    ggProjCS.false_easting    = 0.0;
    ggProjCS.false_northing   = 0.0;
    ggProjCS.central_meridian = 0.0;
    ggProjCS.k0               = 1.0;
    ggProjCS.dx               = 0.0;
    ggProjCS.dy               = 0.0;
    ggProjCS.dz               = 0.0;
    ggProjCS.datum            = datumWGS84;
    ggProjCS.achat            = 1.0 / ggProjCS.invflat;
    ggProjCS.equad            = 2.0 * ggProjCS.achat -
                                ggProjCS.achat * ggProjCS.achat;
    ggProjCS.elinquad         = ggProjCS.equad / (1.0 - ggProjCS.equad);
    ggProjCS.e1               = (1.0 - sqrt(1.0 - ggProjCS.equad)) /
                                (1.0 + sqrt(1.0 - ggProjCS.equad));
    ggProjCS.rad2deg          = 1.0 / ggProjCS.deg2rad;
    ggProjCS.tipo_coord       = coordLATLON;

    // ProjCS (Projected Coordinate System) ou
    // Sistema de Projecão de Coordenadas ou
    // Sistema de Coordenadas Geográficas (scg)
    // para a empresa lidos do arquivo
    // C:\Sinapsis\SINAPgrid\Dat\Geral\ProjCS.txt.
    ProjCS.company_name       = empresa_nome;
    ProjCS.empresa            = empresa_id;
    TParseZona *parse         = new TParseZona(apl);
    if (!parse->ReadProjCSParms(ProjCS))
    {
        // Alerta("Configurar padrão de coordenadas geográficas da empresa");
        Alerta("Configurar o arquivo de parâmetros de coordenadas geográficas da empresa");
    }
    if (parse)
    {
        delete parse;
        parse = NULL;
    }

    // define elipsoide
    EllipsoId = 19;
    /*
     switch(empresa_id)
     {//define constantes
     case ELETROBRAS_ALAGOAS:
     offset.dx = -160;
     offset.dy =   80;
     DefineConstantes();
     break;
     case CEAL:
     offset.dx = 0;
     offset.dy = 0;
     DefineConstantes();
     break;
     default: //DVK 2014.11.10
     offset.dx = 0;
     offset.dy = 0;
     DefineConstantes();
     break;
     }
     */
    // DVK 2014.11.10
    // DefineConstantes(empresa_id);
    // jss20191014
    // substituídos por parâmetros do ProjCS
    // if(! DefineConstantes(empresa_nome)) return;
    // inicia outras variaveis
    Inicia();
}

// ---------------------------------------------------------------------------
__fastcall TScg_polyconic::~TScg_polyconic(void)
   {
   //nada a fazer
   }
/*
//---------------------------------------------------------------------------
void __fastcall TScg_polyconic::DefineConstantes(void)
   {
   //variaveis locais
//   double  bb, ee;

   //define valores da CEAL
   falseEasting  = 1000000.;
   falseNorthing = 10000000.;
   centralMeridian = -36.75;
   originLatitude = -9.5;
   //inicia outras variaveis
   Inicia();
   }
*/
/*
//---------------------------------------------------------------------------
void __fastcall TScg_polyconic::DefineConstantes(int empresa_id)
   { //DVK 2014.11.10
   //constantes que variam de empresa para empresa
   switch(empresa_id)
		{//define constantes
      case CEAL:
		case ELETROBRAS_ALAGOAS:
         offset.dx = -160;
         offset.dy =   80;
         //define valores da CEAL
         falseEasting  = 1000000.;
         falseNorthing = 10000000.;
         centralMeridian = -36.75;
         originLatitude = -9.5;
         break;
		case ELETROBRAS_PIAUI:
         offset.dx = 0;
         offset.dy = 0;
         //define valores do PIAUI
         falseEasting  = 1000000.;
         falseNorthing = 10000000.;
         centralMeridian = -43.0;
         originLatitude = -7.0;
         break;
		default:
         offset.dx = 0;
         offset.dy = 0;
         //define valores da CEAL
         falseEasting  = 1000000.;
         falseNorthing = 10000000.;
         centralMeridian = -36.75;
         originLatitude = -9.5;
         break;
      }
   }
*/
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TScg_polyconic::DefineConstantes(AnsiString empresa_nome)
{   //DVK 2014.11.10
    // variáveis locais
    TParseZona parse(apl);

    try
    { // lê arquivo de zonas utm das empresas
        if (!parse.ExisteEmpresa(empresa_nome))
            return (false);
// jss20191014
        // substituídos por parâmetros do ProjCS

        // determina constantes da empresa
        // falseEasting  = parse.FalseEasting();
        // falseNorthing = parse.FalseNorthing();
        // centralMeridian = parse.CentralMeridian();
        // originLatitude = parse.OriginLatitude();
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}

// --------------------------------------------------------------------------- 
double __fastcall TScg_polyconic::floatEq( double x, double v, double epsilon )
   {
   return ((v - epsilon) < x) && (x < (v + epsilon));
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
void __fastcall TScg_polyconic::Inicia(void)
{
    //variaveis locais
    double j, three_es4;
    double lat, sin2lat, sin4lat, sin6lat;

    Poly_Max_Easting         =  20037509.0;
    Poly_Max_Northing        =  15348215.0;
    Poly_Min_Easting         = -20037509.0;
    Poly_Min_Northing        = -15348215.0;

    //converte para radianos
    Poly_origin_lat          = ProjCS.origin_lat       * ProjCS.deg2rad;
    Poly_origin_lon          = ProjCS.central_meridian * ProjCS.deg2rad;

    //verifica valores
    // Semi-major axis must be greater than zero
    // Inverse flattening must be between 250 and 350
    // Origin latitude out of range
    // Origin longitude out of range
    if (ProjCS.semi_eixo         <= 0.0)
        return;
    if ((ProjCS.invflat          <  250)        ||(350        < ProjCS.invflat))
        return;
    if ((Poly_origin_lat         < -M_PI / 2.0) ||(M_PI / 2.0 < Poly_origin_lat))
        return;
    if ((Poly_origin_lon         < -M_PI)       ||(M_PI       < Poly_origin_lon))
        return;

    if (Poly_origin_lon > M_PI)
        Poly_origin_lon -= 2.0 * M_PI;

    es2 = ProjCS.equad;
    es4 = es2 * es2;
    es6 = es2 * es4;

    j  = 45.0 * es6 / 1024.0;
    three_es4 = 3.0 * es4;
    c0 = 1.0 - es2 / 4.0 - three_es4 / 64.0 - 5.0 * es6 / 256.0;
    c1 = 3.0 * es2 / 8.0 + three_es4 / 32.0 + j;
    c2 = 15.0 * es4 / 256.0 + j;
    c3 = 35.0 * es6 / 3072.0;

    lat     = c0 * Poly_origin_lat;
    sin2lat = polyCoeffTimesSine(c1, 2.0, Poly_origin_lat);
    sin4lat = polyCoeffTimesSine(c2, 4.0, Poly_origin_lat);
    sin6lat = polyCoeffTimesSine(c3, 6.0, Poly_origin_lat);
    M0      = polyM(lat, sin2lat, sin4lat, sin6lat);

    //determina limites
    if (Poly_origin_lon > 0)
    {
        Poly_Max_Easting =  19926189.0;
        Poly_Min_Easting = -20037509.0;
    }
    else if (Poly_origin_lon < 0)
    {
        Poly_Max_Easting =  20037509.0;
        Poly_Min_Easting = -19926189.0;
    }
    else
    {
        Poly_Max_Easting =  20037509.0;
        Poly_Min_Easting = -20037509.0;
    }
    if(ProjCS.false_northing != 0)
    {
        Poly_Max_Northing -= ProjCS.false_northing;
        Poly_Min_Northing -= ProjCS.false_northing;
    }
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TScg_polyconic::LatLon_to_NorthEast(double lat, double lon, double &north, double &east)
{
   //variaveis locais
   double latit, sin2lat, sin4lat, sin6lat;
   double dlam; /* Longitude - Central Meridian */
   double NN;
   double NN_tlat;
   double MM;
   double EE;
   double slat = sin(lat * ProjCS.deg2rad);

    //verifica limites
    /* Latitude out of range */
    /* Longitude out of range */
    if ((lat * ProjCS.deg2rad < -M_PI / 2.0) || (M_PI / 2.0 < lat * ProjCS.deg2rad))
        return(false);
    if ((lon * ProjCS.deg2rad < -M_PI) || (M_PI < lon * ProjCS.deg2rad))
      return(false);

    // Distortion results if Longitude is > 90 deg from the Central Meridian
    dlam = lon * ProjCS.deg2rad - Poly_origin_lon;
    if (fabs(dlam) > M_PI / 2)
        return(false);

    //redefine Longitude - Central Meridian
    if (dlam > M_PI)
        dlam -= 2.0 * M_PI;
    if (dlam < -M_PI)
        dlam += 2.0 * M_PI;
    if (lat == 0.0)
    {
        east    = ProjCS.semi_eixo * dlam + ProjCS.false_easting;
        north   = -M0 + ProjCS.false_northing;
    }
    else
    {
        NN      = ProjCS.semi_eixo / sqrt(1.0 - es2 * (slat * slat));
        NN_tlat = NN / tan(lat * ProjCS.deg2rad);
        latit   = c0 * lat * ProjCS.deg2rad;
        sin2lat = polyCoeffTimesSine(c1, 2.0, lat * ProjCS.deg2rad);
        sin4lat = polyCoeffTimesSine(c2, 4.0, lat * ProjCS.deg2rad);
        sin6lat = polyCoeffTimesSine(c3, 6.0, lat * ProjCS.deg2rad);
        MM      = polyM(latit, sin2lat, sin4lat, sin6lat);
        EE      = dlam * slat;
        east    = NN_tlat * sin(EE) + ProjCS.false_easting;
        north   = MM - M0 + NN_tlat * (1.0 - cos(EE)) + ProjCS.false_northing;
   }
   return(true);
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TScg_polyconic::NorthEast_to_LatLon
                    (double  north, double  east, double &lat,   double &lon)
{
    //variaveis locais
    double dx; /* Delta easting - Difference in easting (easting-FE) */
    double dy; /* Delta northing - Difference in northing (northing-FN) */
    double dx_OVER_a;
    double AA;
    double BB;
    double CC = 0.0;
    double PHIn, Delta_PHI = 1.0;
    double sin_PHIn;
    double PHI, sin2PHI,sin4PHI, sin6PHI;
    double Mn, Mn_prime, Ma;
    double AA_Ma;
    double Ma2_PLUS_BB;
    double AA_MINUS_Ma;
    double tolerance = 1.0e-12;         /*  approximately 1/1000th of
                                            an arc second or 1/10th meter */
//  double longitude, latitude;
    int count = 45000;

    //corrige coordenadas com offset definido p/ a empresa
    north += offset.dy;
    east  += offset.dx;
    //verifica limites
    /* Easting out of range */
    /* Northing out of range */
    if ((east < ProjCS.false_easting + Poly_Min_Easting) ||
        (east > ProjCS.false_easting + Poly_Max_Easting))
        return(false);
    if ((north < ProjCS.false_northing + Poly_Min_Northing) ||
        (north > ProjCS.false_northing + Poly_Max_Northing))
        return(false);

    dy = north - ProjCS.false_northing;
    dx = east  - ProjCS.false_easting;
    dx_OVER_a  = dx / ProjCS.semi_eixo;

    // ???
    if (floatEq(dy, -M0, 1))
    {
        lat = 0.0;
        lon = dx_OVER_a + Poly_origin_lon;
    }
    else
    {
        AA   = (M0 + dy) / ProjCS.semi_eixo;
        BB   = dx_OVER_a * dx_OVER_a + (AA * AA);
        PHIn = AA;

        while (fabs(Delta_PHI) > tolerance && count)
        {
            sin_PHIn = sin(PHIn);
            CC       = sqrt(1.0 - es2 * sin_PHIn * sin_PHIn) * tan(PHIn);
            PHI      = c0 * PHIn;
            sin2PHI  = polyCoeffTimesSine(c1, 2.0, PHIn);
            sin4PHI  = polyCoeffTimesSine(c2, 4.0, PHIn);
            sin6PHI  = polyCoeffTimesSine(c3, 6.0, PHIn);
            Mn       = polyM(PHI, sin2PHI, sin4PHI, sin6PHI);
            Mn_prime = c0 - 2.0 * c1 * cos(2.0 * PHIn) +
                            4.0 * c2 * cos(4.0 * PHIn) -
                            6.0 * c3 * cos(6.0 * PHIn);
            Ma       = Mn / ProjCS.semi_eixo;
            AA_Ma    = AA * Ma;
            Ma2_PLUS_BB = Ma * Ma + BB;
            AA_MINUS_Ma = AA - Ma;
            Delta_PHI   = (AA_Ma * CC + AA_MINUS_Ma - 0.5 * (Ma2_PLUS_BB) * CC) /
                          (es2 * sin2PHI * (Ma2_PLUS_BB - 2.0 * AA_Ma) /
                          4.0 * CC + (AA_MINUS_Ma) * (CC * Mn_prime - 2.0 / sin2PHI) - Mn_prime);
            PHIn       -= Delta_PHI;
            count--;
        }

        if(!count)
            return(false);
//throw CoordinateConversionException( ErrorMessages::northing );

        lat = PHIn;

        if (lat > M_PI / 2.0) /* force distorted values to 90, -90 degrees */
            lat = M_PI / 2.0;
        else if (lat < -M_PI / 2.0)
            lat = -M_PI / 2.0;

        if (floatEq(fabs(lat), M_PI / 2.0, 0.00001) || (lat == 0))
            lon = Poly_origin_lon;
        else
            lon = (asin(dx_OVER_a * CC)) / sin(lat) + Poly_origin_lon;
    }
    if (lon > M_PI)
        lon -= 2.0 * M_PI;
    if (lon < -M_PI)
        lon += 2.0 * M_PI;

    if (lon > M_PI) /* force distorted values to 180, -180 degrees */
        lon = M_PI;
    else if (lon < -M_PI)
        lon = -M_PI;

    lat *= ProjCS.rad2deg;
    lon *= ProjCS.rad2deg;
    return(true);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TScg_polyconic::PM_GetZonaUtm(void)
{
    return("Não utilizada");
}

//---------------------------------------------------------------------------
void __fastcall TScg_polyconic::PM_SetZonaUtm(AnsiString zona)
{
    //nada a fazer
}

//--------------------------------------------------------------------------- 
double __fastcall TScg_polyconic::polyCoeffTimesSine( double coeff, double x, double latit )
{
   return coeff * (sin (x * latit));
}

//--------------------------------------------------------------------------- 
double __fastcall TScg_polyconic::polyM( double c0lat, double c1s2lat, double c2s4lat, double c3s6lat )
{
   return ProjCS.semi_eixo * (c0lat - c1s2lat + c2s4lat - c3s6lat);
}

//---------------------------------------------------------------------------
//eof
