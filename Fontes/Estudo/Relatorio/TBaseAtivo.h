//---------------------------------------------------------------------------
#ifndef TBaseAtivoH
#define TBaseAtivoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBaseAtivo.h"

//---------------------------------------------------------------------------
class VTAcao;
class VTBarra;
class VTEqpto;
class VTObra;
class VTRede;

//---------------------------------------------------------------------------
class TBaseAtivo : public VTBaseAtivo
   {
   public:
              __fastcall  TBaseAtivo(void);
              __fastcall ~TBaseAtivo(void);
     void     __fastcall  Inicia(VTAlternativa *alternativa);
     void     __fastcall  InsereAtivo(VTAtivo *ativo);
     TList*   __fastcall  LisAtivo(void);
     int      __fastcall  NumeroDeAtivos(void);

   private: //métodos
      bool     __fastcall AlteracaoAfetaBaseAtivo(VTEqpto *eqpto_ori, VTEqpto *eqpto_alt);
      VTAtivo* __fastcall CriaAtivo(int tipo_acao, VTObra *obra, VTEqpto *eqpto);
      VTRede*  __fastcall DeterminaRedeDaBarra(VTBarra *barra);
      VTRede*  __fastcall DeterminaRedeDoEqpto(VTEqpto *eqpto);
      int      __fastcall DeterminaTipoDoAtivo(VTEqpto *eqpto);
		VTAtivo* __fastcall ExisteAtivo(int tipo_acao, VTObra *obra, VTEqpto *eqpto);
		void     __fastcall TrataTipoAcao(VTAcao *acao, VTObra *obra);

   private: //dados externos
      VTAlternativa *alternativa;

   private: //dados locais
      TList *lisATIVO;
   };

#endif
//---------------------------------------------------------------------------
//eof

