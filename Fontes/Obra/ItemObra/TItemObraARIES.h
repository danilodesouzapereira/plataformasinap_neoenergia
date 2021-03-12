//---------------------------------------------------------------------------
#ifndef TItemObraARIESH
#define TItemObraARIESH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTItemObraARIES.h"
#include <map>
//---------------------------------------------------------------------------
class VTAcao;
class VTPadrao;
class VTEqpto;
//---------------------------------------------------------------------------
class TItemObraARIES : public VTItemObraARIES
{
	public:
			__fastcall  TItemObraARIES(VTApl *apl);
			__fastcall ~TItemObraARIES(void);
		VTItemObra* __fastcall Clone(bool preserveId, bool isDeep);
		bool __fastcall  ContabilizaCusto(void);
		bool __fastcall  ContabilizaQuantidadeInsercao(void);
		bool __fastcall  ContabilizaQuantidadeRetirada(void);
		bool __fastcall  ContabilizaQuantidades(void);
		void __fastcall  CopiaAtributosDe(VTItemObra &ref);
		void __fastcall  CorrigeValores(int tipoEqpto);
        void __fastcall  IniciaExtras(void);
        void __fastcall  LeIpeAtributo(VTIpeAtributo *atributo);
		void __fastcall  MantemAtributosComunsDe(VTItemObra &ref);
		void __fastcall  SalvaIpeAtributosProprios(void);


	private:
        bool    __fastcall ChaveManobrada(VTAcao* acao);
//		double  __fastcall  QuantidadeEqpto(VTAcao *acao, VTPadrao *padrao, bool inclusao);
		double  __fastcall  QuantidadeEqpto(VTEqpto *eqptoFinal, VTPadrao *padrao);
		bool 	__fastcall PadraoesTemTipoEqptoIguais(VTPadrao *padrao1, VTPadrao *padrao2);

	protected:
		//Getters
		VTPadrao* __fastcall PM_GetPadraoIns(void);
		VTPadrao* __fastcall PM_GetPadraoRet(void);
		//Setters
		void __fastcall PM_SetPadraoIns(VTPadrao *padrao);
		void __fastcall PM_SetPadraoRet(VTPadrao *padrao);

	private:
		std::map<VTEqpto*,VTEqpto*>mapOrigFinalINS;
		std::map<VTEqpto*,VTEqpto*>mapOrigFinalRET;

        std::map<VTEqpto*,VTAcao*>mapAcao;

		VTPadrao    *padraoIns;
		VTPadrao    *padraoRet;
};

#endif
//---------------------------------------------------------------------------
//eof
