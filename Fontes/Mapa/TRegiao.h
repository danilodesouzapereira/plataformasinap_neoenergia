//---------------------------------------------------------------------------
#ifndef TRegiaoH
#define TRegiaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRegiao.h"

//-----------------------------------------------------------------------------
class VTParede;

//-----------------------------------------------------------------------------
class TRegiao : public VTRegiao
   {
   public:
               __fastcall  TRegiao(VTApl *apl);
               __fastcall ~TRegiao(void);
      bool     __fastcall  CalculaCentroCarga(int ind_pat);
      VTEqpto* __fastcall  ExisteEqpto(VTEqpto *eqpto);
      int      __fastcall  LisEqpto(TList *lisEXT, int tipo_eqpto=-1);
      int      __fastcall  LisEqpto(TList *lisEXT, VTRede *rede, int tipo_eqpto=-1);
      bool     __fastcall  ReiniciaLisEqpto(VTRede *rede);
      bool     __fastcall  ReiniciaLisEqpto(VTRedes *redes);

   private: //métodos
      VTParede* __fastcall ExisteParede(VTRede *rede);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisPAREDE;
   };

//---------------------------------------------------------------------------
#endif
//eof
