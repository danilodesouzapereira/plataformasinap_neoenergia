//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesH
#define TAvaliaAcoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTEqbar;
class VTRede;
class TNodeDetalhe;
class VTArvore;
//---------------------------------------------------------------------------
class TAvaliaAcoes : public TObject
{
	public:  //métodos
		__fastcall  TAvaliaAcoes(VTApl *apl);
		__fastcall ~TAvaliaAcoes(void);

		bool __fastcall MontaArvore(TList *lisAcoes, VTArvore *arvoreEXT);
			
	private:
		void __fastcall AvaliaNode(TNodeDetalhe *node);
		void __fastcall SeparaNivel0(TList *lisAcoes, TList *lisEXT_nodeDetalhe); //nivel Obra
		void __fastcall SeparaNivel1(TNodeDetalhe *node); //nivel Tipo Eqpto
		void __fastcall SeparaNivel2(TNodeDetalhe *node); //nivel Semelhanca
		void __fastcall SeparaNivel3(TNodeDetalhe *node); //nivel Rede
		void __fastcall SeparaNivel4(TNodeDetalhe *node); //nivel Eqpto
//        bool    __fastcall ValidaTipoEqpto(VTAcao *acao, int tipoEQPTO);
		int __fastcall DeterminaTipoEqpto(VTAcao *acao);
		VTRede* __fastcall DeterminaRede(VTAcao *acao);
		VTRede* __fastcall DeterminaRede(VTEqbar *eqbar);
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao, int tipoEqpto);
        void    __fastcall InsereAcoes(VTAcao *acao, TList *lisEXT);

	protected: //dados externos
        VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
//eof
