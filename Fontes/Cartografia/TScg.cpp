// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <stdio.h>
#include <stdlib.h>
#include "TScg.h"
#include "TScg_utm.h"
#include "TScg_polyconic.h"
#include "TParseZona.h"
// #include "TEllipsoid.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\..\DLL_Inc\ImportaBDGD.h"
#include "..\..\DLL_Inc\Funcao.h"

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

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
__fastcall TScg::TScg(VTApl *apl, int empresa_id)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// guarda empresa
	empresa = empresa_id;

	// id, Ellipsoid name, Equatorial Radius, square of eccentricity
	ellipsoid[0] = TEllipsoid(-1, "Placeholder", 0, 0);
	// placeholder only, To allow array indices to match id numbers
	ellipsoid[1] = TEllipsoid(1, "Airy", 6377563, 0.00667054);
	ellipsoid[2] = TEllipsoid(2, "Australian National", 6378160, 0.006694542);
	ellipsoid[3] = TEllipsoid(3, "Bessel 1841", 6377397, 0.006674372);
	ellipsoid[4] = TEllipsoid(4, "Bessel 1841 (Nambia) ", 6377484, 0.006674372);
	ellipsoid[5] = TEllipsoid(5, "Clarke 1866", 6378206, 0.006768658);
	ellipsoid[6] = TEllipsoid(6, "Clarke 1880", 6378249, 0.006803511);
	ellipsoid[7] = TEllipsoid(7, "Everest", 6377276, 0.006637847);
	ellipsoid[8] = TEllipsoid(8, "Fischer 1960 (Mercury) ", 6378166, 0.006693422);
	ellipsoid[9] = TEllipsoid(9, "Fischer 1968", 6378150, 0.006693422);
	ellipsoid[10] = TEllipsoid(10, "GRS 1967", 6378160, 0.006694605);
	ellipsoid[11] = TEllipsoid(11, "GRS 1980", 6378137, 0.00669438);
	ellipsoid[12] = TEllipsoid(12, "Helmert 1906", 6378200, 0.006693422);
	ellipsoid[12] = TEllipsoid(13, "Hough", 6378270, 0.00672267);
	ellipsoid[14] = TEllipsoid(14, "International", 6378388, 0.00672267);
	ellipsoid[15] = TEllipsoid(15, "Krassovsky", 6378245, 0.006693422);
	ellipsoid[16] = TEllipsoid(16, "Modified Airy", 6377340, 0.00667054);
	ellipsoid[17] = TEllipsoid(17, "Modified Everest", 6377304, 0.006637847);
	ellipsoid[18] = TEllipsoid(18, "Modified Fischer 1960", 6378155, 0.006693422);
	ellipsoid[19] = TEllipsoid(19, "South American 1969", 6378160, 0.006694542);
	ellipsoid[20] = TEllipsoid(20, "WGS 60", 6378165, 0.006693422);
	ellipsoid[21] = TEllipsoid(21, "WGS 66", 6378145, 0.006694542);
	ellipsoid[22] = TEllipsoid(22, "WGS-72", 6378135, 0.006694318);
	ellipsoid[23] = TEllipsoid(23, "WGS-84", 6378137, 0.00669438); // ,
	// TEllipsoid( 24, "South American 1969_2", 6378160, 298.25)
	// cria objeto datum
	datum = new TDatum();
}

