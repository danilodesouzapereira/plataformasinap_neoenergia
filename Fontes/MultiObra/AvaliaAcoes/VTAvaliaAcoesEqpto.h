//---------------------------------------------------------------------------
#ifndef VTAvaliaAcoesEqptoH
#define VTAvaliaAcoesEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTCurva;
class VTEqbar;
class VTEqpto;
class VTLigacao;
//---------------------------------------------------------------------------
class VTAvaliaAcoesEqpto : public TObject
{
	public:  //métodos
		__fastcall  VTAvaliaAcoesEqpto(VTApl *apl);
		__fastcall ~VTAvaliaAcoesEqpto(void);
	bool    __fastcall ValidaTipoEqpto(VTAcao *acao, int tipoEQPTO);
	bool    __fastcall MesmaCurva(VTCurva *curva1, VTCurva *curva2);
	bool    __fastcall MesmoLocal(VTLigacao *lig1, VTLigacao *lig2);
	bool    __fastcall MesmoLocal(VTEqbar *eqbar1, VTEqbar *eqbar2);
	VTEqpto* __fastcall DeterminaEqpto(VTAcao *acao);
	virtual AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao) = 0;

	protected:


	protected:
		VTApl *apl;

};
// -----------------------------------------------------------------------------
// função global
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesTrecho(VTApl *apl);
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesRegulador(VTApl *apl);
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesTrafo(VTApl *apl);
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesRede(VTApl *apl);
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesChave(VTApl *apl);
VTAvaliaAcoesEqpto* __fastcall NewObjAvaliaAcoesCapacitor(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
