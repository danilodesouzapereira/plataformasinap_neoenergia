//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesTrechoH
#define TAvaliaAcoesTrechoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaAcoesEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTAcao;
class VTTrecho;
//---------------------------------------------------------------------------
class TAvaliaAcoesTrecho : public VTAvaliaAcoesEqpto
{
	public:  //métodos
		__fastcall  TAvaliaAcoesTrecho(VTApl *apl);
		__fastcall ~TAvaliaAcoesTrecho(void);

	protected:
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao);

	private:
        VTArranjo* __fastcall ArranjoFinal(VTAcao *acao);
		AnsiString __fastcall NomeArranjo(VTArranjo *arranjo);
		AnsiString __fastcall NomeTipoAcao(VTAcao *acao);
		AnsiString __fastcall NomeRefaseamento(VTAcao *acao);
		int        __fastcall TipoTrocaDeArranjo(VTAcao *acao);
		AnsiString __fastcall TrataNomeArranjo(VTArranjo *arranjo);
		bool       __fastcall MesmoArranjo(VTTrecho *trecho1, VTTrecho *trecho2);
		bool       __fastcall MesmoComprimento(VTTrecho *trecho1, VTTrecho *trecho2);
		bool       __fastcall MesmaFase(VTTrecho *trecho1, VTTrecho *trecho2);
		bool       __fastcall MesmoNumFase(VTTrecho *trecho1, VTTrecho *trecho2);

	//define de strings para tentar facilitar
	#define tipoAaRECONDUT "Recondutoramento: "
    #define tipoAaREFASEAM "Refaseamento: "
	#define tipoAaINSERIR "Inserção: "
	#define tipoAaRETIRAR "Retirada : "
	//enum para definir tipos de altera de trecho
	enum tipoTAT {ttatINDEF=-1, ttatRECOND=1, ttatGIROFASE, ttatINSERIR};


};

//---------------------------------------------------------------------------
#endif
//eof
