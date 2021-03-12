//---------------------------------------------------------------------------
#ifndef TValidaArqCurvaH
#define TValidaArqCurvaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTClasse;
class VTCurva;

//---------------------------------------------------------------------------
class TValidaArqCurva : public TObject
	{
	public:
					__fastcall  TValidaArqCurva(VTApl *apl);
					__fastcall ~TValidaArqCurva(void);
		AnsiString  __fastcall  Executa(AnsiString arq_curva);

	private: //métodos
		VTCurva* __fastcall CriaCurvaParcial(AnsiString linha);
		bool __fastcall ExisteLimiteInferior(double valor);
		bool __fastcall ExisteLimiteSuperior(double valor);
		void __fastcall Organiza(VTClasse *classe, int dia_medicao);
		AnsiString __fastcall TrataLinha(AnsiString linha);
		bool __fastcall ValidaFaixas(void);
		bool __fastcall ValidaFaixaUnica(VTCurva *curva);

	private: //obj locais
		TStringList *linhas;
		AnsiString msg;
		TList *lisCURVA, *lisORD;

	private: //obj externo
		VTApl *apl;

	};
//---------------------------------------------------------------------------
#endif
