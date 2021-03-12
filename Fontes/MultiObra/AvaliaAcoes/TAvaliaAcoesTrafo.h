//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesTrafoH
#define TAvaliaAcoesTrafoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaAcoesEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTTrafo;
//---------------------------------------------------------------------------
class TAvaliaAcoesTrafo : public VTAvaliaAcoesEqpto
{
	public:  //métodos
		__fastcall  TAvaliaAcoesTrafo(VTApl *apl);
		__fastcall ~TAvaliaAcoesTrafo(void);

	protected:
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao);

	private:
		bool       __fastcall MesmoAjuste(VTTrafo *trafo1, VTTrafo *trafo2);
		bool       __fastcall MesmoEqpto(VTTrafo *trafo1, VTTrafo *trafo2);
		bool       __fastcall MesmoNumFases(VTTrafo *trafo1, VTTrafo *trafo2);
		bool       __fastcall MesmoNumUnidades(VTTrafo *trafo1, VTTrafo *trafo2);
        AnsiString __fastcall NomeTipoAcao(VTAcao *acao);
		AnsiString __fastcall StringPotencia(VTTrafo *trafo);
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
