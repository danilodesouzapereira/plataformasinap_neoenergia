//---------------------------------------------------------------------------
#ifndef TAvaliaAcoesChaveH
#define TAvaliaAcoesChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaAcoesEqpto.h"
//---------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTChave;
//---------------------------------------------------------------------------
class TAvaliaAcoesChave : public VTAvaliaAcoesEqpto
{
	public:  //métodos
		__fastcall  TAvaliaAcoesChave(VTApl *apl);
		__fastcall ~TAvaliaAcoesChave(void);

	protected:
		AnsiString __fastcall DeterminaTipoSemelhanca(VTAcao *acao);

	private:
		bool       __fastcall MesmoEqpto(VTChave *chave1, VTChave *chave2);
		bool       __fastcall MesmoEstado(VTChave *chave1, VTChave *chave2);
		AnsiString __fastcall NomeTipoAcao(VTAcao *acao);
		AnsiString __fastcall StringTipoChave(VTChave *chave, bool nova);
		AnsiString __fastcall StringTipoRede(VTChave *chave);
		int 	   __fastcall TipoAlteracao(VTAcao *acao);


	//define de strings para tentar facilitar
    #define tipoAaSUBST "Substituição por: "
	#define tipoAaMANOBRA "Manobra : "
	//enum para definir tipos de alteracao de chave
	enum tipoTAR {ttarINDEF=-1, ttarSUBST=1,  ttarMANOBRA};


};

//---------------------------------------------------------------------------
#endif
//eof
