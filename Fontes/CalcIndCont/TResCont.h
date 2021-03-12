// ---------------------------------------------------------------------------
#ifndef TResContH
#define TResContH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTResCont.h"

// ---------------------------------------------------------------------------
class TResCont : public VTResCont
{
public:
	__fastcall TResCont(VTApl *apl);
	__fastcall ~TResCont(void);
	void __fastcall IniciaDicFicPu(bool flagZera);
	void __fastcall IniciaListas(VTRede *rede);
	void __fastcall IniciaResultados(VTRede *rede);
	VTResCont* __fastcall ClonaResCont(void);
	double __fastcall Indic(strResBloco *resBloco, int ind);
	void __fastcall ImprimeResBloco(AnsiString arq);
	void __fastcall ImprimeResCarga(AnsiString arq);
	void __fastcall ImprimeResGeralRede(AnsiString arq);
	// void   __fastcall LisResCargaRede(TList *lisExt, VTRede *rede);

private:
	void __fastcall IniciaListaRede(VTRede *rede);
	int __fastcall NCons(VTCarga *carga);
	int __fastcall NCons(TList *lisCargas);
	int __fastcall NConsVIP(TList *lisCargas);

public: // métodos

		protected : // métodos acessados via property
	strResBloco* __fastcall PM_GetResBloco(VTBloco *bloco, bool flagCria);
	strResCarga* __fastcall PM_GetResCarga(VTCarga *carga, TList *lisRes);
	strResRede* __fastcall PM_GetResRede(VTRede *rede);
	strResRede* __fastcall PM_GetResGeral(void);

	TList* __fastcall PM_GetLisResBloco(void)
	{
		return (lisResBloco);
	};

	TList* __fastcall PM_GetLisResRede(void)
	{
		return (lisResRede);
	};

	TList* __fastcall PM_GetLisResCarga(void)
	{
		return (lisResCarga);
	};
	TList* __fastcall PM_GetLisResBlocoRede(VTRede *rede);
	TList* __fastcall PM_GetLisResCargaRede(VTRede *rede);
	TList* __fastcall PM_GetLisResCargaBloco(VTBloco *bloco);
	// AnsiString __fastcall PM_GetCodigo(void);
	// void       __fastcall PM_SetCodigo(AnsiString codigo);

private: // dados externos

	VTApl *apl;
	// dados locais
	strResRede *resGeral;
	TList *lisResBloco;
	TList *lisResRede;
	TList *lisResCarga;
	TList *lisTemp; // lista local destruida pela classe TResCont
	// dados locais
	};

#endif
// ---------------------------------------------------------------------------
