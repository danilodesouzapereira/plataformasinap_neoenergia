//---------------------------------------------------------------------------

#ifndef TDiagTrafoH
#define TDiagTrafoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//opções para diagnostico de trafo
enum opDIAG_TRAFO {
                     opTrfCATALOGO,
                     opTrfVNOM,
                     opTrfVTAP,
                     opTrfSNOM,
                     opTrfBANCOMONO,
                     //opTrfFASES,
                     opTrfPERDAFE,
                     opTrfIMPEDANCIA,
                     opTrfLIGINCOERENTE,
//                     opTrfISO,
					 opTrfCOUNT
                     };
//texto referente às opções
const AnsiString op_TRF_TEXT [opTrfCOUNT] = {
                                            "Comparar com catálogo",
                                            "Tensão Nominal [kV]",
                                            "Vtap [kV]",
                                            "Potência Nominal [MVA]",
                                            "Banco de Trafos. Mono.",
                                            //"Fases",
                                            "Perda no Ferro",
                                            "Impedância",
//                                            "Isolado",
											"Ligação incoerente"};
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_TRAFO {
                     colTrf_SEGMENTO=0,
                     colTrf_REDE,
                     colTrf_EQPTO,
                     colTrf_CODIGO,
                     colTrf_TIPOERRO,
                     colTrf_CODBARRA1,
                     colTrf_CODBARRA2,
                     colTrf_PADRAO,
                     colTrf_DESCRICAO,
                     colTrf_VAL_PADRAO,
                     colTrf_VAL_TRAFO,
                     colTrf_COUNT};
//---------------------------------------------------------------------------
class TFormCatalogoTrf;
class VTTrafo;
//---------------------------------------------------------------------------
//class BANCO_DELTAABERTO : public TObject
//   {
//   public:
//      VTTrafo *trafo1;
//      VTTrafo *trafo2;
//   };
////---------------------------------------------------------------------------
//class BANCO_DELTAFECHADO : public TObject
//   {
//   public:
//      VTTrafo *trafo1;
//      VTTrafo *trafo2;
//      VTTrafo *trafo3;
//   };
//---------------------------------------------------------------------------
class DIAG_TRAFO : public TObject
   {
   public:
      int        id;
      AnsiString codigo;
      double     snom_mva;
      double     vnomPri_kv;
      //double     tapPri_kv;
      double     vnomSec_kv;
      //double     tapSec_kv;
      //int        ligacaoPri;
      //int        ligacaoSec;
      double     perdaFe;
   };

//---------------------------------------------------------------------------
class TDiagTrafo : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagTrafo(VTApl *apl);
                  __fastcall  ~TDiagTrafo(void);
      bool        __fastcall  Executa(void);
      void        __fastcall  Inicia(void);
      void        __fastcall  IniciaLog(TStringList *log);
      void		   __fastcall  IniciaCheckList(TCheckListBox *checkListBox);
      bool        __fastcall  LeArquivo(void);
      void        __fastcall  LeCheckList(TCheckListBox *checkListBox);
      void        __fastcall  PreencheListView(TListView *listView);
      bool        __fastcall  SalvaArquivo(void);
      void        __fastcall  SeparaBancosMono(TList *lisTrafos, TList *lisBancoDeltaAberto, TList *lisBancoDeltaFechado);
      void        __fastcall  ShowFormCfg(void);
      int         __fastcall  ShowFormCfgModal(void);
      int         __fastcall  Tipo(void);
      AnsiString  __fastcall  TipoAsString(void);

   public: //Dados internos
   /* TODO : mudar pra dados acessados por property */
      //TList       *LisCfgDiagTrafo;
      TStringList *log_trafo;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoTrafos;

   public: //property
      __property bool   OpcaoDiagBancoMono         = {read=dgTrafo.banco,            write=dgTrafo.banco};
      __property bool   OpcaoDiagExisteNoCatalogo  = {read=dgTrafo.existeNoCatalogo, write=dgTrafo.existeNoCatalogo};
      //__property bool   OpcaoDiagFases             = {read=dgTrafo.fases,            write=dgTrafo.fases};
      __property bool   OpcaoDiagImpedancia        = {read=dgTrafo.z,                write=dgTrafo.z};
//      __property bool   OpcaoDiagIsolado           = {read=dgTrafo.isolado,          write=dgTrafo.isolado};
	  __property bool   OpcaoDiagLigIncoerente     = {read=dgTrafo.ligIncoerente,    write=dgTrafo.ligIncoerente};
      __property bool   OpcaoDiagPerdaFe           = {read=dgTrafo.perdafe,          write=dgTrafo.perdafe};
      __property bool   OpcaoDiagSnom              = {read=dgTrafo.snom,             write=dgTrafo.snom};
      __property bool   OpcaoDiagVnom              = {read=dgTrafo.vnom,             write=dgTrafo.vnom};
      __property bool   OpcaoDiagVtap              = {read=dgTrafo.vtap,             write=dgTrafo.vtap};

   private: //User declarations
      void           __fastcall  AddLinhaLog(VTTrafo *trafo,int opDIAG_TRAFO, DIAG_TRAFO *cfgDiagTrafo = NULL, AnsiString desc = NULL, int enrolamento = 0);
      DIAG_TRAFO*    __fastcall  CriaCfgTrafoDefault(int id, AnsiString codigo);
      int            __fastcall  ContaNeutros(TList *lisTrafos, int enrolamento = 0);
      bool           __fastcall  DeltaAbertoValido(VTTrafo *trafo1, VTTrafo *trafo2, int enrolamento = 0);
      bool           __fastcall  DeltaFechadoValido(VTTrafo *trafo1, VTTrafo *trafo2, VTTrafo *trafo3, int enrolamento = 0);
      //bool         __fastcall  ExisteCfgTipoRede(int redeSEGMENTO);
      DIAG_TRAFO*    __fastcall  Get_DiagTrafo(int id);
      void           __fastcall  VerificaOpBancoMono(VTTrafo *trafo1, VTTrafo *trafo2,VTTrafo *trafo3 = NULL);
      DIAG_TRAFO*    __fastcall  VerificaOpExisteNoCatalogo(VTTrafo *trafo);
      //void           __fastcall  VerificaOpFases(VTTrafo *trafo);
      void           __fastcall  VerificaOpImpedancia(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo);
//      void           __fastcall  VerificaOpIsolado(VTTrafo *trafo);
	  void           __fastcall  VerificaOpLigIncoerente(VTTrafo *trafo);
      void           __fastcall  VerificaOpPerdaFe(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo);
      void           __fastcall  VerificaOpSnom(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo);
      void           __fastcall  VerificaOpVnom(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo);
      void           __fastcall  VerificaOpVtap(VTTrafo *trafo);
      void           __fastcall  VerificaTrafo(VTTrafo *trafo);

   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      TFormCatalogoTrf *formCatalogoTrf;

   protected: //dados acessados por property
      struct   {
               bool  existeNoCatalogo;
               bool  vnom;
               bool  vtap;
               bool  snom;
               bool  banco;
               //bool  fases;
               bool  perdafe;
               bool  z;
//               bool  isolado;
               bool  ligIncoerente;
               }dgTrafo;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif

