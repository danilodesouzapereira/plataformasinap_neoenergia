//---------------------------------------------------------------------------
#ifndef TVetvalH
#define TVetvalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTVetval.h"

//---------------------------------------------------------------------------
class TVetval : public VTVetval
   {
   public:
                 __fastcall TVetval(int ind_atrib, AnsiString descricao, int num_pat);
                 __fastcall ~TVetval(void);
     //void       __fastcall AjustaUE_A(void);
     //void       __fastcall AjustaUE_KV(void);
     //void       __fastcall AjustaUE_MVA(void);
     //void       __fastcall AjustaUE_MVAR(void);
     //void       __fastcall AjustaUE_MW(void);
     AnsiString __fastcall DescUnid(void);

   private: //métodos acessados via property
      double     __fastcall PM_GetValor(int ind);
      AnsiString __fastcall PM_GetValorAsStr(int ind);
      void       __fastcall PM_SetValor(int ind, double valor);
      void       __fastcall PM_SetValorAsStr(int ind, AnsiString val_str);
   };
   
//---------------------------------------------------------------------------
#endif
//eof
