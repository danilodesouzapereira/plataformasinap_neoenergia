// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
// #include "..\Rede\VTBarra.h"
// #include "..\Rede\VTChave.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTEmpresa.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Cartografia\VTScg.h"
// #include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Cartografia.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TCPEqpto.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TCPEqpto::TCPEqpto(VTApl *apl)
{
	this->apl = apl;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	// cria um VTScg
	scg = DLL_NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
}

// ---------------------------------------------------------------------------
__fastcall TCPEqpto::~TCPEqpto(void)
{
	// destrói objetos
	if (scg)
	{
		delete scg;
		scg = NULL;
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
void __fastcall TCPEqpto::ConverteUTMtoLatLon(void)
{ // variaveis locais
	double lat, lon;
	int grau, minuto;
	double segundo;
	double decimal;
	double aux;

	// scg->ZonaUTM = "25L";
//  scg->NorthEast_to_LatLon(strEQPTO_CP.coordUTM_y, strEQPTO_CP.coordUTM_x, lat, lon,
//  scg->TipoCoord);
    scg->NorthEast_to_LatLon(strEQPTO_CP.coordUTM_y, strEQPTO_CP.coordUTM_x,
                             lat, lon);
	// separa o valor em GMS
	if (lat >= 0)
	{
		strEQPTO_CP.coordGMS_lat = "N";
	}
	else
	{
		strEQPTO_CP.coordGMS_lat = "S";
	}
	if (lon >= 0)
	{
		strEQPTO_CP.coordGMS_lon = "E";
	}
	else
	{
		strEQPTO_CP.coordGMS_lon = "W";
	}
	// poe o modulo
	lat = fabs(lat);
	lon = fabs(lon);
	// grau parte inteira
	grau = lat;
	decimal = lat - grau;
	aux = 60 * decimal;
	minuto = aux;
	decimal = aux - minuto;
	segundo = 60 * decimal;
	// seta os valores de lat
	strEQPTO_CP.coordGMS_G_lat = grau;
	strEQPTO_CP.coordGMS_M_lat = minuto;
	strEQPTO_CP.coordGMS_S_lat = segundo;

	// grau parte inteira
	grau = lon;
	decimal = lon - grau;
	aux = 60 * decimal;
	minuto = aux;
	decimal = aux - minuto;
	segundo = 60 * decimal;
	// seta os valores de lat
	strEQPTO_CP.coordGMS_G_lon = grau;
	strEQPTO_CP.coordGMS_M_lon = minuto;
	strEQPTO_CP.coordGMS_S_lon = segundo;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TCPEqpto::LinhaArqCsv(bool utm)
{
	// variáveis locais
	// char       separador = ';';
	AnsiString separador = "\t";
	AnsiString txt;

	txt = strEQPTO_CP.subscriber + separador;
	// txt += strCARGA.numero + separador;
	txt += strEQPTO_CP.userData1 + separador;
	txt += strEQPTO_CP.userData2 + separador;
	txt += strEQPTO_CP.userData3 + separador;
	txt += strEQPTO_CP.userData4 + separador;
	txt += strEQPTO_CP.userData5 + separador;
	txt += strEQPTO_CP.userData6 + separador;
	txt += strEQPTO_CP.userData7 + separador;
	txt += strEQPTO_CP.userData8 + separador;
	txt += IntToStr(strEQPTO_CP.act) + separador;
	txt += IntToStr(strEQPTO_CP.fase) + separador;
	txt += IntToStr(strEQPTO_CP.area) + separador;
	txt += IntToStr(strEQPTO_CP.flag1) + separador;
	txt += IntToStr(strEQPTO_CP.flag2) + separador;
	txt += IntToStr(strEQPTO_CP.flag3) + separador;
	txt += IntToStr(strEQPTO_CP.flag4) + separador;
	txt += IntToStr(strEQPTO_CP.flag5) + separador;
	txt += IntToStr(strEQPTO_CP.flag6) + separador;
	txt += IntToStr(strEQPTO_CP.flag7) + separador;
	txt += IntToStr(strEQPTO_CP.flag8) + separador;
	txt += IntToStr(strEQPTO_CP.col) + separador;
	if (utm == true)
	{
		txt += DoubleToStr("%2.1f", strEQPTO_CP.coordUTM_x) + separador;
		txt += DoubleToStr("%2.1f", strEQPTO_CP.coordUTM_y) + separador;
	}
	else
	{
		ConverteUTMtoLatLon();
		txt += IntToStr(strEQPTO_CP.coordGMS_G_lat) + separador;
		txt += IntToStr(strEQPTO_CP.coordGMS_M_lat) + separador;
		txt += DoubleToStr("%2.1f", strEQPTO_CP.coordGMS_S_lat) + separador;
		txt += strEQPTO_CP.coordGMS_lat + separador;

		txt += IntToStr(strEQPTO_CP.coordGMS_G_lon) + separador;
		txt += IntToStr(strEQPTO_CP.coordGMS_M_lon) + separador;
		txt += DoubleToStr("%2.1f", strEQPTO_CP.coordGMS_S_lon) + separador;
		txt += strEQPTO_CP.coordGMS_lon + separador;
	}

	return (txt);
}
// ---------------------------------------------------------------------------
// eof
