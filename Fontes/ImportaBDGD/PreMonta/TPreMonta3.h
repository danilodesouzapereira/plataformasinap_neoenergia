// ---------------------------------------------------------------------------
#ifndef TPreMonta3H
#define TPreMonta3H

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "TPreMonta.h"

// ---------------------------------------------------------------------------
class TPreMonta3 : public TPreMonta
{
public:
	__fastcall TPreMonta3(VTApl *apl);
	__fastcall ~TPreMonta3(void);
	bool __fastcall InsereSec_Barra(TBarraBDGD *barraBT);       // OK
	bool __fastcall InsereSec_Capacitor(TCapacitorBDGD *capBT);
	bool __fastcall InsereSec_Chave(TChaveBDGD *chaveBDGD);
	bool __fastcall InsereSec_Consumidor(TCargaBDGD *cargaBT);
    bool __fastcall InsereSec_ConsIP(TCargaIP *cargaIP);
	bool __fastcall InsereSec_Gerador(TGeradorBDGD *geradorBT);
    bool __fastcall  InsereSec_Ramal(TRamalLigacao *ramal);
	bool __fastcall InsereSec_Rede(TRedeBT *redeBT);                    // OK
	bool __fastcall InsereSec_TrafoMTBT(TTrafoDistribuicao *trafoMTBT);   // OK
	bool __fastcall InsereSec_Trecho(TTrechoBDGD *trechoBT);    // OK

private:
	bool __fastcall InsereSec_Cap_Paralelo(TCapacitorBDGD *cap);
	bool __fastcall InsereSec_Cap_Serie(TCapacitorBDGD *cap);

};

#endif
// -----------------------------------------------------------------------------
// eof
