//---------------------------------------------------------------------------

#ifndef TDiagCargaH
#define TDiagCargaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//opções para diagnostico de carga
enum opDIAG_CARGA {
                     opCarENERGIANULA=0,
                     opCarCONSELEVADO,
                     opCarFASES,
//                     opCarISO,
					 opCarCOUNT
                     };
//texto referente às opções
const AnsiString op_CAR_TEXT [opCarCOUNT] = {
                                         "Energia Nula",
                                         "Consumo Elevado",
										 "Fases"
//                                         "Isolada"
										 };
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_CARGA {
                     //colCar_SEGMENTO=0,
                     //colCar_REDE,
                     colCar_EQPTO=0,
                     colCar_CODIGO,
                     colCar_TIPOERRO,
                     colCar_CODBARRA,
                     colCar_DESCRICAO,
                     colCar_CLASSE,
                     //colCar_VALOR,
                     colCar_FASE,
                     colCar_CONSUMO,
                     colCar_COUNT};
//---------------------------------------------------------------------------
class VTCarga;
class TFormDiagCarga;
//---------------------------------------------------------------------------
class DIAG_CARGA : public TObject
   {
   public:
       int        classe;
       AnsiString codigo;
       double     energiaMax_mwh;

   };
//---------------------------------------------------------------------------
class TDiagCarga : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagCarga(VTApl *apl);
                  __fastcall  ~TDiagCarga(void);
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
      TStringList *log_carga;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoCargas;

   public: //property
      __property bool   OpcaoDiagConsElevado = {read=dgCarga.consElevado,  write=dgCarga.consElevado};
      __property bool   OpcaoDiagEnergiaNula = {read=dgCarga.energiaNula,  write=dgCarga.energiaNula};
      __property bool   OpcaoDiagFases       = {read=dgCarga.fases,        write=dgCarga.fases};
//      __property bool   OpcaoDiagIsolado     = {read=dgCarga.isolado,      write=dgCarga.isolado};

   private: //User declarations
      void        __fastcall  AddLinhaLog(VTCarga *carga,int opDIAG_CARGA,DIAG_CARGA *diagCarga = NULL);
      DIAG_CARGA* __fastcall  CriaCfgCargaDefault(int classe, AnsiString nomeClasse);
      bool        __fastcall  ExisteCfgTipoCarga(int classe);
      DIAG_CARGA* __fastcall  Get_DiagCarga(int classe);
      void        __fastcall  VerificaTiposClasse(void);
      void        __fastcall  VerificaCarga(VTCarga *carga);
      bool        __fastcall  VerificaOpConsumoElevado(VTCarga *carga, DIAG_CARGA *diagCarga);
      bool        __fastcall  VerificaOpEnergiaNula(VTCarga *carga);
      bool        __fastcall  VerificaOpFases(VTCarga *carga);
//      bool        __fastcall  VerificaOpIsolado(VTCarga *carga);

   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      TFormDiagCarga *formDiagCarga;
      //TFormCatalogoCar *formCatalogoCar;

   protected: //dados acessados por property
      struct   {
               bool  energiaNula;
               bool  consElevado;
               bool  fases;
//               bool  isolado;
               }dgCarga;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif
