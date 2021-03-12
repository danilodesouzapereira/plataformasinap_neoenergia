//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesCapacitorH
#define TAvaliaAcoesCapacitorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaAcoesEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTCapacitor;
//---------------------------------------------------------------------------
class TAvaliaAcoesCapacitor : public VTAvaliaAcoesEqpto
{
	public:  //métodos
		__fastcall  TAvaliaAcoesCapacitor(VTApl *apl);
		__fastcall ~TAvaliaAcoesCapacitor(void);

	protected:
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao);

	private:
		bool       __fastcall MesmoAjuste(VTCapacitor *cap1, VTCapacitor *cap2);
		bool       __fastcall MesmoEqpto(VTCapacitor *cap1, VTCapacitor *cap2);
		AnsiString __fastcall NomeTipoAcao(VTAcao *acao);
		AnsiString __fastcall StringPotencia(VTCapacitor *cap);
		int 	   __fastcall TipoAlteracao(VTAcao *acao);

	//define de strings para tentar facilitar
	#define tipoAaAJUSTE "Ajuste: "
	#define tipoAaSUBST "Substituição: "
	#define tipoAaINSERIR "Inserção: "
	#define tipoAaRETIRAR "Retirada : "
	//enum para definir tipos de altera de trecho
	enum tipoTAT {ttatINDEF=-1, ttatAJUSTE=1, ttatSUBST, ttatINSERIR, ttatRETIRAR};


};

//---------------------------------------------------------------------------
#endif
//eof
