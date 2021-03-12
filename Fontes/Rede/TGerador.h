//---------------------------------------------------------------------------
#ifndef TGeradorH
#define TGeradorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTGerador.h"

//---------------------------------------------------------------------------
class TGerador : public VTGerador
   {
   public:
                 __fastcall  TGerador(int num_pat);
                 __fastcall ~TGerador(void);
      VTGerador* __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void       __fastcall  DefineCurva(VTCurva *nova_curva);
      VTCurva*   __fastcall  ExisteCurva(void);
      double     __fastcall  FornecimentoMVA(int num_pat);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   protected: //métodos acessados via property
      bool       __fastcall PM_GetDefinidoPorFase(void);
      int        __fastcall PM_GetFases(void);
      int        __fastcall PM_GetFonte(void);
      AnsiString __fastcall PM_GetFonteStr(void);
      double     __fastcall PM_GetSnom_mva(void);
      double     __fastcall PM_GetVnom_kv(void);
      //
      void        __fastcall PM_SetFases(int fases);
      void        __fastcall PM_SetFonte(int tipo);
      void        __fastcall PM_SetSnom_mva(double snom_mva);
      void        __fastcall PM_SetVnom_kv(double vnom_kv);

   protected: //dados acessados via property
      struct   {
               int    fonte;
               int    fases;
               double vnom_kv;
               double snom_mva;
               }PD;

   };

//---------------------------------------------------------------------------
#endif
//eof

 