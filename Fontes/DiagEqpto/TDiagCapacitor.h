//---------------------------------------------------------------------------

#ifndef TDiagCapacitorH
#define TDiagCapacitorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//opções para diagnostico de capacitor
enum opDIAG_CAPACITOR {
                     opCapCATALOGO=0,
                     opCapQMVAr,
//                     opCapISO,
					 opCapCOUNT
                     };
//texto referente às opções
const AnsiString op_CAP_TEXT [opCapCOUNT] = {
                                         "Comparar com catálogo",
                                         "Catalogo: Q [MVAr]"};
//                                         "Isolado"};
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_CAPACITOR {
                     //colCap_SEGMENTO=0,
                     //colCap_REDE,
                     colCap_EQPTO=0,
                     colCap_CODIGO,
                     colCap_TIPOERRO,
                     colCap_CODBARRA,
                     colCap_DESCRICAO,
                     colCap_COUNT};
//---------------------------------------------------------------------------
class VTCapacitor;
class TFormCatalogoCap;
//---------------------------------------------------------------------------
class DIAG_CAPACITOR : public TObject
   {
   public:
       int        id;
       AnsiString codigo;
       double     Q_MVAr;

   };
//---------------------------------------------------------------------------
class TDiagCapacitor : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagCapacitor(VTApl *apl);
                  __fastcall  ~TDiagCapacitor(void);
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
      //TList       *LisCfgDiagCapacitor;
      TStringList *log_capacitor;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoCapacitores;

   public: //property
      __property bool   OpcaoDiagExisteNoCatalogo  = {read=dgCapacitor.existeNoCatalogo,  write=dgCapacitor.existeNoCatalogo};
      __property bool   OpcaoDiagQMVAr             = {read=dgCapacitor.q_mvar,            write=dgCapacitor.q_mvar};
//      __property bool   OpcaoDiagIsolado           = {read=dgCapacitor.isolado,           write=dgCapacitor.isolado};

   private: //User declarations
      void              __fastcall  AddLinhaLog(VTCapacitor *capacitor,int opDIAG_CAPACITOR,DIAG_CAPACITOR *diagCapacitor = NULL);
      DIAG_CAPACITOR*   __fastcall  CriaCfgCapacitorDefault(int redeSEGMENTO, AnsiString nomeTipo);
      //bool              __fastcall  ExisteCfgTipoCap(int tipo_cap);
      DIAG_CAPACITOR*   __fastcall  Get_DiagCapacitor(int tipo_cap);
      //void              __fastcall  VerificaTiposRede(void);
      void              __fastcall  VerificaCapacitor(VTCapacitor *capacitor);
      DIAG_CAPACITOR*   __fastcall  VerificaOpExisteNoCatalogo(VTCapacitor *capacitor);
//      void              __fastcall  VerificaOpIsolado(VTCapacitor *capacitor);
	  void              __fastcall  VerificaOpQ_MVAr(VTCapacitor *capacitor, DIAG_CAPACITOR *diagCapacitor);


   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      //TFormDiagEqptoCapacitor *formDiagEqptoCapacitor;
      TFormCatalogoCap *formCatalogoCap;

   protected: //dados acessados por property
      struct   {
               bool  existeNoCatalogo;
               bool  q_mvar;
//               bool  isolado;
               }dgCapacitor;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif
