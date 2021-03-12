//---------------------------------------------------------------------------
#ifndef TProdutoH
#define TProdutoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTProduto.h"

//---------------------------------------------------------------------------
class TProduto : public VTProduto
   {
   public:  //métodos
            __fastcall  TProduto(void);
            __fastcall ~TProduto(void);

   protected://funções acessadas via property
      AnsiString __fastcall PM_GetCodigo(void);
      AnsiString __fastcall PM_GetCodLicenca(void);
      AnsiString __fastcall PM_GetCodRev(void);
      AnsiString __fastcall PM_GetCodRevSeq(void);
      AnsiString __fastcall PM_GetCodRevSeqData(void);
      AnsiString __fastcall PM_GetData(void);
      AnsiString __fastcall PM_GetDescricao(void);
      int        __fastcall PM_GetId(void);
      AnsiString __fastcall PM_GetRevisao(void);
      AnsiString __fastcall PM_GetRevisaoSeq(void);
      AnsiString __fastcall PM_GetSinapsis(void);
      AnsiString __fastcall PM_GetWinRegister(void);

   private:
      struct   {
               AnsiString revisao;
               AnsiString revisao_seq;
               AnsiString data;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
