//-----------------------------------------------------------------------------
#ifndef TEqpH
#define TEqpH

//-----------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTMsg;

//---------------------------------------------------------------------------
class TEqp : public TObject
   {
   public:
           __fastcall  TEqp(int tipo_eqpto, AnsiString cod_eqpto);
           __fastcall ~TEqp(void);
      void __fastcall  InsereMsg(VTMsg *msg);

   public:
      bool       enabled;
      int        id;
      int        tipo;
      AnsiString codigo;
      TList      *lisMSG;
   };

#endif
//---------------------------------------------------------------------------
//eof
