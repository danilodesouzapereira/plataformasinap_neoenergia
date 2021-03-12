//---------------------------------------------------------------------------
#ifndef TDominioH
#define TDominioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTDominio.h"

//---------------------------------------------------------------------------
class TDominio : public VTDominio
   {
   public:
                 __fastcall  TDominio(void);
                 __fastcall ~TDominio(void);
      VTDominio* __fastcall  Clone(void);
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      bool       __fastcall  ExisteRede(VTRede *rede);
      void       __fastcall  InsereRede(VTRede *rede);
      TList*     __fastcall  LisRede(void);
      void       __fastcall  LisRede(TList *lisEXT);
      void       __fastcall  Reinicia(void);
      void       __fastcall  RemoveRede(VTRede *rede);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   protected: //métodos acessados via property
      TDateTime __fastcall PM_GetDataCriacao(void);
      void      __fastcall PM_SetDataCriacao(TDateTime dttDataCriacao);

   private: //locais
      TDateTime data_criacao; // Data de criação do domínio
      TList     *lisREDE;     // lista de objetos VTRede
   };

//---------------------------------------------------------------------------
#endif
//eof

