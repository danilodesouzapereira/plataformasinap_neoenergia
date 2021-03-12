//---------------------------------------------------------------------------
#ifndef VTAjustaH
#define VTAjustaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <PlataformaSinap\Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTMedicao;
class VTAjusteFatores;
//---------------------------------------------------------------------------
//enum adTIPO_FATORES
//{
//	adtfFATOR_UNICO=0,
//	adtfFATOR_PQ,
//	adtfFATOR_PFP,
//	adtfCOUNT
//};
//struct adFATORES
//{
//    bool   ativa;
//	double multP;
//	double multQ;
//	double fp;
//	int tipo;
//	VTRede *rede;
//	VTMedicao *medicao;
//};
//---------------------------------------------------------------------------
class VTAjusta : public TObject
   {
   public:  //property
	  __property bool       BatchMode         = {write=PD.batch_mode, read=PD.batch_mode};
      __property bool       AjusteJaExecutado = {write=PD.ajuste_ja_executado,   read=PD.ajuste_ja_executado};
      __property bool       IncluirConsA4     = {write=PD.incluir_consA4,        read=PD.incluir_consA4};
      __property bool       IncluirPerda      = {write=PD.incluir_perda,         read=PD.incluir_perda};
      __property bool       LimitaAjuste      = {write=PD.limite_ajuste,         read=PD.limite_ajuste};
	  __property int        TipoFluxo         = {write=PD.tipo_fluxo,            read=PD.tipo_fluxo};
	  //__property int        TipoFatorCorrec   = {write=PD.fatores.tipo,          read=PD.fatores.tipo};
	  //__property double     FatorUnico        = {write=PD.fatcor_unico,          read=PD.fatcor_unico};
	  //__property double     FatorCorrecP      = {write=PD.fatores.multP,         read=PD.fatores.multP};
	  //__property double     FatorCorrecQ      = {write=PD.fatores.multQ,         read=PD.fatores.multQ};
	  //__property double     FatorCorrecFP     = {write=PD.fatores.fp,            read=PD.fatores.fp};
      __property double     AjusteMaximo      = {write=PD.ajuste_maximo,         read=PD.ajuste_maximo};
      __property double     ErroMaximo        = {write=PD.erro_maximo,           read=PD.erro_maximo};
      __property double     TempoMinuto       = {write=PD.tempo,                 read=PD.tempo};
      __property AnsiString Diagnostico       = {write=PD.diagnostico,           read=PD.diagnostico};
      __property bool       ProgressoEnabled  = {write=PD.prog_enabled,          read=PD.prog_enabled};
	  __property bool		PorMedicao		  = {write=PD.usa_medicao,         	 read=PD.usa_medicao};
	  __property VTAjusteFatores* Fatores	  = {								 read=fatores};
	  __property bool             Patamares[int ind_pat] = {write=PM_SetPat,           read=PM_GetPat};

   public:
                     __fastcall  VTAjusta(void) {};
      virtual        __fastcall ~VTAjusta(void) {};
      virtual bool   __fastcall  CalculaFluxo(bool prog_enabled=true) = 0;
      virtual bool   __fastcall  Executa(bool med_neg_enabled=false)  {return(false);}
      virtual bool   __fastcall  Finaliza(void)                       {return(false);}
      virtual bool   __fastcall  IniciaDados(TComponent *Owner) = 0;
      virtual TList* __fastcall  LisRedeVal(void) = 0;
	  virtual bool   __fastcall  PM_GetPat(int ind_pat) = 0;
	  virtual void   __fastcall  PM_SetPat(int ind_pat, bool enabled) = 0;
      virtual void   __fastcall  RestauraCargasOriginais(void) = 0;
	  virtual void   __fastcall  RestauraCargasRedesNaoAjustadas(void) = 0;
	  virtual void   __fastcall  RemoveMedicoesFicticias(void) = 0;

   protected:  //dados acessados via property
	  struct   {
               bool       batch_mode;
			   bool       ajuste_ja_executado;
               bool       incluir_consA4;
               bool       incluir_perda;
               bool       limite_ajuste;
			   bool       prog_enabled;
			   bool		  usa_medicao;
               int        tipo_fluxo;
               double     fatcor_unico;
               double     ajuste_maximo; //ajuste máximo das cargas
               double     erro_maximo;   //erro máximo entre medição e cálculo
               double     tempo;         //tempo máximo para executar ajuste
			   AnsiString diagnostico;
			   }PD;

//			   DynamicArray<adFATORES> fatores;
			   VTAjusteFatores *fatores;
   };

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTAjusta* __fastcall NewObjAjustaPorFase(VTApl *apl_owner);
VTAjusta* __fastcall NewObjAjustaTrifasico(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 