//---------------------------------------------------------------------------
#ifndef TImportaRegiaoH
#define TImportaRegiaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaRegiao.h"

//-----------------------------------------------------------------------------
class VTRegiao;
class VTScg;

//-----------------------------------------------------------------------------
class TImportaRegiao : public VTImportaRegiao
   {
   public:
           __fastcall  TImportaRegiao(VTApl *apl);
           __fastcall ~TImportaRegiao(void);
      bool __fastcall  Executa(AnsiString arq_txt);

   private: //métodos
      int  __fastcall ExtraiCampos(AnsiString txt);
      void __fastcall IniciaZonaCoordenadasUtm(void);
      bool __fastcall InserePontoLatLon(double lat, double lon);
      bool __fastcall InserePontoUtm(double utm_x, double utm_y);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      VTRegiao    *regiao;
      VTScg       *scg;
      int         ReferenceEllipsoid;
      AnsiString 	UTMZone;
      TStrings    *lines, *campos;
   };

//---------------------------------------------------------------------------
#endif
//eof
