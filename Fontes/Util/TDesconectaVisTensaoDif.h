//---------------------------------------------------------------------------

#ifndef TDesconectaVisTensaoDifH
#define TDesconectaVisTensaoDifH
#include "VTDesconectaVisTensaoDif.h"
#include <Classes.hpp>
#include <map>
//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;
class VTChave;
// ---------------------------------------------------------------------------
class TDesconectaVisTensaoDif : public VTDesconectaVisTensaoDif
{
public:
			__fastcall TDesconectaVisTensaoDif(VTApl *apl);
			__fastcall ~TDesconectaVisTensaoDif(void);
	bool  	__fastcall Executa(void);

private:
	void    __fastcall DescobreTensaoRedes(void);
	void    __fastcall DescobreTensaoRede(VTRede *rede);
	void    __fastcall DesconectaBarra(VTChave *chave, int indice_barra);
	int     __fastcall IndiceBarraFronteiraChaveVis(VTChave *chave);
	bool    __fastcall PossuiLigacaoRedeOutraTensao(VTBarra *barra, VTRede *redeOrig);
    void    __fastcall Reinicia(void);


	private:
	//dados externos
	VTApl      *apl;

	//dados internos
	TList *lisBARRA_NOVA;
	TList *lisEQPTO_MOD;
	std::map<VTRede*,double> mapRedeTensao; //map para guardar a tensão da rede
};

// ---------------------------------------------------------------------------
#endif
// eof
