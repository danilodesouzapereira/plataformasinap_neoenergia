//---------------------------------------------------------------------------

#ifndef TInfoRJChaveH
#define TInfoRJChaveH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTArvore;
class VTNode;
class VTBloco;
class VTChave;
// ---------------------------------------------------------------------------
class TInfoRJChave : public TObject
{

public:
				 __fastcall TInfoRJChave(void);
				 __fastcall ~TInfoRJChave(void);
	void		 __fastcall Clear(void);
	TInfoRJChave* __fastcall Add(const  TInfoRJChave *infoAdd);
	//TInfoRJChave* operator+=(const  TInfoRJChave *infoAdd);
	//TInfoRJChave* operator+( TInfoRJChave *infoResult, const  TInfoRJChave *infoAdd);

public:
	double demandaAtivaAcum_kw;
	double demandaReativaAcum_kvar;
	double potenciaNomAcum_kva;
	struct
	{
		int resid;
		int comerc;
		int indust;
	}qntdeConsuBT, qntdeConsuMT;
	struct
	{
		double resid_kwh;
		double comerc_kwh;
		double indust_kwh;
		int	n_dias;
	}consumo;
	VTNode	*nodeBlocoJus;
	VTNode	*nodeChaveJus;
	VTBloco *blocoJus;
	VTChave *chave;
	TList   *lisCargas;
	TList	*lisTrafos;
};
// ---------------------------------------------------------------------------
#endif
