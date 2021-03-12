//---------------------------------------------------------------------------
#ifndef TDiagBIsoH
#define TDiagBIsoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
#define TIPOSEQPTO_COUNT (eqptoMOTOR + 1)
//---------------------------------------------------------------------------
//opções para diagnostico de BlocoIsolado
enum opDIAG_ISOLADO {
                  opBIsoELETRIC = 0,
                  opBIsoDEBUG,
                  opBIsoCOUNT
                  };
//texto referente às opções
const AnsiString op_BISO_TEXT [opBIsoCOUNT] = {
                                         "Eletricamente Isolado",
                                         "DEBUG",
                                         };
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_ISOLADO {
                     colBIso_CODIGO = 0,
                     colBIso_TIPOERRO,
                     colBIso_DESCRICAO,
                     colBIso_COUNT};
//---------------------------------------------------------------------------
class VTBarra;
class VTArea;
//---------------------------------------------------------------------------
class DIAG_ISOLADO : public TObject
   {
   public:
       int        id;
       AnsiString codigo;
       bool       finalizado;
       TList      *lisBarras;
       TList      *lisLigacoes;
	   TList      *lisEqpto;
       bool       isolado;

   public: //User declarations
                  __fastcall  DIAG_ISOLADO(int id, AnsiString codigo);
                  __fastcall  ~DIAG_ISOLADO(void);
      void        __fastcall  Finaliza(void);

   private:
      TList*      __fastcall  ConcatenaLisEqpto(void);
   };
//---------------------------------------------------------------------------
class TDiagBIso : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagBIso(VTApl *apl);
                  __fastcall  ~TDiagBIso(void);
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
      TStringList *log_biso;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoBlocoIsolados;

   public: //property
      __property bool   OpcaoDiagEletric   = {read=dgBlocoIsolado.eletric,  write=dgBlocoIsolado.eletric};

   private: //User declarations
      void           __fastcall  AddLinhaLog(DIAG_ISOLADO *blocoIso,int opDIAG_ISOLADO);
      void           __fastcall  ListaBarrasLigadas(VTBarra *barra, DIAG_ISOLADO *blocoIso);
      TList*         __fastcall  ListaBlocos(void);
      bool           __fastcall  VerificaRelevanciaBloco(DIAG_ISOLADO *bloco);
      void           __fastcall  TagBarrasComGeracao(void);

   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      //long     tipoEqptoIsolado [TIPOSEQPTO_COUNT]; //vetor com todos os possiveis tipos de equipamentos
      TList    *listaBlocosIsolados;
      TList    *lisBarrasIsoladas;

   protected: //dados acessados por property
      struct   {
               bool  eletric;
               }dgBlocoIsolado;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif
