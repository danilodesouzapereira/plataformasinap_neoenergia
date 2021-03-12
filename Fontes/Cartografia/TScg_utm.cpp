// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TDatum.h"
#include "TEllipsoid.h"
#include "TScg_utm.h"
#include "TScg_polyconic.h"
#include "TParseZona.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
static TEllipsoid ellipsoid[] =
{   // id, Ellipsoid name, Equatorial Radius, square of eccentricity
    TEllipsoid(-1, "Placeholder", 0, 0),        // placeholder only, allows array indices to match id numbers
    TEllipsoid(1,  "Airy", 6377563, 0.00667054),
    TEllipsoid(2,  "Australian National", 6378160, 0.006694542),
    TEllipsoid(3,  "Bessel 1841", 6377397, 0.006674372),
    TEllipsoid(4,  "Bessel 1841 (Nambia) ", 6377484, 0.006674372),
    TEllipsoid(5,  "Clarke 1866", 6378206, 0.006768658),
    TEllipsoid(6,  "Clarke 1880", 6378249, 0.006803511),
    TEllipsoid(7,  "Everest", 6377276, 0.006637847),
    TEllipsoid(8,  "Fischer 1960 (Mercury) ", 6378166, 0.006693422),
    TEllipsoid(9,  "Fischer 1968", 6378150, 0.006693422),
    TEllipsoid(10, "GRS 1967", 6378160, 0.006694605),
    TEllipsoid(11, "GRS 1980", 6378137, 0.00669438),
    TEllipsoid(12, "Helmert 1906", 6378200, 0.006693422),
    TEllipsoid(13, "Hough", 6378270, 0.00672267),
    TEllipsoid(14, "International", 6378388, 0.00672267),
    TEllipsoid(15, "Krassovsky", 6378245, 0.006693422),
    TEllipsoid(16, "Modified Airy", 6377340, 0.00667054),
    TEllipsoid(17, "Modified Everest", 6377304, 0.006637847),
    TEllipsoid(18, "Modified Fischer 1960", 6378155, 0.006693422),
    TEllipsoid(19, "South American 1969", 6378160, 0.006694542),
    TEllipsoid(20, "WGS 60", 6378165, 0.006693422),
    TEllipsoid(21, "WGS 66", 6378145, 0.006694542),
    TEllipsoid(22, "WGS-72", 6378135, 0.006694318),
    TEllipsoid(23, "WGS-84", 6378137, 0.00669438) // ,
//  TEllipsoid(24, "South American 1969_2", 6378160, 298.25)
};

/*
 //---------------------------------------------------------------------------
 VTScg* __fastcall NewObjScg(VTApl *apl, int empresa_id, AnsiString empresa_nome)
 {
 switch(empresa_id)
 {
 case ELETROBRAS_ALAGOAS:
 //conversor de Policônicas
 return(new TScg_polyconic(apl, empresa_id, empresa_nome));
 case CEAL:
 //conversor de Policônicas
 return(new TScg_polyconic(apl, empresa_id, empresa_nome));
 default:
 //conversor padrão de UTM
 return(new TScg_utm(apl, empresa_id, empresa_nome));
 }
 }
 */
