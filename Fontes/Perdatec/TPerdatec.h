//---------------------------------------------------------------------------
#ifndef TPerdatecH
#define TPerdatecH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPerdatec.h"

//---------------------------------------------------------------------------
class TPerdatec : public VTPerdatec
   {
   public:
             __fastcall  TPerdatec(VTApl *apl);
             __fastcall ~TPerdatec(void);
      void   __fastcall  Clear(void);
      bool   __fastcall  Executa(TDateTime data, int dia_semana);
      bool   __fastcall  PreparaRede(TDateTime data);

   private: //métodos
	  bool __fastcall ExecutaCalculoFluxo(int dia_semana);
	  void __fastcall ValidaRedeId(void);

   private: //objetos externos
	  VTApl *apl;

   private: //objetos locais
	  TList *lisGER, *lisBAR_CLONE;
   };

#endif
//---------------------------------------------------------------------------
//eof
