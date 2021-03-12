//---------------------------------------------------------------------------
#ifndef TNivelTensaoH
#define TNivelTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class TFaixaTensao;
class TRelatorioTensao;

//---------------------------------------------------------------------------
enum nvTENSAO {nvAT=0, nvMT, nvBT};

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TNivelTensao : public TObject
{
    public:
						__fastcall  TNivelTensao(VTApl *apl, int nivel);
						__fastcall ~TNivelTensao(void);
		void            __fastcall  Executa(TRelatorioTensao *relatorio_tensao);
		TList*  		__fastcall  LisFaixa(void);
		TNivelTensao* 	__fastcall  Clone(void);

	private:
        double __fastcall DeterminaTensao(VTBarra *barra);
		void   __fastcall IniciaLisFaixa(void);
		void   __fastcall PreencheFaixa(TFaixaTensao *faixa);
		void   __fastcall PreencheMap(double v_inf, double v_sup);

	private: //dados locais
	   TList *lisFAIXA;
	   int nivel;
	   std::map<VTBarra*, double> tensao_barras;

	private: //dados externos
	   VTApl *apl;
	   TRelatorioTensao *relatorio_tensao;
};

//---------------------------------------------------------------------------
#endif
