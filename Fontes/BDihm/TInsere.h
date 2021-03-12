//---------------------------------------------------------------------------
#ifndef TInsereH
#define TInsereH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTInsere.h"

//---------------------------------------------------------------------------
class VTBD;
class VTMRede;

//---------------------------------------------------------------------------
class TInsere : public VTInsere
   {
   public:
           __fastcall  TInsere(VTApl *apl_owner, TAction *ActionInsRede);
           __fastcall ~TInsere(void);
      bool __fastcall  Executa(AnsiString cod_rede);
      bool __fastcall  Executa(VTReducao *reducao);

   private:   //métodos
      //void     __fastcall DeterminaMRedesPreCarregadas(void);
      VTMRede* __fastcall ExisteMRede(AnsiString cod_rede);
      VTMRede* __fastcall ExisteMRede(int mrede_id);
      bool     __fastcall IniciaConexao(VTBD *bd);
      bool     __fastcall InsereRede(VTMRede *mrede_owner);

   private:   //dados externos
      TAction  *ActionInsRede;
      VTApl    *apl_owner;
   
   private:   //dados locais
      TList *lisSEL;
   };

//---------------------------------------------------------------------------
#endif
//eof


