//---------------------------------------------------------------------------
#ifndef TMedidorH
#define TMedidorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMedidor.h"

//---------------------------------------------------------------------------
class TMedidor : public VTMedidor
   {
   public:
                 __fastcall  TMedidor(void);
                 __fastcall ~TMedidor(void);
      void       __fastcall  Clear(void);
      VTMedidor* __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      VTCanal*   __fastcall  ExisteCanal(int numero);
      VTCanal*   __fastcall  ExisteCanal(AnsiString codigo);
      bool       __fastcall  ExisteCanal(VTCanal *canal);
		VTCanal*   __fastcall  ExisteCanal(VTEqpto *eqpto, int tipo_canal, int fases);
      bool       __fastcall  InsereCanal(VTCanal *canal);
      VTCanal*   __fastcall  InsereCanal(VTEqpto *eqpto, int tipo_canal, int fases);
      TList*     __fastcall  LisCanal(void);
      int        __fastcall  LisCanal(TList *lisEXT, VTEqpto *eqpto);
      int        __fastcall  LisEqptoDosCanais(TList *lisEXT);
      bool       __fastcall  RemoveCanal(VTCanal *canal);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   protected:  //métodos acessados via property
      VTBarra* __fastcall PM_GetBarra(void);
      bool     __fastcall PM_GetCanaisEnabled(void);
      void     __fastcall PM_SetBarra(VTBarra *barra);

   private: //dados locais
      TList *lisCANAL;
   };

//---------------------------------------------------------------------------
#endif
//eof


