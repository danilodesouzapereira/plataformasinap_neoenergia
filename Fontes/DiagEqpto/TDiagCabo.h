//---------------------------------------------------------------------------

#ifndef TDiagCaboH
#define TDiagCaboH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//opções para diagnostico de cabo
enum opDIAG_CABO {
                     opCabCATALOGO,
                     opCabRAIO,
                     opCabRESIST,
                     opCabRMG,
                     opCabCOUNT
                     };
//texto referente às opções
const AnsiString op_CAB_TEXT [opCabCOUNT] = {
                                         "Comparar com catálogo",
                                         "Catálogo: Raio [m]",
                                         "Catálogo: Resistência [ohm/km]",
                                         "Catálogo: RMG [m]"
                                         };
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_CABO {
                     colCab_EQPTO = 0,
                     colCab_CODIGO,
                     colCab_TIPOERRO,
                     colCab_PADRAO,
                     colCab_DESCRICAO,
                     colCab_VAL_PADRAO,
                     colCab_VAL_CABO,
                     colCab_COUNT};
//---------------------------------------------------------------------------
class TFormCatalogoCab;
class VTCabo;
//---------------------------------------------------------------------------
class DIAG_CABO : public TObject
   {
   public:
      int        id;
      int        cfg_cabo;
      AnsiString codigo;
      double     iadm_a;
      double     vmax_kv;
      double     vmin_kv;
      double     r_ohm_km;
      double     raio_m;
      double     gmr_m;
      bool       preReunido;
      double     z0_r_ohm_km;
      double     z0_x_ohm_km;
      double     z1_r_ohm_km;
      double     z1_x_ohm_km;
      double     c0_nf_km;
      double     c1_nf_km;
   };
//---------------------------------------------------------------------------
class TDiagCabo : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagCabo(VTApl *apl);
                  __fastcall  ~TDiagCabo(void);
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
      //TList       *LisCfgDiagCabo;
      TStringList *log_cabo;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoCabos;

   public: //property
      __property bool   OpcaoDiagExisteNoCatalogo   = {read=dgCabo.existeNoCatalogo,  write=dgCabo.existeNoCatalogo};
      __property bool   OpcaoDiagRaio   = {read=dgCabo.raio,  write=dgCabo.raio};
      __property bool   OpcaoDiagResist   = {read=dgCabo.resist,  write=dgCabo.resist};
      __property bool   OpcaoDiagRMG  = {read=dgCabo.rmg,  write=dgCabo.rmg};

   private: //User declarations
      void           __fastcall  AddLinhaLog(VTCabo *cabo,int opDIAG_CABO, DIAG_CABO *diagCabo = NULL);
      DIAG_CABO*     __fastcall  CriaCfgCaboDefault(int id, AnsiString codigo);
      //bool           __fastcall  ExisteCfgTipoRede(int redeSEGMENTO);
      DIAG_CABO*     __fastcall  Get_DiagCabo(int id);
      void           __fastcall  VerificaCabo(VTCabo *cabo);
      DIAG_CABO*     __fastcall  VerificaOpExisteNoCatalogo(VTCabo *cabo);
      void           __fastcall  VerificaOpRaio(VTCabo *cabo, DIAG_CABO *diagCabo);
      void           __fastcall  VerificaOpResist(VTCabo *cabo, DIAG_CABO *diagCabo);
      void           __fastcall  VerificaOpRmg(VTCabo *cabo, DIAG_CABO *diagCabo);
   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      TFormCatalogoCab *formCatalogoCab;

   protected: //dados acessados por property
      struct   {
               bool  existeNoCatalogo;
               bool  raio;
               bool  resist;
               bool  rmg;
               }dgCabo;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif

