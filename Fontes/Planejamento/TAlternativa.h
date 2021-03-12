//---------------------------------------------------------------------------
#ifndef TAlternativaH
#define TAlternativaH

//----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAlternativa.h"

//----------------------------------------------------------------------------
//class

//----------------------------------------------------------------------------
class TAlternativa : public VTAlternativa
   {
   public:
           __fastcall  TAlternativa(VTApl *apl);
           __fastcall ~TAlternativa(void);
      void __fastcall  CopiaObras(VTAlternativa *alternativa);
      void __fastcall  ReconfiguraRede(int ano);
      void __fastcall  ReconfiguraRedeAteObra(VTObra *obra);
      void __fastcall  ReconfiguraRedeSemObra(void);
      void __fastcall  ReconfiguraRedeTodaObra(VTObra *obra);
      void __fastcall  RetiraObraSemAcao(void);

   protected:    //métodos acessados via property
      bool __fastcall PM_GetFixa(void);
	  void __fastcall PM_SetFixa(bool fixa);
	  bool __fastcall PM_GetAlterado(void);
      void __fastcall PM_SetAlterado(bool alterado);
   };

//---------------------------------------------------------------------------
#endif
//eof
