//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesRedeH
#define TAvaliaAcoesRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaAcoesEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTRede;
//---------------------------------------------------------------------------
class TAvaliaAcoesRede : public VTAvaliaAcoesEqpto
{
	public:  //métodos
		__fastcall  TAvaliaAcoesRede(VTApl *apl);
		__fastcall ~TAvaliaAcoesRede(void);

	protected:
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao);

	private:
		AnsiString __fastcall NomeTipoAcao(VTAcao *acao);
		AnsiString __fastcall StringInsereRede(VTRede *rede);
		AnsiString __fastcall StringTensao(VTRede *rede);
		AnsiString __fastcall TensaoRedeRadial(VTRede *rede);
		AnsiString __fastcall TensaoSubestacao(VTRede *rede);

	//enum para definir tipos de altera de rede



};

//---------------------------------------------------------------------------
#endif
//eof