/*
 // ---------------------------------------------------------------------------
 VTScg* __fastcall NewObjScg(VTApl *apl, int empresa_id, AnsiString empresa_nome)
 { // DVK 2014.11.07
 // variáveis locais
 TParseZona parse(apl);

 try
 { // lê arquivo de zonas utm das empresas
 if (!parse.ExisteEmpresa(empresa_nome))
 {
 // conversor padrão de UTM
 Alerta("Configurar padrão de coordenadas geográficas da empresa");
 return (new TScg_utm(apl, empresa_id, empresa_nome));
 }
 // determina tipo de coordenada da empresa
 switch (parse.TipoCoord())
 {
 case coordPOLYC:
 // conversor de Policônicas
 return (new TScg_polyconic(apl, empresa_id, empresa_nome));
 case coordUTM:
 // conversor de UTM
 return (new TScg_utm(apl, empresa_id, empresa_nome));
 case coordNONE:
 // conversor de UTM
 return (NewObjScg_antigo(apl, empresa_id, empresa_nome));
 default:
 // conversor padrão de UTM
 return (new TScg_utm(apl, empresa_id, empresa_nome));
 }
 }
 catch (Exception &e)
 {
 // conversor padrão de UTM
 return (new TScg_utm(apl, empresa_id, empresa_nome));
 }
 }

 // ---------------------------------------------------------------------------
 VTScg* __fastcall NewObjScg_antigo(VTApl *apl, int empresa_id, AnsiString empresa_nome)
 {
 switch (empresa_id)
 {
 case ELETROBRAS_ALAGOAS:
 // conversor de Policônicas
 return (new TScg_polyconic(apl, empresa_id, empresa_nome));
 default:
 // conversor padrão de UTM
 return (new TScg_utm(apl, empresa_id, empresa_nome));
 }
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
__fastcall TScg_utm::TScg_utm(VTApl *apl, int empresa_id, AnsiString empresa_nome)
    : TScg(apl, empresa_id)
{
	// salva ponteiro p/ objetos
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
    ProjCS.company_name = empresa_nome;
    ProjCS.empresa      = empresa_id;
    TParseZona *parse   = new TParseZona(apl);
    if (!parse->ReadProjCSParms(ProjCS))
    {
//      Alerta("Configurar padrão de coordenadas geográficas da empresa");
        Alerta("Configurar o arquivo de parâmetros de coordenadas geográficas da empresa");
        return;
    }
    if (parse)
    {
        delete parse;
        parse = NULL;
    }

    // define valores padrão
    EllipsoId = 23;
    empresa = empresa_id;
    if (!LeArquivoZonaEmpresa(empresa_nome))
        UTMZone = "23K";

    // define tipo de coordenada
    switch (empresa_id)
    {
    case COELBA:
        {
            TipoCoord = coordLATLON;
            break;
        }
    case CELPE:
        {
            this->TipoCoord = coordLATLON;
            break;
        }
    case COSERN:
        {
            this->TipoCoord = coordLATLON;
            break;
        }
    case COPEL:
        {
            this->TipoCoord = coordUTM;
            this->empresa = COPEL;
            break;
        }
    case CPFL:
        {
            this->TipoCoord = coordLATLON;
            break;
        }
    default:
        {
            this->TipoCoord = coordUTM;
        }
    }
}

// ---------------------------------------------------------------------------
__fastcall TScg_utm::~TScg_utm(void)
{
    // nada a fazer
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TScg_utm::CasosEspeciais(double &UTMNorthing, double &UTMEasting, double &lat, double &lon)
 {//DVK 2017.06.01
 switch(empresa)
 {
 case AES_SUL:
 {//AES Sul: x pode estar na zona 21 (x<500000) ou 22 (x>=500000)
 //redefine x e y: sistema TM
 if(UTMEasting>=500000.0)
 {
 UTMZone = "22J";
 UTMEasting = UTMEasting + 208709.0 + 1019.0 - 500000.0;
 }
 else
 {
 UTMZone = "21J";
 UTMEasting = UTMEasting + 210590.35 + 1019.0;
 }
 UTMNorthing = UTMNorthing - 1000000.0 + 6677424.02 + 8208.0;
 //k0 = 0.9993;
 break;
 }
 case CELPE:
 {//redefine elipsoide e zona intermediária
 //         EllipsoId   = 19;//10;
 //         UTMZone = DoubleToStr("%6.5f",(183.-38.)/6.) + "L";  //merid.central = -38
 //         break;
 lat = double(UTMNorthing * 1e-5) - 100.;
 lon = double(UTMEasting * 1e-5) - 100.;
 return(true);
 }
 case COELBA:
 {//redefine elipsoide e zona intermediária
 //         EllipsoId   = 19;//10;
 //         UTMZone = DoubleToStr("%6.5f",(183.-42.)/6.) + "L";  //merid.central = -42
 //         break;
 lat = double(UTMNorthing * 1e-5) - 100.;
 lon = double(UTMEasting * 1e-5) - 100.;
 return(true);
 }
 case COSERN:
 {//redefine elipsoide e zona intermediária
 //         EllipsoId   = 19;//10;
 //         UTMZone = DoubleToStr("%6.5f",(183.-36.)/6.) + "M";  //merid.central = -36
 //         break;
 lat = double(UTMNorthing * 1e-5) - 100.;
 lon = double(UTMEasting * 1e-5) - 100.;
 return(true);
 }
 case FECOERGS:
 {//ajuste fino
 UTMEasting -= 59.29;
 UTMNorthing -= 45.18;
 break;
 }
 case EDP_ESCELSA:
 {//ajuste fino
 UTMEasting -= 42.94;
 UTMNorthing -= 37.37;
 break;
 }
 case ELETROBRAS_ACRE:
 {//ELETROBRAS_ACRE: x pode estar na zona 18 (x<169256) ou 19 (x>=169256)
 //Y pode estar na zona  L (y<9114496) ou M (y>=9114496)
 //redefine x e y: sistema UTM estendido
 if(UTMEasting < 169256.16)
 {
 UTMZone = "18";
 UTMEasting = 830750.0 - (169256.16 - UTMEasting);
 }
 else
 {
 UTMZone = "19";
 //            UTMEasting -= 1000.0;
 }
 if(UTMNorthing < 9114496.24) UTMZone += "L";
 else                         UTMZone += "M";
 break;
 }
 case ELETROBRAS_RORAIMA:
 {//ELETROBRAS_RORAIMA: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
 //Y pode estar na zona  M (y<=0)      ou  N (y>0)
 //redefine x e y: sistema UTM estendido
 if(UTMEasting > 833867.0)
 {
 UTMZone = "21";
 UTMEasting = (UTMEasting - 833867.0) + 166064.0;
 }
 else
 {
 UTMZone = "20";
 //            UTMEasting -= 1000.0;
 }
 if(UTMNorthing < 0)
 {
 UTMNorthing = 9999899.0 + UTMNorthing;
 UTMZone += "M";
 }
 else                UTMZone += "N";
 break;
 }
 case ELETROBRAS_RONDONIA:
 {//ELETROBRAS_RONDONIA: x pode estar na zona 19 (x<=170113) ou 20 (x>170113)
 //acho que não chega na zona 21...
 //redefine x : sistema UTM estendido
 if(UTMEasting < 170113.0)
 {
 UTMZone = "19";
 UTMEasting = 829230.0 - (170113.0 - UTMEasting);
 }
 else
 {
 UTMZone = "20";
 //            UTMEasting -= 1000.0;
 }
 UTMZone += "L";
 break;
 }
 case ELETROBRAS_AMAZONAS: //vale somente para capital
 {//ELETROBRAS_AMAZONAS: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
 //redefine x : sistema UTM estendido "diferente"
 if(UTMEasting > 500000.0)
 {
 UTMZone = "21";
 UTMEasting = (UTMEasting - 500000.0) + 166064.0;
 }
 else
 {
 UTMZone = "20";
 UTMEasting = 833867.0 - (500000.0 - UTMEasting);
 }
 UTMNorthing = 9000000.0 + UTMNorthing;
 UTMZone += "M";
 break;
 }
 }
 //remove 500,000 meter offset for longitude
 UTMEasting = UTMEasting - 500000.0;
 return(false);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TScg_utm::CasosEspeciais(double &UTMNorthing, double &UTMEasting,
                                         double &lat,         double &lon,
                                         int tipo_coordenada)
{
// jss20191014
    // os ajustes empíricos usados nesse método foram retirados
    // foram mantidos apenas o recálculo da zona UTM
    // e os Ellipsoids para retrocompatibilidade

    // DVK 2017.07.11
    switch (empresa)
    {
        case CPFL: // tipo_coordenada == coordLATLON
        { // converte falso UTM em lat lon
            lat = double(UTMNorthing * 1e-5) - 100.;
            lon = double(UTMEasting * 1e-5) - 100.;
            return (true);
        }
        case RGESUL:
        { // AES Sul: x pode estar na zona 21 (x<500000) ou 22 (x>=500000)
            // redefine x e y: sistema TM
            if (UTMEasting >= 500000.0)
            {
                UTMZone = "22J";
//              UTMEasting = UTMEasting + 208709.0 + 1019.0 - 500000.0;
            }
            else
            {
                UTMZone = "21J";
//              UTMEasting = UTMEasting + 210590.35 + 1019.0;
            }
//          UTMNorthing = UTMNorthing - 1000000.0 + 6677424.02 + 8208.0;
//          k0 = 0.9993;
            break;
        }
        case CELPE:
        { // considera opção do usuário
            if (tipo_coordenada == coordLATLON)
            { // converte falso UTM em lat lon
                lat = double(UTMNorthing * 1e-5) - 100.;
                lon = double(UTMEasting * 1e-5) - 100.;
                return (true);
            }
            // se for UTM, redefine elipsoide e zona intermediária celpe
            EllipsoId = 19; // 10;
            UTMZone = DoubleToStr("%6.5f", (183. - 38.) / 6.) + "L"; // merid.central = -38
            break;
        }
        case COELBA:
        { // considera opção do usuário
            if (tipo_coordenada == coordLATLON)
            { // converte falso UTM em lat lon
                lat = double(UTMNorthing * 1e-5) - 100.;
                lon = double(UTMEasting * 1e-5) - 100.;
                return (true);
            }
            // se for UTM, redefine elipsoide e zona intermediária coelba
            EllipsoId = 19; // 10;
            UTMZone = DoubleToStr("%6.5f", (183. - 42.) / 6.) + "L"; // merid.central = -42
            break;
        }
        case COSERN:
        { // considera opção do usuário
            if (tipo_coordenada == coordLATLON)
            { // converte falso UTM em lat lon
                lat = double(UTMNorthing * 1e-5) - 100.;
                lon = double(UTMEasting * 1e-5) - 100.;
                return (true);
            }
            // se for UTM, redefine elipsoide e zona intermediária cosern
            EllipsoId = 19; // 10;
            UTMZone = DoubleToStr("%6.5f", (183. - 36.) / 6.) + "M"; // merid.central = -36
            break;
        }
        case COPEL:
        { // ajuste fino
            // UTMEasting -= 200;
            // UTMNorthing -= 300;
            EllipsoId = 19;
            break;
        }
        case FECOERGS:
        { // ajuste fino
//          UTMEasting -= 59.29;
//          UTMNorthing -= 45.18;
            break;
        }
        case EDP_ESCELSA:
        { // ajuste fino
//          UTMEasting -= 42.94;
//          UTMNorthing -= 37.37;
            break;
        }
//jss20191014
/*
        case ELETROBRAS_ACRE:
        { // ELETROBRAS_ACRE: x pode estar na zona 18 (x<169256) ou 19 (x>=169256)
            // Y pode estar na zona  L (y<9114496) ou M (y>=9114496)
            // redefine x e y: sistema UTM estendido
            if (UTMEasting < 169256.16)
            {
                UTMZone      = "18";
                // UTMEasting = 830750.0 - (169256.16 - UTMEasting);
                UTMEasting   = 830308.26 - (169256.16 - UTMEasting); // D.V.C. 05/12
                UTMNorthing += 1187.94; // D.V.C. 06/12
            }
            else
            {
                // Ajuste para zona 19
                UTMZone      = "19";
                UTMEasting  -= 62.2;
                UTMNorthing -= 39.07; // D.V.C. 06/12
            }
            if (UTMNorthing < 9114496.24)
            {
                UTMZone     += "L";
            }
            else
                UTMZone += "M";
            break;
        }
        case ELETROBRAS_RORAIMA:
        { // ELETROBRAS_RORAIMA: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
            // Y pode estar na zona  M (y<=0)      ou  N (y>0)
            // redefine x e y: sistema UTM estendido
            if (UTMEasting > 833867.0)
            {
                UTMZone = "21";
                UTMEasting = (UTMEasting - 833867.0) + 166064.0;
            }
            else
            {
                UTMZone = "20";
            }
            if (UTMNorthing < 0)
            {
                UTMNorthing = 9999899.0 + UTMNorthing;
                UTMZone += "M";
            }
            else
                UTMZone += "N";
            break;
        }
        case ELETROBRAS_RONDONIA:
        { // ELETROBRAS_RONDONIA: x pode estar na zona 19 (x<=170113) ou 20 (x>170113)
            // acho que não chega na zona 21...
            // redefine x : sistema UTM estendido
            if (UTMEasting < 170113.0)
            {
                UTMZone = "19";
                UTMEasting = 829230.0 - (170113.0 - UTMEasting);
            }
            else
            {
                UTMZone = "20";
            }
            UTMZone += "L";
            break;
        }
*/
        // case ELETROBRAS_AMAZONAS: // vale somente para capital
        // { // ELETROBRAS_AMAZONAS: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
        // // redefine x : sistema UTM estendido "diferente"
        // if (UTMEasting > 500000.0)
        // {
        // UTMZone = "21";
        // UTMEasting = (UTMEasting - 500000.0) + 166064.0;
        // }
        // else
        // {
        // UTMZone = "20";
        // UTMEasting = 833867.0 - (500000.0 - UTMEasting);
        // }
        // UTMNorthing = 9000000.0 + UTMNorthing;
        // UTMZone += "M";
        // break;
        // }
        case ELETROBRAS_AMAZONAS: // vale para capital se Y < 9000000
        {
            if (UTMNorthing < 9000000.0)
            {   // ELETROBRAS_AMAZONAS: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
                // redefine x : sistema UTM estendido "diferente"
                if (UTMEasting > 500000.0)
                {
                    UTMZone = "21";
//                  UTMEasting = (UTMEasting - 500000.0) + 166064.0;
                }
                else
                {
                    UTMZone = "20";
//                  UTMEasting = 833867.0 - (500000.0 - UTMEasting);
                }
//jss20191014
//              UTMNorthing  = 9000000.0 + UTMNorthing - 500.0;
                UTMNorthing += 9000000.0;
                UTMZone += "M";
                break;
            }
            else // interior
            {   // ELETROBRAS_AMAZONAS: x pode estar na zona 19 (x<=832749) ou 20 (x>832749)
                // redefine x : sistema UTM estendido "diferente"
                if (UTMEasting > 832749.0)
                {
                    UTMZone = "20";
//                  UTMEasting = (UTMEasting - 833222.0) + 166100.0;
                }
                else
                {
                    UTMZone = "19";
//                  UTMEasting = 833867.0 - (500000.0 - UTMEasting);
                }
//              UTMNorthing = 2350.0 + UTMNorthing;
                // ajuste fino em Coari (não sei se vale para outras SED)
                UTMZone += "M";
                break;
            }
        }
    }
    // remove 500,000 meter offset for longitude
