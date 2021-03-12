// ---------------------------------------------------------------------------
#ifndef TPreMonta2H
#define TPreMonta2H

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "TPreMonta.h"

// ---------------------------------------------------------------------------
class TPreMonta2 : public TPreMonta
{
public:
	__fastcall TPreMonta2(VTApl *apl);
	__fastcall ~TPreMonta2(void);
	bool __fastcall InsereSec_Barra(TBarraBDGD *barraBT);
	bool __fastcall InsereSec_Capacitor(TCapacitorBDGD *capBT);
	bool __fastcall InsereSec_Chave(TChaveBDGD *chaveBDGD);
	bool __fastcall InsereSec_Consumidor(TCargaBDGD *cargaBT);
    bool __fastcall InsereSec_ConsIP(TCargaIP *cargaIP);
	bool __fastcall InsereSec_Gerador(TGeradorBDGD *geradorBT);
	bool __fastcall InsereSec_Ramal(TRamalLigacao *ramal);
	bool __fastcall InsereSec_Rede(TRedeBT *redeBT);
	bool __fastcall InsereSec_TrafoMTBT(TTrafoDistribuicao *trafoMTBT);
	bool __fastcall InsereSec_Trecho(TTrechoBDGD *trechoBT);
};

#endif
// -----------------------------------------------------------------------------
// eof
