 //---------------------------------------------------------------------------
#ifndef VTDiagEqptoH
#define VTDiagEqptoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
#define str_TIPOERRO_AVISO "Aviso"
#define str_TIPOERRO_ERRO "Erro"
//---------------------------------------------------------------------------
enum tipo_DGEQPTO {
                  dgeGERAL = 0,
                  dgeBARRA,
                  dgeTRECHO,
                  dgeCHAVE,
                  dgeARRANJO,
                  dgeCABO,
                  dgeCAPACITOR,
                  dgeTRAFO,
                  dgeREG,
                  dgeCARGA,
                  dgeREDE,
                  dgeBLOCOISOLADO};
//---------------------------------------------------------------------------
class VTDiagEqpto : public TObject
   {
   public:
                           __fastcall VTDiagEqpto(void){};
      virtual              __fastcall ~VTDiagEqpto(void){};
               AnsiString  __fastcall BooleanoParaString(bool boolean);
               bool        __fastcall StringParaBool(AnsiString boolean_ptbr);
      virtual	bool		   __fastcall Executa(void) = 0;
      virtual 	void        __fastcall Inicia(void) = 0;
      virtual	void		   __fastcall IniciaCheckList(TCheckListBox *checkListBox) = 0;
      virtual	void		   __fastcall IniciaLog(TStringList *log) = 0;
      virtual	bool     	__fastcall LeArquivo(void) = 0;
      virtual  void        __fastcall LeCheckList(TCheckListBox *checkListBox) = 0;
      virtual	void		   __fastcall PreencheListView(TListView *listView) = 0;
      virtual	bool        __fastcall SalvaArquivo(void) = 0;
      virtual  void        __fastcall ShowFormCfg(void) = 0;
      virtual  int         __fastcall ShowFormCfgModal(void) = 0;
      virtual	int         __fastcall Tipo(void) = 0;
      virtual	AnsiString  __fastcall TipoAsString(void) = 0;
               bool        __fastcall VerificaLimites(double valor, double limInf, double limSup);
   public:  //dados externos

   public:  //dados locais

   public:  //dados locais
       __property bool           Executado     = {read = executado};
       __property bool           InfoExtra     = {read = infoExtra};
       __property TList*         ListCFG       = {read = listCfg};
       __property TStringList*   Log           = {read = PM_GetLog};
       __property long           Ocorrências   = {read = PM_GetOcorrencias};
       __property long           Verificados   = {read = PM_GetItensVerificados};

   private: //User declarations


   private: //Dados externos
      VTApl *apl;

   private: //Dados proprios


   protected: //dados acessados por property
      bool  executado;
      bool  infoExtra;
      bool  temErro;
      TList *listCfg;

    protected: //métodos acessados via property
      virtual long         __fastcall PM_GetItensVerificados(void)=0;
      //virtual TStringList* __fastcall PM_GetInfoExtra(void);
      virtual TStringList* __fastcall PM_GetLog(void)=0;
      virtual long         __fastcall PM_GetOcorrencias(void)=0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTDiagEqpto
//---------------------------------------------------------------------------
VTDiagEqpto* __fastcall NewObjDiagEqpto(VTApl *apl, int tipo_DGEQPTO);
//---------------------------------------------------------------------------
#endif
//eof
