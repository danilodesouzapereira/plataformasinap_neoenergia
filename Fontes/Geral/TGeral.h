#ifndef TGeralH
#define TGeralH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTGeral.h"

//---------------------------------------------------------------------------
class TGeral : public VTGeral
   {
   public:
                    __fastcall  TGeral(VTApl *apl);
                    __fastcall ~TGeral(void);
      void          __fastcall  CopiaAtributosDe(VTGeral &geral, bool somenteCfgFlow);
      VTPadraoRede* __fastcall  ExistePadraoRede(int padraorede_id);
	  bool          __fastcall  InserePadraoRede(VTPadraoRede* padrao);
	  bool          __fastcall  LeGeral(bool redeEmpresa);
	  bool			__fastcall 	LeLimitesTensao(void);
	  TList*        __fastcall  LisPadraoRede(void);
	  bool          __fastcall  PadroesRedesDiferentes(VTGeral &geral);
      void          __fastcall  ReiniciaValoresDefault(void);
	  bool          __fastcall  RetiraPadraoRede(VTPadraoRede* padrao);
	  bool			__fastcall	SalvaLimitesTensao(void);
      bool          __fastcall  SalvaGeral(bool redeEmpresa);
	  int			__fastcall  TipoSegmentoNivelTensao(VTBarra *barra);

   private:
	  bool 			__fastcall	LimitesTensaoDefault(void);
	  void          __fastcall  ValoresDefault(void);
	  void          __fastcall  ValoresDefaultCOPEL(void);

   private: //objetos externos
     VTApl *apl;

   private: //dados locais
	 TList* lisPADRAOREDE;
   };

#endif
//-----------------------------------------------------------------------------
// eof
