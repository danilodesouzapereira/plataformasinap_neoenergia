//---------------------------------------------------------------------------
#ifndef TDiagChaveH
#define TDiagChaveH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"

//---------------------------------------------------------------------------
//opções para diagnostico de trecho
enum opDIAG_CHAVE {
                  opChvREDE=0,
                  opChvCOMP,
                  opChvINOM,
                  opChvTIPO,
                  opChvTENSOES,
//                  opChvISO,
				  opChvCOUNT
                  };
//texto referente às opções
const AnsiString op_CHV_TEXT [opChvCOUNT] = {
                                         "Pertence a rede",
                                         "Distância entre barras [m]",
                                         "Corrente nominal [A]",
                                         "Tipo de chave",
										 "Tensões iguais nas barras"};
//                                         "Isolada"};
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_CHAVE {
                  colChv_SEGMENTO=0,
                  colChv_REDE,
                  colChv_EQPTO,
                  colChv_CODIGO,
                  colChv_TIPOERRO,
                  colChv_CODBARRA1,
                  colChv_CODBARRA2,
                  colChv_DESCRICAO,
                  colChv_COUNT
                  };
//---------------------------------------------------------------------------
class VTChave;
class TFormDiagChave;
//---------------------------------------------------------------------------
class DIAG_CHAVE : public TObject
   {
   public:
       int        tipo_chave;
       AnsiString nome_tipo;
       double     inom_min_a;
       double     inom_max_a;
       double     comp_max_m;
   };
//---------------------------------------------------------------------------
class TDiagChave : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagChave(VTApl *apl);
                  __fastcall  ~TDiagChave(void);
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
      //TList       *LisCfgDiagChave;
      TStringList *log_chave;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         double m;
         }infoChaves;

   public: //property
      __property bool   OpcaoDiagCompri   = {read=dgChave.comp,   write=dgChave.comp};
      __property bool   OpcaoDiagInom     = {read=dgChave.iNom,   write=dgChave.iNom};
//      __property bool   OpcaoDiagIsolado  = {read=dgChave.isolado,write=dgChave.isolado};
	  __property bool   OpcaoDiagTipo     = {read=dgChave.tipo,   write=dgChave.tipo};
      __property bool   OpcaoDiagRede     = {read=dgChave.rede,   write=dgChave.rede};
      __property bool   OpcaoDiagTensoes  = {read=dgChave.tensoes,write=dgChave.tensoes};

   private: //User declarations
      void           __fastcall  AddLinhaLog(VTChave *chave,int opDIAG_CHAVE);
      DIAG_CHAVE*    __fastcall  CriaCfgChaveDefault(int chaveTIPO, AnsiString nomeTipo);
      bool           __fastcall  ExisteCfgTipoChave(int chaveTIPO);
      DIAG_CHAVE*    __fastcall  Get_DiagChave(int chaveTIPO);
      void           __fastcall  VerificaTiposChave(void);
      void           __fastcall  VerificaChave(VTChave *chave);
      void           __fastcall  VerificaOpComp(VTChave *chave, DIAG_CHAVE *diagChave);
      void           __fastcall  VerificaOpINom(VTChave *chave, DIAG_CHAVE *diagChave);
//      void           __fastcall  VerificaOpIsolado(VTChave *chave);
	  void           __fastcall  VerificaOpTipo(VTChave *chave);
      void           __fastcall  VerificaOpRede(VTChave *chave);
      void           __fastcall  VerificaOpTensoes(VTChave *chave);

   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      TFormDiagChave *formDiagChave;

   protected: //dados acessados por property
      struct   {
               bool  comp;
               bool  iNom;
               bool  tipo;
               bool  rede;
               bool  tensoes;
//               bool  isolado;
               }dgChave;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif
