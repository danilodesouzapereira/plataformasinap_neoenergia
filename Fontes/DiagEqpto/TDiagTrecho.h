//---------------------------------------------------------------------------

#ifndef TDiagTrechoH
#define TDiagTrechoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//#include <Vcl.Controls.hpp>
//#include <Vcl.StdCtrls.hpp>
//#include <Vcl.Forms.hpp>
//#include <System.Actions.hpp>
//#include <Vcl.ActnList.hpp>
//#include <Vcl.ExtCtrls.hpp>
//#include <Vcl.ImgList.hpp>
//#include <Vcl.ToolWin.hpp>
//#include <Vcl.Buttons.hpp>
//#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
//opções para diagnostico de trecho
enum opDIAG_TRECHO {
                     opTchCOMP=0,
                     opTchIADM,
                     opTchZSEQ,
                     opTchTENSOES,
                     opTchCOUNT};
//texto referente às opções
const AnsiString op_TCH_TEXT [opTchCOUNT] = {
                                         "Comprimento [m]",
                                         "Corrente admissível [A]",
                                         "Impedâncias de sequência [ohm/km]",
                                         "Tensões iguais nas barras"};

//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_TRECHO {
                     colTch_SEGMENTO=0,
                     colTch_REDE,
                     colTch_EQPTO,
                     colTch_CODIGO,
                     colTch_TIPOERRO,
                     colTch_CODBARRA1,
                     colTch_CODBARRA2,
                     colTch_DESCRICAO,
                     colTch_COUNT};
//---------------------------------------------------------------------------
class VTTrecho;
class TFormDiagTrecho;
//---------------------------------------------------------------------------
class DIAG_TRECHO : public TObject
   {
   public:
       int        tipo_rede;
       AnsiString nome_tipo;
       double     comp_min_m;
       double     comp_max_m;
       double     iadm_min_a;
       double     iadm_max_a;
       double     seq0_r_max_ohmkm;
       double     seq0_r_min_ohmkm;
       double     seq0_x_max_ohmkm;
       double     seq0_x_min_ohmkm;
       double     seq0_c_max_nFkm;
       double     seq0_c_min_nFkm;
       double     seq1_r_max_ohmkm;
       double     seq1_r_min_ohmkm;
       double     seq1_x_max_ohmkm;
       double     seq1_x_min_ohmkm;
       double     seq1_c_max_nFkm;
       double     seq1_c_min_nFkm;
   };
//---------------------------------------------------------------------------
class TDiagTrecho : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagTrecho(VTApl *apl);
                  __fastcall  ~TDiagTrecho(void);
      void        __fastcall  Inicia(void);
      void        __fastcall  IniciaLog(TStringList *log);
      bool        __fastcall  Executa(void);
      bool        __fastcall  LeArquivo(void);
      bool        __fastcall  SalvaArquivo(void);
      void        __fastcall  ShowFormCfg(void);
      int         __fastcall  ShowFormCfgModal(void);
      void        __fastcall  PreencheListView(TListView *listView);
      int         __fastcall  Tipo(void);
      AnsiString  __fastcall  TipoAsString(void);
	   void		   __fastcall  IniciaCheckList(TCheckListBox *checkListBox);
      void        __fastcall  LeCheckList(TCheckListBox *checkListBox);

   public: //Dados internos
   /* TODO : mudar pra dados acessados por property */
      //TList       *LisCfgDiagTrecho;
      TStringList *log_trecho;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoTrechos;

   public: //property
      __property bool   OpcaoDiagCompri   = {read=dgTrecho.comp,  write=dgTrecho.comp};
      __property bool   OpcaoDiagIadm     = {read=dgTrecho.iAdm,  write=dgTrecho.iAdm};
      __property bool   OpcaoDiagZseq     = {read=dgTrecho.zSeq,  write=dgTrecho.zSeq};
      __property bool   OpcaoDiagTensoes  = {read=dgTrecho.tensoes,  write=dgTrecho.tensoes};

   private: //User declarations
      void           __fastcall  AddLinhaLog(VTTrecho *trecho,int opDIAG_TRECHO);
      DIAG_TRECHO*   __fastcall  CriaCfgTrechoDefault(int redeSEGMENTO, AnsiString nomeTipo);
      bool           __fastcall  ExisteCfgTipoRede(int redeSEGMENTO);
      DIAG_TRECHO*   __fastcall  Get_DiagTrecho(int redeSEGMENTO);
      void           __fastcall  VerificaTiposRede(void);
      void           __fastcall  VerificaTrecho(VTTrecho *trecho);
      void           __fastcall  VerificaOpComp(VTTrecho *trecho, DIAG_TRECHO *cfgDiagTrecho);
      void           __fastcall  VerificaOpIAdm(VTTrecho *trecho, DIAG_TRECHO *cfgDiagTrecho);
      void           __fastcall  VerificaOpTensoes(VTTrecho *trecho);
      void           __fastcall  VerificaOpZSeq(VTTrecho *trecho, DIAG_TRECHO *cfgDiagTrecho);

   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      TFormDiagTrecho *formDiagTrecho;

   protected: //dados acessados por property
      struct   {
               bool  comp;
               bool  iAdm;
               bool  zSeq;
               bool  tensoes;
               }dgTrecho;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);
};
//---------------------------------------------------------------------------
//eof
#endif
