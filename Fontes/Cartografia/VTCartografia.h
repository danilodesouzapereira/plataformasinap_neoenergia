//---------------------------------------------------------------------------
#ifndef VTCartografiaH
#define VTCartografiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTScg.h"

//---------------------------------------------------------------------------
//enum mapaMODO    {mapaINATIVO=0, mapaNORMAL,   mapaSATELITE, mapaHIBRIDO, mapaRELEVO};
//enum tipoCOORD {coordNONE=-1, coordUTM, coordPOLYC, coordLATLON};
//enum mapTYPE {mapaNENHUM=-1, mapaROADMAP=1, mapaSATELLITE, mapaHYBRID, mapaTERRAIN};
enum mapTYPE {mapaNENHUM=-1, mapaROADMAP=1, mapaSATELLITE, mapaHYBRID, mapaTERRAIN, mapaDXF, mapaCOPEL}; //will

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTQuad;
class VTScg;

//---------------------------------------------------------------------------
class VTCartografia : public TObject
   {
   public:  //property
	  __property double     Escala_pixel_cm = {read=PM_GetEscala_pixel_cm                      };
	  __property int        TipoMapa        = {read=PM_GetTipoMapa,        write=PM_SetTipoMapa};
	  __property AnsiString ZonaUTM         = {read=PM_GetZonaUtm,         write=PM_SetZonaUtm };

   public:
				   __fastcall  VTCartografia(void) {};
	  virtual      __fastcall ~VTCartografia(void) {};
	  virtual bool __fastcall  CarregaArea_utm(int map_type, VTArea *area_utm, VTQuad *quad_pixel, bool &disponivel, int tipo_coord=coordUTM) = 0;
	  virtual bool __fastcall  CopiaBitmap(VTArea *area_utm, TCanvas *canvas,
										   int x1, int y1, int x2, int y2, double fat_escala, int tipo_coord=coordUTM) = 0;
	  virtual bool __fastcall  DefineEscala(VTArea *area_utm, VTQuad *quad_pixel) = 0;
	  virtual void __fastcall  Redimensiona(int width, int height) = 0;

   protected: //métodos acessados via property
	  virtual double     __fastcall PM_GetEscala_pixel_cm(void) = 0;
	  virtual int        __fastcall PM_GetTipoMapa(void) = 0;
	  virtual AnsiString __fastcall PM_GetZonaUtm(void) = 0;
	  virtual void       __fastcall PM_SetTipoMapa(int map_type) = 0;
	  virtual void       __fastcall PM_SetZonaUtm(AnsiString zona) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTCartografia* __fastcall NewObjCartografia(VTApl *apl, TWinControl *parent, TAction *ActionMapaDisp);


#endif
//---------------------------------------------------------------------------
//eof

