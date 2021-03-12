//---------------------------------------------------------------------------
#ifndef TExportaCargaH
#define TExportaCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTDemanda;
class VTFases;
class VTPath;
class VTRedes;

//---------------------------------------------------------------------------
class TExportaCarga : public TObject
   {
   public:  //métodos
             __fastcall  TExportaCarga(VTApl *apl);
             __fastcall ~TExportaCarga(void);
      bool   __fastcall  Executa(AnsiString filename, int ind_pat);

   private:  //métodos
      bool __fastcall GravaCarga(VTCarga *carga, int ind_pat);

   private:  //objetos externos
      VTApl     *apl;
      VTDemanda *demanda;
      VTFases   *fases;
      VTPath    *path;
      VTRedes   *redes;

   private: //dados locais
      FILE  *fp;
      TList *lisCAR;
   };

#endif
//---------------------------------------------------------------------------
//eof
