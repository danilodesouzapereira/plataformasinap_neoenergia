//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesReguladorH
#define TAvaliaAcoesReguladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaAcoesEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTRegulador;
//---------------------------------------------------------------------------
class TAvaliaAcoesRegulador : public VTAvaliaAcoesEqpto
{
	public:  //métodos
		__fastcall  TAvaliaAcoesRegulador(VTApl *apl);
		__fastcall ~TAvaliaAcoesRegulador(void);

	protected:
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao);

	private:
		bool       __fastcall MesmoAjuste(VTRegulador *reg1, VTRegulador *reg2);
		bool       __fastcall MesmoEqpto(VTRegulador *reg1, VTRegulador *reg2);
		bool       __fastcall MesmoNumUnidades(VTRegulador *reg1, VTRegulador *reg2);
        AnsiString __fastcall NomeTipoAcao(VTAcao *acao);
		AnsiString __fastcall StringPotencia(VTRegulador *regulador);
		int 	   __fastcall TipoAlteracao(VTAcao *acao);


	//define de strings para tentar facilitar
	#define tipoAaAJUSTE "Troca de ajuste: "
    #define tipoAaSUBST "Substituição: "
	#define tipoAaINSERIR "Inserção: "
	#define tipoAaRETIRAR "Retirada : "
	//enum para definir tipos de alteracao de regulador
	enum tipoTAR {ttarINDEF=-1, ttarAJUSTE=1, ttarSUBST, ttarINSERIR};


};

//---------------------------------------------------------------------------
#endif
//eof