// ---------------------------------------------------------------------------
__fastcall TScg::~TScg(void)
{
	// destroi objeto
	if (datum)
	{
		delete datum;
		datum = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TScg::ConverteUTMmesmaZona(double &x1, double &x2)
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
bool __fastcall TScg::LatLon_to_FakeLatLon(double &FakeLat, double &FakeLon, double &lat,
	double &lon)
{
	int tipo_coordenada = coordLATLON;
	if(importaBDGD != NULL)
	{
	   FakeLat = double(lat + 100) * 1e5;
	   FakeLon = double(lon + 100) * 1e5;
				return (true);
    }
	switch (empresa)
	{
	case CELPE:
	case COELBA:
	case COSERN:
		{ // considera opção do usuário
			if (tipo_coordenada == coordLATLON)
			{ // converte falso UTM em lat lon
				FakeLat = double(lat + 100) * 1e5;
				FakeLon = double(lon + 100) * 1e5;
				return (true);
			}
			break;
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
void __fastcall TScg::DatumConvert
                        (double  LatIn, double  LongIn, double  HtIn,
                         double &LatTo, double &LongTo, double &HtTo)
{
    // converts LatLongHt in datum dIn, to LatLongHt in datum dTo;  2002dec: by Eugene Reimer, from PeterDana equations.
    // Lat and Long params are in degrees;  North latitudes and East longitudes are positive;  Height is in meters;
    // ==This approach to TDatum-conversion is a waste of time;  to get acceptable accuracy a large table is needed -- see NADCON, NTv2...
    // double a, ee, N, X, Y, Z, EE, p, b, t;

    double equad1, achat1, w1, n1, semi_eixo1, lat1, lon1, x1, y1, z1;
    double equad2, achat2, w2, n2, semi_eixo2, lat2, lon2, x2, y2, z2, d;

    // calcula coordenadas geocentricas cartesianas no datum 1
    achat1     = ProjCS.achat;              // datum->Achat(dIn);
    semi_eixo1 = ProjCS.semi_eixo;          // datum->SemiEixo(dIn);
    lat1       = LatIn  * ProjCS.deg2rad;
    lon1       = LongIn * ProjCS.deg2rad;

    equad1     = ProjCS.equad;              // (2.0 * achat1) - pow(achat1, 2.);
    w1         = sqrt(1. - (equad1 * pow(sin(lat1), 2.)));
    n1         = semi_eixo1 / w1;

    x1         = ((n1 + HtIn) * cos(lat1)) * cos(lon1);
    y1         = ((n1 + HtIn) * cos(lat1)) * sin(lon1);
    z1         = (n1 * (1. - equad1) + HtIn) * sin(lat1);

    // calcula coordenadas geocentricas cartesianas no datum 2
    x2         = x1 + (ProjCS.dx - ggProjCS.dx);
    y2         = y1 + (ProjCS.dy - ggProjCS.dy);
    z2         = z1 + (ProjCS.dz - ggProjCS.dz);

    // calcula coordenadas geodesicas no datum 2
    achat2     = ggProjCS.achat;
    semi_eixo2 = ggProjCS.semi_eixo;

    equad2     = ggProjCS.equad;
    lat2       = lat1;

/*    do
    {
        w2     = sqrt(1. - (equad2 * pow(sin(lat2), 2.0)));
        n2     = semi_eixo2 / w2;
        lat2   = atan(((z2 + ((n2 * equad2) * sin(lat2)))) / sqrt((x2 * x2) + (y2 * y2)));
        w2     = sqrt(1. - (equad2 * pow(sin(lat2), 2.0)));
        d      = semi_eixo2 / w2 - n2;
    }
    while (abs(d) > 0.0000000000001);
    lon2       = atan(y2 / x2);
*/

//jss20191014
//  c�digo baseado na calculadora de coordenadas do site
//  http://www.dpi.inpe.br/calcula/
//  traduzido do php
//  usado para evitar o loop acima
    double m1, h1, h2, f1, f2;
    double dx, dy, dz, da, df, dh;
    double dlat, dlon;

    h1         = HtIn;
    f1         = ProjCS.achat;
    f2         = ggProjCS.achat;
    da         = semi_eixo2 - semi_eixo1;
    df         = f2 - f1;
    dh         = 0;

    dx         = ggProjCS.dx - ProjCS.dx;
    dy         = ggProjCS.dy - ProjCS.dy;
    dz         = ggProjCS.dz - ProjCS.dz;

    m1         = semi_eixo1 * (1 - equad1) / w1 /w1 /w1;
    dlat       = (cos(lat1) * dz -
                  sin(lat1) * cos(lon1) * dx -
                  sin(lat1) * sin(lon1) * dy +
                  (semi_eixo1 * df + f1 * da) * sin(2.0 * lat1)) / (m1 + h1);
    lat2       = lat1 + dlat;
    dlon       = (cos(lon1) * dy - sin(lon1) * dx) / (n1 + h1) / cos(lat1);
    lon2       = lon1 + dlon;
    dh         = cos(lat1) * cos(lon1) * dx +
                 cos(lat1) * sin(lon1) * dy +
                 sin(lat1) * dz +
                 (semi_eixo1 * df + f1 * da) * sin(lat1) * sin(lat1) - da;

    LatTo      = lat2 * ggProjCS.rad2deg;
    LongTo     = lon2 * ggProjCS.rad2deg;
    HtTo       = HtIn + dh;
}

// ---------------------------------------------------------------------------
// eof
