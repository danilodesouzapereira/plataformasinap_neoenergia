//---------------------------------------------------------------------------

#ifndef TDiagRedeH
#define TDiagRedeH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//#define TIPOSEQPTO_COUNT (eqptoMOTOR + 1)
#define M_SQRT_3 1.732050807568877293527
//---------------------------------------------------------------------------
//opções para diagnostico de Rede
enum opDIAG_REDE {
                  opRedeFASES = 0,
                  opRedeDEMTRAFO,
                  opRedeVNOMTRAFO,
                  opRedeREGREVER,
                  opRedeCOUNT
                  };
//texto referente às opções
const AnsiString op_REDE_TEXT [opRedeCOUNT] = {
                                         "Fases",
                                         "Demanda aprox. Trafos MT/BT",
                                         "Tensão dos Trafos",
                                         "Regulador Reversível"
                                         };
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_REDE {
                     colRede_SEGMENTO=0,
                     colRede_REDE,
                     colRede_EQPTO,
                     colRede_CODIGO,
                     colRede_TIPOERRO,
                     colRede_UTMX,
                     colRede_UTMY,
                     colRede_DESCRICAO,
                     colRede_COUNT};
//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEqpto;
class VTLigacao;
class VTPrimario;
class VTRadial;
class VTSecundario;
class VTTrecho;
//class TFormDiagEqptoRede;
//---------------------------------------------------------------------------
//class DIAG_REDE : public TObject
//   {
//   public:
//       int        tipo_rede;
//       AnsiString nome_tipo;
//   };
//---------------------------------------------------------------------------
class TDiagRede : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagRede(VTApl *apl);
                  __fastcall  ~TDiagRede(void);
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
      //TList       *LisCfgDiagRede;
      TStringList *log_rede;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoRedes;

   public: //property
      __property bool   OpcaoDiagFases    = {read=dgRede.fases,  write=dgRede.fases};
      __property bool   OpcaoDiagDemTrafo = {read=dgRede.demTrafo,  write=dgRede.demTrafo};
      __property bool   OpcaoDiagRegRever = {read=dgRede.regRever,  write=dgRede.regRever};
      __property bool   OpcaoDiagVnomTrafo = {read=dgRede.vnomTrafo,  write=dgRede.vnomTrafo};

   private: //User declarations
	  void        __fastcall  AddLinhaLog(VTEqpto *eqpto,int opDIAG_REDE, AnsiString descr =" ");
	  int         __fastcall  DeterminaFasesLigacaoPai(VTLigacao *filha);
	  bool        __fastcall  ExisteTrafo(VTSecundario * secundario);
	  //VTTrecho*   __fastcall  ExisteTrechoPai(VTLigacao *filha);
	  VTLigacao*  __fastcall  ExistePaiValido(VTLigacao *filha);
	  int         __fastcall  FasesDisponiveis(VTLigacao *ligacao, VTBarra *barraSecTerc = NULL);
	  int         __fastcall  FasesNecessarias(VTLigacao *ligacao);
	  int         __fastcall  FasesDaLigacao(VTLigacao *ligacao, VTBarra *barra);
	  void        __fastcall  LisTrafoConectaBarra(TList *lisLIG, VTBarra *barra, TList *lisEXT);
	  AnsiString  __fastcall  MediaUTM_X(VTLigacao *ligacao);
	  AnsiString  __fastcall  MediaUTM_Y(VTLigacao *ligacao);
	  void        __fastcall  ProcuraFilhas(VTLigacao *pai, TList *lisLIG, TList *lisEXT);
	  void        __fastcall  SomaScomplexoPorPatamares(VTEqpto *eqpto, TList *lisValores);
	  void        __fastcall  VerificaDemanda(VTSecundario * secundario);
	  void        __fastcall  VerificaFases(VTPrimario * primario);
	  void        __fastcall  VerificaFases(VTSecundario * secundario);
	  void        __fastcall  VerificaFases(void);
	  void        __fastcall  VerificaFasesEntradaSecundario(VTSecundario *secundario);
	  //void        __fastcall  VerificaFasesTrafo(VTTrecho *trecho_pai, VTTrafo *trafo);
	  void        __fastcall  VerificaRede(void);
	  void        __fastcall  VerificaOpFases(void);
	  void        __fastcall  VerificaOpDemTrafos(void);
	  void        __fastcall  VerificaOpRegRever(void);
	  void        __fastcall  VerificaOpVnomTrafos(void);

   private: //Dados externos
      VTApl    *apl;
      VTRadial *radial;

   private: //Dados internos
      TList        *lisBAR;
      TList        *lisEQP;
      TList        *lisLIG;
      TList        *lisTRF;

   protected: //dados acessados por property
      struct   {
               bool  fases;
               bool  demTrafo;
               bool  regRever;
               bool  vnomTrafo;
               }dgRede;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif
