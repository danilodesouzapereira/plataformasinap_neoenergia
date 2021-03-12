//---------------------------------------------------------------------------

#ifndef TDiagBarraH
#define TDiagBarraH

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
//opções para diagnostico de Barra
enum opDIAG_BARRA {
                  opBarCOORDNULA = 0,
                  opBarCOUNT
                  };
//texto referente às opções
const AnsiString op_BAR_TEXT [opBarCOUNT] = {
                                         "Coordenadas nulas",
                                         };
//---------------------------------------------------------------------------
//colunas do log
enum col_LOG_BARRA {
                     colBar_SEGMENTO=0,
                     colBar_REDE,
                     colBar_EQPTO,
                     colBar_CODIGO,
                     colBar_TIPOERRO,
                     colBar_UTMX,
                     colBar_UTMY,
                     colBar_DESCRICAO,
                     colBar_COUNT};
//---------------------------------------------------------------------------
class VTBarra;
//class TFormDiagEqptoBarra;
//---------------------------------------------------------------------------
class DIAG_BARRA : public TObject
   {
   public:
       int        tipo_rede;
       AnsiString nome_tipo;
       int        utm_x_m_max;
       int        utm_y_m_max;
       int        utm_x_m_min;
       int        utm_y_m_min;
   };
//---------------------------------------------------------------------------
class TDiagBarra : public VTDiagEqpto
{
   public: //User declarations
                  __fastcall  TDiagBarra(VTApl *apl);
                  __fastcall  ~TDiagBarra(void);
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
      //TList       *LisCfgDiagBarra;
      TStringList *log_barra;
      struct   {
         long  n_total;
         long  n_verif;
         long  n_ocorrencias;
         }infoBarras;

   public: //property
      __property bool   OpcaoDiagCoord   = {read=dgBarra.coord,  write=dgBarra.coord};

   private: //User declarations
      void           __fastcall  AddLinhaLog(VTBarra *barra,int opDIAG_BARRA);
      //DIAG_BARRA*    __fastcall  CriaCfgBarraDefault(int redeSEGMENTO, AnsiString nomeTipo);
      //bool           __fastcall  ExisteCfgTipoRede(int redeSEGMENTO);
      //void           __fastcall  VerificaTiposRede(void);
      void           __fastcall  VerificaBarra(VTBarra *barra);

   private: //Dados externos
      VTApl *apl;

   private: //Dados internos
      //TFormDiagEqptoBarra *formDiagEqptoBarra;

   protected: //dados acessados por property
      struct   {
               bool  coord;
               }dgBarra;

   protected: //métodos acessados via property
      long           __fastcall PM_GetItensVerificados(void);
      TStringList*   __fastcall PM_GetLog(void);
      long           __fastcall PM_GetOcorrencias(void);

};
//---------------------------------------------------------------------------
//eof
#endif
