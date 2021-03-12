//---------------------------------------------------------------------------
#ifndef TSuprimentoH
#define TSuprimentoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTSuprimento.h"

//---------------------------------------------------------------------------
class TSuprimento : public VTSuprimento
   {
   public:
                    __fastcall  TSuprimento(int num_pat);
                    __fastcall ~TSuprimento(void);
      void          __fastcall  DefineCurva(VTCurva *curva);
      VTCurva*      __fastcall  ExisteCurva(void);
      VTSuprimento* __fastcall  Clone(void);
      void          __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void          __fastcall  ReiniciaFornecimento(double v_pu=1.);
      int           __fastcall  Tipo(void);
      AnsiString    __fastcall  TipoAsString(void);

   protected: //métodos acessados via property
      bool __fastcall PM_GetDefinidoPorFase(void);
   };

//---------------------------------------------------------------------------
#endif
//eof

 