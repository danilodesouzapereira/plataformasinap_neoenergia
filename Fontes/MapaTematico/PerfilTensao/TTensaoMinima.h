//---------------------------------------------------------------------------
#ifndef TTensaoMinimaH
#define TTensaoMinimaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;
class VTCaminho;
class VTEqpto;
class VTLigacao;
class VTRadial;
class VTTronco;

//---------------------------------------------------------------------------
class TResBarra: public TObject
{
public:      // User declarations
		   __fastcall  TResBarra(void);
		   __fastcall ~TResBarra(void);

public:
		   double     van_pu_min, vbn_pu_min, vcn_pu_min;
           double     vfn_pu_min;
		   double     dist_m;
		   AnsiString cod, pat_min;
           VTBarra    *barra;
};

//---------------------------------------------------------------------------
class TTensaoMinima : public TObject
{
public:      // User declarations
		   __fastcall  TTensaoMinima(VTApl *apl_owner);
		   __fastcall ~TTensaoMinima(void);
	void   __fastcall  Executa(void);
    TList* __fastcall  LisLig(void);
	TList* __fastcall  LisResBarra(void);


	struct  {//estrutura c/ opções (entrada)
			VTRede  *rede;
			VTBarra *barra_ini, *barra_fim;
			bool tronco;
			int pat;//, ano;
			} opcao;

	struct   {//estrutura c/ Eqptos inicial e final do caminho elétrico (saída)
			VTBarra *barra_ini, *barra_fim;
			} dist;

	struct  {//estrutura c/ valores de tensão minima (saída)
			double     van_pu_min, vbn_pu_min, vcn_pu_min;
            double     vab_pu_min, vbc_pu_min, vca_pu_min;
			double     dist_m;
			AnsiString pat_min;
			VTBarra    *barra_ff, *barra_fn;
			} resrede, resgeral, resgeralpat;

private:
	VTBarra* __fastcall BarraMenorTensaoFF(VTRede *rede, int ind_pat);
	VTBarra* __fastcall BarraMenorTensaoFN(VTRede *rede, int ind_pat);
	VTBarra* __fastcall BarraMenorTensaoPatFF(VTRede *rede, int &pat_min);
	VTBarra* __fastcall BarraMenorTensaoPatFN(VTRede *rede, int &pat_min);
    void     __fastcall Calcula2Barras(void);
	double   __fastcall CalculaDistancia_m(VTLigacao *ligacao);
	void __fastcall CalculaPiorTensao(void);
	void __fastcall CalculaTronco(void);
	void __fastcall CriaResBarra(VTBarra *bar_ini, TList *lisLIG);
	void __fastcall PreencheGeral(void);
	void __fastcall PreencheGeralFF(void);
	void __fastcall PreencheGeralFN(void);
	void __fastcall PreencheGeralPat(void);
	void __fastcall PreencheGeralPatFF(void);
	void __fastcall PreencheGeralPatFN(void);
	void __fastcall PreencheRede(void);
	void __fastcall PreencheRedeFF(void);
	void __fastcall PreencheRedeFN(void);

private:   //objetos externos
	VTApl     *apl;

private:
   VTCaminho *caminho;
   VTRadial  *radial;
   VTTronco  *tronco;
   TList     *lisLIG;
   TList     *lisRES_BAR;
};

//---------------------------------------------------------------------------
#endif
