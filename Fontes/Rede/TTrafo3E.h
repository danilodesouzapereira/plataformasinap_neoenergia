//---------------------------------------------------------------------------
#ifndef TTrafo3EH
#define TTrafo3EH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTTrafo3E.h"

//---------------------------------------------------------------------------
class TTrafo3E : public VTTrafo3E
   {
   public:
                 __fastcall TTrafo3E(int num_pat);
                 __fastcall ~TTrafo3E(void);
      bool       __fastcall AngulosNoSecundario(double vet_ang_grau[MAX_FASE]);
      bool       __fastcall AngulosNoTerciario(double vet_ang_grau[MAX_FASE]);
      VTTrafo3E* __fastcall Clone(void);
      void       __fastcall CopiaAtributosDe(VTEqpto &ref);
      double     __fastcall DefasagemPriSec(void);
      double     __fastcall DefasagemPriTer(void);
      bool       __fastcall ExisteLigacaoDelta(void);
      double     __fastcall PerdaNoFerroMW(void);
      void       __fastcall OrdenaBar123PriSecTer(void);
      void       __fastcall RedefineBarraPriSecTer(VTBarra *pb[]);
      bool       __fastcall RedefineDeltaComoPrimario(void);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected: //métodos virtuais redefinidos
      int        __fastcall Fases(VTBarra *barra);
      int        __fastcall NumeroBarras(void);

   protected:  //métodos acessados via property
      int        __fastcall PM_GetLigacao(void);
      AnsiString __fastcall PM_GetPadrao(void);
      int        __fastcall PM_GetTap(void);
      double     __fastcall PM_GetSbase(void);
      void       __fastcall PM_SetPadrao(AnsiString padrao);
      void       __fastcall PM_SetTap(int tap);

   private:
      void __fastcall RotacionaPriSecTer(void);

   private:
      AnsiString padrao;
   };

//---------------------------------------------------------------------------
#endif
//eof

 