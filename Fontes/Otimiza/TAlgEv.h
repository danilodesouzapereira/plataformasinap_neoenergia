//---------------------------------------------------------------------------
#ifndef TAlgEvH
#define TAlgEvH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <math.hpp>
#include <math.h>
#include <stdio.h>
#include <VCLTee.Series.hpp>
#include <PlataformaSinap\Fontes\Otimiza\VTOtimiza.h>

//---------------------------------------------------------------------------
class VTAvalia;
class VTIndividuo;

//---------------------------------------------------------------------------
class TAlgEv : public VTOtimiza
   {
   public:
                   __fastcall TAlgEv(void);
                   __fastcall ~TAlgEv(void);
      //Derivadas de VTOtimiza
      bool         __fastcall Executa(VTProgresso* prog = NULL);
		void         __fastcall Inicia(void);



		//Proprias de TAlgEv
		void         __fastcall DefChartSerie(TChartSeries *serie);
		void         __fastcall InsereIndividuo(VTIndividuo *indExt);


	private: //métodos
		void      __fastcall CalcParametros(double &a, double &b, double media, double min, double max);
		void      __fastcall EncerraRel(void);
	   bool      __fastcall ExecutaAvalia(TList* lis, bool flagImprime = false);
      void      __fastcall ExecutaCruz(TList* lisP, TList* lisF);
      void      __fastcall ExecutaCruzAE(TList* lisP, TList* lisF);
		void      __fastcall ExecutaCruzAG(TList* lisP);
      bool      __fastcall ExecutaGenese(TList* lisP, int npais);
      void      __fastcall ExecutaMutacao(TList* lisP, TList* lisF, int nfilhos);
      void      __fastcall ExecutaMutacaoAE(VTIndividuo* individuo);
      void      __fastcall ExecutaMutacaoAG(VTIndividuo* individuo);
      void      __fastcall ExecutaReprodDet(TList* lis, int n);
      void      __fastcall ExecutaReprodTorneio(TList* lis, int q, int np, VTIndividuo* ind_elite = NULL);
      void      __fastcall ExecutaSelecao(TList* lis);
		void      __fastcall ImprimeRes(TList* lisP, int nger, TChartSeries* serie);
      void      __fastcall ImprimeRel(int nger, TList* lista);
      bool      __fastcall IniciaIdade(void);
      void      __fastcall IniciaRel(AnsiString rel);
		void      __fastcall InserePonto(VTIndividuo* indiv, int nger, TChartSeries* serie);
      void      __fastcall InsereDesv(VTIndividuo* indiv, int nger, TChartSeries* serie);
      void      __fastcall InsereNota(VTIndividuo* indiv, int nger, TChartSeries* serie);
      double    __fastcall inv_normal(double u1, double u2, double media, double desvio);
      double    __fastcall Normal(double media, double desv_pad);
      void      __fastcall OrdenaLista(TList* lista);
      double    __fastcall Sorteio(void);
      void      __fastcall SorteioVar(VTIndividuo* indiv, bool flag_inic = false);

	protected:  //métodos acessados via property
	  TList*        __fastcall PM_GetLisIndiv(void);
	  VTIndividuo*  __fastcall PM_GetMelhorIndiv(void);
	  strParAg      __fastcall PM_GetParam(void);
	  VTAvalia*     __fastcall PM_GetAvalia(void);
	  void          __fastcall PM_SetAvalia(VTAvalia* avalia);
	  void          __fastcall PM_SetParam(strParAg par);


	private: //dados externos
      VTProgresso   *prog;
		VTAvalia      *avalia;
      TChartSeries   *serie1;
      TChartSeries   *serie2;
      TChartSeries   *serie3;
      TChartSeries   *serie4;

   private: //dados locais
		strParAg parAg;
		TList* lisALL;
		TList* lisPExt; //lista de indivíduos externa
		VTIndividuo* indiv_melhor;
		bool   flag_exe;
      FILE*  fout;
   };

#endif
//---------------------------------------------------------------------------
//eof

