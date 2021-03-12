//---------------------------------------------------------------------------
#ifndef VTConversorH
#define VTConversorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TArranjoBDGD;
struct strOPImporta;
class EImportaUgrid;
class VTImportador;

//---------------------------------------------------------------------------
enum destinoINDEX
{
	destINDEX_MEMORIA = 0,
	destINDEX_BASEUNICA,
	destINDEX_BASESEPARADA
};
//---------------------------------------------------------------------------
class VTConversor : public TObject
   {
   public:  //property
      __property AnsiString TNSname  = {read=PD.TNSname,  write=PD.TNSname};
      __property AnsiString Username = {read=PD.username, write=PD.username};
	  __property AnsiString Password = {read=PD.password, write=PD.password};
	  __property AnsiString NomeArquivoBaseUnica = {read = PD.nomeArquivoBaseUnica, write = PD.nomeArquivoBaseUnica};
	  __property AnsiString Pasta = {read = PD.pasta, write = PD.pasta};
	  __property bool BatchMode = {read = PD.batchMode, write = PD.batchMode, default = false};
	  __property TArranjoBDGD* CondutorDefaultMT = {read=condutor_default.mt, write=condutor_default.mt};
	  __property TArranjoBDGD* CondutorDefaultBT = {read=condutor_default.bt, write=condutor_default.bt};
	  __property TArranjoBDGD* CondutorDefaultRL = {read=condutor_default.rl, write=condutor_default.rl};
	  __property TArranjoBDGD* CondutorDefaultIP = {read=condutor_default.ip, write=condutor_default.ip};
	  __property TList*    LisSubSelecionada = {read=lisSUB_SEL, write=lisSUB_SEL};
	  __property bool ArqEgrid = {read=PD.arqEgrid, write=PD.arqEgrid};
	  __property VTImportador *Importador = {read=PD.importador, write=PD.importador};

   public:
				   __fastcall  VTConversor(void){};
	  virtual      __fastcall ~VTConversor(void){};
	  virtual bool __fastcall  ImportaCommons(AnsiString fileName) throw (EImportaUgrid) = 0;
      virtual bool __fastcall  ImportaRedes(TList *lisSUBESTACAO,  strOPImporta *opcao, int destino) = 0;

   protected:
      struct{
				AnsiString TNSname;
				AnsiString username;
				AnsiString password;
				AnsiString nomeArquivoBaseUnica;
				AnsiString pasta;
				bool batchMode;
				bool arqEgrid;
				VTImportador *importador;
			}PD;
			struct{
				TArranjoBDGD *mt;
				TArranjoBDGD *bt;
				TArranjoBDGD *rl;
				TArranjoBDGD *ip;
			}condutor_default;
			TList *lisSUB_SEL;

   protected: // métodos
//	void __fastcall PM_SetImportador(VTImportador *importadorArquivo);
//	VTImportador* __fastcall PM_GetImportador(void);
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTConversor
//-----------------------------------------------------------------------------
VTConversor* NewObjConversor(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
