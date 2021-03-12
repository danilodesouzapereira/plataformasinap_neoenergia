//---------------------------------------------------------------------------
#ifndef VTImportadorH
#define VTImportadorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TRedeMT;
class TSubestacao;
struct strOPImporta;
class VTPremonta;
class VTCommons;

//---------------------------------------------------------------------------
class VTImportador: public TObject
   {
   public:  //property
	  __property bool BatchMode = {read=PD.batch, write=PD.batch};
	  __property bool ArqEgrid = {read=PD.arqEgrid, write=PD.arqEgrid};
	  __property UnicodeString GridFilePath = {read=PD.gridFilePath, write=PD.gridFilePath};

	protected:
	struct{
		bool batch;
		bool arqEgrid;
		UnicodeString gridFilePath;
		}PD;

   // Métodos
   public:
				   __fastcall  VTImportador(void) {};
	  virtual      __fastcall ~VTImportador(void) {};

	  virtual void __fastcall Close(void){};
	  virtual VTCommons* __fastcall CriaCommons(UnicodeString filepathXGRID) throw(Exception) = 0;
	  virtual void __fastcall CriaDAO(AnsiString nomeArq) = 0;
	  virtual void __fastcall DestroiDAO(void) = 0;
	  virtual void __fastcall IniciaZip(UnicodeString gridPath) = 0;
	  virtual bool __fastcall ReadCommons(void) = 0;
	  virtual void __fastcall Unzip(TList *lisTMP) = 0;
	  virtual bool __fastcall ExisteSubZip(AnsiString sigla_sub) = 0;
	  virtual TStringList* __fastcall GetSubestacaoList(void) = 0;
	  virtual bool __fastcall InsereArranjo(void){return true;};
	  virtual bool __fastcall LeRedes(TSubestacao *subestacao, strOPImporta *opcao) = 0;

	  // Métodos de leitura de tabelas
	  virtual bool __fastcall ReadCfg_TipoChave(void) = 0;
	  // Etd
	  virtual bool __fastcall ReadEtd_TabBarramento(int etd_id) = 0;
	  virtual bool __fastcall ReadEtd_TabSe(int etd_id) = 0;
	  virtual bool __fastcall ReadEtd_TabTrafoSe(int etd_id) = 0;
	  // Rede primária
	  virtual bool __fastcall ReadPri_TabCapacitor(int pri_id) = 0;
	  virtual bool __fastcall ReadPri_TabChave(int pri_id) = 0;
	  virtual bool __fastcall ReadPri_TabRede(TRedeMT *primario) = 0;
	  virtual bool __fastcall ReadPri_TabCargaMT(int pri_id) = 0;
	  virtual bool __fastcall ReadPri_TabGerador(int pri_id) = 0;
	  virtual bool __fastcall ReadPri_TabRegulador(int pri_id) = 0;
	  virtual bool __fastcall ReadPri_TabTrecho(int pri_id) = 0;
	  // Rede secundária
	  virtual bool __fastcall ReadSec_TabRede(TRedeMT *primario) = 0;
	  virtual bool __fastcall ReadSec_TabTrafoMTBT(TRedeMT *primario) = 0;
	  virtual bool __fastcall ReadSec_TabCapacitor(int sec_id) = 0;
	  virtual bool __fastcall ReadSec_TabChave(int sec_id) = 0;
	  virtual bool __fastcall ReadSec_TabConsBT(int sec_id) = 0;
	  virtual bool __fastcall ReadSec_TabConsIP(int sec_id) = 0;
	  virtual bool __fastcall ReadSec_TabGeradorBT(int sec_id) = 0;
	  virtual bool __fastcall ReadSec_TabRamalLig(int sec_id) = 0;
	  virtual bool __fastcall ReadSec_TabTrechoBT(int sec_id) = 0;

   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTImportador
//-----------------------------------------------------------------------------
VTImportador* NewObjImportadorUgrid(VTApl *apl, UnicodeString filename);
VTImportador* NewObjImportadorEgrid(VTApl *apl, UnicodeString filename);

#endif
//---------------------------------------------------------------------------
//eof