//jss20191014
    // esse ajuste foi retirado para usar, ao invés dele, o parâmetro lido
    // do arquivo ProjCS.txt
    // assim, esse valor pode variar de empresa para empresa
//  UTMEasting = UTMEasting - 500000.0;
    return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TScg_utm::ConverteUTMmesmaZona(double &x1, double &x2)
{ // DVK 2017.02.01
    switch (empresa)
    {
    case ELETROBRAS_ACRE:
        { // ELETROBRAS_ACRE: x pode estar na zona 18 (x<169256) ou 19 (x>=169256)
            // Y pode estar na zona  L (y<9114496) ou M (y>=9114496)
            // redefine x : sistema UTM estendido
            // o maior x deve estar mais perto da fronteira e terá seu valor alterado
            if (x1 > x2)
                x1 = x1 - 830750.0 + 169256.16;
            else
                x2 = x2 - 830750.0 + 169256.16;
            break;
        }
    case ELETROBRAS_RORAIMA:
        { // ELETROBRAS_RORAIMA: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
            // Y pode estar na zona  M (y<=0)      ou  N (y>0)
            // redefine x : sistema UTM estendido
            // o menor x deve estar mais perto da fronteira e terá seu valor alterado
            if (x1 < x2)
                x1 = x1 + 833867.0 - 166064.0;
            else
                x2 = x2 + 833867.0 - 166064.0;
            break;
        }
    case ELETROBRAS_RONDONIA:
        { // ELETROBRAS_RONDONIA: x pode estar na zona 19 (x<=170113) ou 20 (x>170113)
            // acho que não chega na zona 21...
            // redefine x : sistema UTM estendido
            // o maior x deve estar mais perto da fronteira e terá seu valor alterado
            if (x1 > x2)
                x1 = x1 - 829230.0 + 170113.0;
            else
                x2 = x2 - 829230.0 + 170113.0;
        }
    case ELETROBRAS_AMAZONAS: // vale somente para capital
        { // ELETROBRAS_AMAZONAS: x pode estar na zona 20 (x<=833867) ou 21 (x>833867)
            // redefine x : sistema UTM estendido
            // o menor x deve estar mais perto da fronteira e terá seu valor alterado
            if (x1 < x2)
                x1 = x1 + 833867.0 - 166064.0;
            else
                x2 = x2 + 833867.0 - 166064.0;
            break;
        }
    default:
        break;
    }
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TScg_utm::LeArquivoZonaEmpresa(AnsiString empresa_nome)
{
    // variáveis locais
    TParseZona parse(apl);

    try
    { // lê arquivo de zonas utm das empresas
        if (!parse.ExisteEmpresa(empresa_nome))
            return (false);
        // determina zona utm da empresa
        UTMZone = parse.UTMZoneX() + parse.UTMZoneY();
    }
    catch (Exception &e)
    {
        return (false);
    }
    return (true);
}

// ---------------------------------------------------------------------------
// bool __fastcall TScg_utm::LatLon_to_FakeLatLon(double &FakeLat, double &FakeLon, double &lat,
// double &lon)
// {
// int tipo_coordenada = coordLATLON;
// switch (empresa)
// {
// case CELPE:
// case COELBA:
// case COSERN:
// { // considera opção do usuário
// if (tipo_coordenada == coordLATLON)
// { // converte falso UTM em lat lon
// FakeLat = double(lat + 100) * 1e5;
// FakeLon = double(lon + 100) * 1e5;
// return (true);
// }
// break;
// }
// }
// return (false);
// }
//
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TScg_utm::LatLon_to_NorthEast(double  lat,   double  lon,
                                              double &north, double &east)
{
// jss20191014
    // os valores fixos foram substituídos pelos valores lidos do arquivo
    // ProjCS.txt
    // assim, os valores poderão variar de empresa para empresa

    // variaveis locais

//  double PI = M_PI;
//  double FOURTHPI = PI / 4;
//  double deg2rad = PI / 180.0;
//  double rad2deg = 180.0 / PI;

    double a = ProjCS.semi_eixo;            // ellipsoid[EllipsoId].EquatorialRadius;
    double eccSquared = ProjCS.equad;       // ellipsoid[EllipsoId].eccentricitySquared;
    double k0 = ProjCS.k0;                  // 0.9996;
//  double LongOrigin;
    double eccPrimeSquared = ProjCS.elinquad;
    double N, T, C, A, M;

    if (LatLon_to_FakeLatLon(north, east, lat, lon))
        return (true);

    // Make sure the longitude is between -180.00 .. 179.9
//  double LongTemp = (lon + 180)-int((lon + 180) / 360) * 360 - 180; // -180.00 .. 179.9;

    double LatRad  = lat * ProjCS.deg2rad;
    double LongRad = lon * ProjCS.deg2rad;  // LongTemp * deg2rad;
//  double LongOriginRad;
//  int ZoneNumber;

//  ZoneNumber = int((LongTemp + 180) / 6) + 1;
//  ZoneNumber = int((LongTemp + 180)/6);

//  if (lat >= 56.0 && lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0)
//      ZoneNumber = 32;

/*
    // Special zones for Svalbard
    if (lat >= 72.0 && lat < 84.0)
    {
        if (LongTemp >= 0.0 && LongTemp < 9.0)
            ZoneNumber = 31;
        else if (LongTemp >= 9.0 && LongTemp < 21.0)
            ZoneNumber = 33;
        else if (LongTemp >= 21.0 && LongTemp < 33.0)
            ZoneNumber = 35;
        else if (LongTemp >= 33.0 && LongTemp < 42.0)
            ZoneNumber = 37;
    }
    LongOrigin    = (ZoneNumber - 1) * 6 - 180 + 3; // +3 puts origin in middle of zone
    LongOriginRad = LongOrigin * deg2rad;
*/

    // compute the UTM Zone from the latitude and longitude
//  UTMZone = "23K";
//  sprintf(UTMZone.c_str(), "%d%c", ZoneNumber, UTMLetterDesignator(lat));
//  eccPrimeSquared = (eccSquared) / (1 - eccSquared);

    N = a / sqrt(1 - eccSquared * sin(LatRad) * sin(LatRad));
    T = tan(LatRad) * tan(LatRad);
    C = eccPrimeSquared * cos(LatRad) * cos(LatRad);
    A = cos(LatRad) * (LongRad - ProjCS.central_meridian * ProjCS.deg2rad);
                                // LongOriginRad);

    M = a * ((1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 -
        5 * eccSquared * eccSquared * eccSquared / 256) * LatRad -
        (3 * eccSquared / 8 + 3 * eccSquared * eccSquared / 32 +
        45 * eccSquared * eccSquared * eccSquared / 1024) * sin(2 * LatRad) +
        (15 * eccSquared * eccSquared / 256 + 45 * eccSquared * eccSquared * eccSquared / 1024)
        * sin(4 * LatRad) - (35 * eccSquared * eccSquared * eccSquared / 3072) * sin(6 * LatRad));

    east = (double)(k0 * N * (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T + 72 * C -
        58 * eccPrimeSquared) * A * A * A * A * A / 120) + ProjCS.false_easting);

    north = (double)(k0 * (M + N * tan(LatRad) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C)
        * A * A * A * A / 24 + (61 - 58 * T + T * T + 600 * C - 330 * eccPrimeSquared)
        * A * A * A * A * A * A / 720)));
    if (lat < 0)
        north += ProjCS.false_northing; // 10000000 meter offset for southern hemisphere
    return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
bool __fastcall TScg_utm::NorthEast_to_LatLon(double  north, double  east,
                                              double &lat,   double &lon)
//                    (double north, double east, double &lat, double &lon,
//                     int tipo_coordenada)
{
// jss20191014
    // os valores fixos foram substituídos pelos valores lidos do arquivo
    // ProjCS.txt
    // assim, os valores poderão variar de empresa para empresa

    // variaveis locais
    // double PI = M_PI;
    // double FOURTHPI = PI / 4;
    // double deg2rad = PI / 180;
    // double rad2deg = 180.0 / PI;

//  double k0 = ProjCS.k0;                          // 0.9996;
    double a = ProjCS.semi_eixo;                    // ellipsoid[EllipsoId].EquatorialRadius;
    double eccSquared = ProjCS.equad;               // ellipsoid[EllipsoId].eccentricitySquared;
    double eccPrimeSquared = ProjCS.elinquad;
    double e1 = ProjCS.e1;                          // (1 - sqrt(1 - eccSquared)) / (1 + sqrt(1 - eccSquared));
    double N1, T1, C1, R1, D, M;
//  double LongOrigin;
//    double mu, phi1, phi1Rad;
    double mu, phi1Rad;
    double x, y;
//  double ZoneNumber;
//  char*  ZoneLetter;
//  int    NorthernHemisphere; // 1 for northern hemispher, 0 for southern
//    AnsiString letra;
    this->TipoCoord = ProjCS.tipo_coord;           // tipo_coordenada;

    // define x e y
//jss20191014
    x = east;
    y = north;
    // redefine x e y, dependendo da empresa
    if (CasosEspeciais(y, x, lat, lon, ProjCS.tipo_coord)) // tipo_coordenada))
        return (true);
    // converte
/*  letra = UTMZone.SubString(UTMZone.Length(), 1);
    // ZoneNumber = strtoul(UTMZone.c_str(), &ZoneLetter, 10);
    strtoul(letra.c_str(), &ZoneLetter, 1);
    ZoneNumber = StrToDouble(UTMZone.SubString(0, UTMZone.Length() - 1));
    LongOrigin = (ZoneNumber - 1) * 6 - 180 + 3; // +3 puts origin in middle of zone
    ZoneLetter = ProjCS.zone_letter;
    ZoneNumber = ProjCS.zone_number;
    LongOrigin = ProjCS.central_meridian;

    if ((*ZoneLetter - 'N') >= 0)
        NorthernHemisphere = 1;                 // point is in northern hemisphere
    else
    {
        NorthernHemisphere = 0;                 // point is in southern hemisphere
        y -= ProjCS.false_northing;             // 10000000.0; // remove 10,000,000 meter offset used for southern hemisphere
    }
*/

//    eccPrimeSquared = (eccSquared) / (1 - eccSquared);

    M = (y - ProjCS.false_northing)/ ProjCS.k0;
    mu = M / (a * (1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 -
                   5 * eccSquared * eccSquared * eccSquared / 256));

    phi1Rad = mu +
              (3 * e1 / 2 - 27 * e1 * e1 * e1 / 32) * sin(2 * mu) +
              (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32) * sin(4 * mu) +
              (151 * e1 * e1 * e1 / 96) * sin(6 * mu);
//    phi1 = phi1Rad * ProjCS.rad2deg;

    N1 = a / sqrt(1 - eccSquared * sin(phi1Rad) * sin(phi1Rad));
    T1 = tan(phi1Rad) * tan(phi1Rad);
    C1 = eccPrimeSquared * cos(phi1Rad) * cos(phi1Rad);
    R1 = a *
         (1 - eccSquared) /
         pow(1 - eccSquared * sin(phi1Rad) * sin(phi1Rad), 1.5);
    D = (x  - ProjCS.false_easting) / (N1 * ProjCS.k0);

    lat = phi1Rad -
          (N1 * tan(phi1Rad) / R1) *
          (D * D / 2 - (5 + 3 * T1 + 10 * C1 - 4 * C1 * C1 - 9 * eccPrimeSquared) *
           D * D * D * D / 24 +
           (61 + 90 * T1 + 298 * C1 + 45 * T1 * T1 - 252 * eccPrimeSquared - 3 * C1 * C1) *
           D * D * D * D * D * D / 720
          );
    lat = lat * ProjCS.rad2deg;

    lon = (D - (1 + 2 * T1 + C1) * D * D * D / 6 +
           (5 - 2 * C1 + 28 * T1 - 3 * C1 * C1 +8 * eccPrimeSquared + 24 * T1 * T1) *
           D * D * D * D * D / 120
          ) / cos(phi1Rad);
    lon = ProjCS.central_meridian + lon * ProjCS.rad2deg;        // LongOrigin + lon * rad2deg;

//jss20191014
/*
#ifdef _DEBUG
        double lat_orig = lat;
        double lon_orig = lon;
#endif

    // se for Copel, troca datum (não precisa mais, pois o serviço já está no datum certo)
    if (empresa == COPEL)
    {
        double h = 0.;
        int dIn = 1; // SAD69
        int dTo = 4; // WGS84
        DatumConvert(dIn, lat, lon, 0., dTo, lat, lon, h);
    }
*/
    return (true);
}

// ---------------------------------------------------------------------------

AnsiString __fastcall TScg_utm::PM_GetZonaUtm(void)
{
    return (UTMZone);
}

// ---------------------------------------------------------------------------
void __fastcall TScg_utm::PM_SetZonaUtm(AnsiString zona)
{
    UTMZone = zona;
}

// ---------------------------------------------------------------------------
char __fastcall TScg_utm::UTMLetterDesignator(double Lat)
{
    // This routine determines the correct UTM letter designator for the given latitude
    // returns 'Z' if latitude is outside the UTM limits of 84N to 80S
    // Written by Chuck Gantz- chuck.gantz@globalstar.com
    char LetterDesignator;

    if ((84 >= Lat) && (Lat >= 72))
        LetterDesignator = 'X';
    else if ((72 > Lat) && (Lat >= 64))
        LetterDesignator = 'W';
    else if ((64 > Lat) && (Lat >= 56))
        LetterDesignator = 'V';
    else if ((56 > Lat) && (Lat >= 48))
        LetterDesignator = 'U';
    else if ((48 > Lat) && (Lat >= 40))
        LetterDesignator = 'T';
    else if ((40 > Lat) && (Lat >= 32))
        LetterDesignator = 'S';
    else if ((32 > Lat) && (Lat >= 24))
        LetterDesignator = 'R';
    else if ((24 > Lat) && (Lat >= 16))
        LetterDesignator = 'Q';
    else if ((16 > Lat) && (Lat >= 8))
        LetterDesignator = 'P';
    else if ((8 > Lat) && (Lat >= 0))
        LetterDesignator = 'N';
    else if ((0 > Lat) && (Lat >= -8))
        LetterDesignator = 'M';
    else if ((-8 > Lat) && (Lat >= -16))
        LetterDesignator = 'L';
    else if ((-16 > Lat) && (Lat >= -24))
        LetterDesignator = 'K';
    else if ((-24 > Lat) && (Lat >= -32))
        LetterDesignator = 'J';
    else if ((-32 > Lat) && (Lat >= -40))
        LetterDesignator = 'H';
    else if ((-40 > Lat) && (Lat >= -48))
        LetterDesignator = 'G';
    else if ((-48 > Lat) && (Lat >= -56))
        LetterDesignator = 'F';
    else if ((-56 > Lat) && (Lat >= -64))
        LetterDesignator = 'E';
    else if ((-64 > Lat) && (Lat >= -72))
        LetterDesignator = 'D';
    else if ((-72 > Lat) && (Lat >= -80))
        LetterDesignator = 'C';
    else
        LetterDesignator = 'Z';
    // This is here as an error flag to show that the Latitude is outside the UTM limits

    return LetterDesignator;
}

// ---------------------------------------------------------------------------
// eof
