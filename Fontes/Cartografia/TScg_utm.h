#ifndef TScg_utmH
#define TScg_utmH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TScg.h"

// ---------------------------------------------------------------------------
class TScg_utm : public TScg
{
public:
                __fastcall TScg_utm(VTApl *apl, int empresa_id, AnsiString empresa_nome);
                __fastcall ~TScg_utm(void);
	bool 	    __fastcall ConverteUTMmesmaZona(double &x1, double &x2);
//	bool    	__fastcall NorthEast_to_LatLon(double north, double east,
//                                               double &lat, double &lon, int tipo_coordenada);
	bool    	__fastcall NorthEast_to_LatLon(double  north, double  east,
                                               double &lat,   double &lon);
	bool 	    __fastcall LatLon_to_NorthEast(double lat, double lon, double &north, double &east);

protected: // métodos acessados via property
	AnsiString 	__fastcall PM_GetZonaUtm(void);
	void 		__fastcall PM_SetZonaUtm(AnsiString zona);

private: // métodos
	bool	    __fastcall CasosEspeciais(double &UTMNorthing, double &UTMEasting,
                                          double &lat, double &lon, int tipo_coordenada);
//	void        __fastcall DatumConvert(int dIn, double LatIn, double LongIn, double HtIn, int dTo,  double& LatTo,
//							double& LongTo, double& HtTo);
	bool 		__fastcall LeArquivoZonaEmpresa(AnsiString empresa_nome);
	char 		__fastcall UTMLetterDesignator(double Lat);

	// dados locais
	AnsiString UTMZone;
};

// ---------------------------------------------------------------------------
#endif
// eof
