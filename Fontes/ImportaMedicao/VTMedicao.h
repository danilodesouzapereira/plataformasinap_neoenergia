//---------------------------------------------------------------------------
#ifndef VTMedicaoH
#define VTMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTChave;
class VTCurva;
class VTMedidor;
class VTPrimario;
class VTSecundario;

//---------------------------------------------------------------------------
enum enumDIAG_MEDICAO {diagMED_OK=1,
                       diagMED_ERRO_INICIAL,
                       diagMED_ERRO_FINAL,
                       diagMED_ERRO_FLUXO,
                       diagTEMPO};

//---------------------------------------------------------------------------
class VTMedicao : public TObject
   {
   public:
						__fastcall  VTMedicao(void) {};
						__fastcall ~VTMedicao(void) {};
	  virtual bool   	__fastcall  CurvaNula(int tipo_medicao) = 0;
	  virtual bool   	__fastcall  CurvaNulaNegativa(int tipo_medicao) = 0;
	  virtual AnsiString __fastcall DiagnosticoAsStr(void) = 0;
	  virtual int    	__fastcall  FasesMed(int index) = 0;
	  virtual int    	__fastcall  FasesRede(int index) = 0;
	  virtual bool   	__fastcall  FasesValidas(void) = 0;
	  virtual double 	__fastcall  MaiorErro(void) = 0;
      virtual void   	__fastcall  Reinicia(void) = 0;

   public:
      AnsiString   cod_etd;
      //AnsiString   diagnostico;  //DVK 2014.10.07
      bool         enabled;
      int          diagnostico;
      int          ind_bar_ref;     //índice da barra de referência da medição
      int          ind_pat_dem_max;
      double       fatcor_max, erro;
      VTChave      *chave_med;      //chave onde foi feita a medição
      TList        *lisCAPACITOR;
      TList        *lisGER_SUP;
      TList        *lisCAR_AJUS;    //lista das cargas que serão ajustadas
      TList        *lisCAR_FIXA;    //lista de cargas que NÃO serão ajustadas
      VTCurva      *curva_pq;
   };

//---------------------------------------------------------------------------
VTMedicao* __fastcall NewObjMedicao(void);

//---------------------------------------------------------------------------
#endif
//eof

