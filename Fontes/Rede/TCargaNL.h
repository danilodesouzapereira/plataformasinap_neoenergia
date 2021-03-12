//---------------------------------------------------------------------------
#ifndef TCargaNLH
#define TCargaNLH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCargaNL.h"

//---------------------------------------------------------------------------
class TCargaNL : public VTCargaNL
   {
   public:
                 __fastcall  TCargaNL(void);
                 __fastcall ~TCargaNL(void);
      VTCargaNL* __fastcall  Clone(void);
      bool       __fastcall  DefineFundamental(strFUN *str_fun);
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
		bool       __fastcall  InsereHarmonica(strHRM *str_hrm);
		strFUN*    __fastcall  LeFundamental(void);
		strHRM*    __fastcall  LeHarmonica(int index);
		int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   protected: //métodos acessados via property
      double __fastcall PM_GetFatPot_pu(void);
      bool   __fastcall PM_GetIndutiva(void);
      int    __fastcall PM_GetNumHarmonicas(void);
      //
      void __fastcall PM_SetFatPot_pu(double fatpot_pu);
      void __fastcall PM_SetIndutiva(bool indutiva);

   private: //dados locais
      bool                 indutiva;
      double               fatpot_pu;
      strFUN               fundamental;
      DynamicArray<strHRM> harmonica;
   };

//---------------------------------------------------------------------------
#endif
//eof

 