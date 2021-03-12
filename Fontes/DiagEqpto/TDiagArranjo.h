//---------------------------------------------------------------------------

#ifndef TDiagArranjoH
#define TDiagArranjoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "VTDiagEqpto.h"
//---------------------------------------------------------------------------
//opções para diagnostico de arranjo
enum opDIAG_ARRANJO {
                     opArjCABOFASES,
                     opArjCOUNT
                     };
//colunas do log
enum col_LOG_ARRANJO {
                     colArj_EQPTO = 0,
                     colArj_CODIGO,
                     colArj_TIPOERRO,
                     colArj_DESCRICAO,
                     colArj_COUNT};
//---------------------------------------------------------------------------
class TFormDiagArranjo;
class VTArranjo;
//---------------------------------------------------------------------------
class DIAG_ARRANJO : public TObject
   {
   public:
       int        tipo_rede;
       AnsiString nome_tipo;

   };
//---------------------------------------------------------------------------
class TDiagArranjo : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagArranjo(VTApl *apl);
                  __fastcall  ~TDiagArranjo(void);
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
      TStringList *log_arranjo;

      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoArranjos;

   public: //property
      __property bool   OpcaoDiagCaboFase   = {read=dgArranjo.caboFase,  write=dgArranjo.caboFase};

   private: //User declarations
      void           __fastcall  AddLinhaLog(VTArranjo *arranjo,int opDIAG_ARRANJO);
      //DIAG_ARRANJO*  __fastcall  CriaCfgArranjoDefault(int redeSEGMENTO, AnsiString nomeTipo);
      //bool           __fastcall  ExisteCfgTipoRede(int redeSEGMENTO);
      //DIAG_ARRANJO*  __fastcall  Get_DiagArranjo(int redeSEGMENTO);
      void           __fastcall  VerificaArranjo(VTArranjo *arranjo);
      void           __fastcall  VerificaOpCaboFase(VTArranjo *arranjo, DIAG_ARRANJO *diagArranjo = NULL);


   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      //TFormDiagEqptoArranjo *formDiagEqptoArranjo;

   protected: //dados acessados por property
      struct   {
               bool  caboFase;
               }dgArranjo;
   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif

