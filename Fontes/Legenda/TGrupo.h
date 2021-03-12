//---------------------------------------------------------------------------
#ifndef TGrupoH
#define TGrupoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTGrupo.h"

//---------------------------------------------------------------------------
class TGrupo : public VTGrupo
   {
   private: //dados acessados via property
      struct   {
               AnsiString descricao;
               VTEqpto    *eqpto;
               TList      *lisGRUPO;
               TList      *lisDADO;
               TList      *lisVETVAL;
               } PD;

   public:
           __fastcall TGrupo(VTEqpto *eqpto, AnsiString descricao);
           __fastcall ~TGrupo(void);
      void __fastcall Add(VTDado *dado);
      void __fastcall Add(VTVetval *vetval);
      void __fastcall Add(VTGrupo *grupo);

   protected: //métodos acessados via property
      AnsiString __fastcall PM_GetDescricao(void);
      TList*     __fastcall PM_GetLisDado(void);
      TList*     __fastcall PM_GetLisGrupo(void);
      TList*     __fastcall PM_GetLisVetval(void);
      VTEqpto*   __fastcall PM_GetEqpto(void);
   };
   
//---------------------------------------------------------------------------
#endif
//eof
