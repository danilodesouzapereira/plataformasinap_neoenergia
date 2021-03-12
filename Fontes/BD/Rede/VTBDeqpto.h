//---------------------------------------------------------------------------
#ifndef VTBDeqptoH
#define VTBDeqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCorte;
class VTDominio;
class VTEstudo;
class VTLock;
class VTMRede;
class VTManobra;
class VTRede;
class VTTipoRede;
class VTSNPDBConn;

//---------------------------------------------------------------------------
class VTBDeqpto : public TObject
   {
   public:
                   __fastcall  VTBDeqpto(void) {};
      virtual      __fastcall ~VTBDeqpto(void) {};
      virtual bool __fastcall  CarregaEstudo(VTEstudo *estudo) = 0;
      virtual bool __fastcall  CarregaMRedeEstudo(VTEstudo *estudo, TList *lisMREDE, TList *lisMREDE_VIZ) = 0;
      virtual bool __fastcall  CarregaRedeEstudo(VTEstudo *estudo) = 0;
      //virtual bool __fastcall  CarregaRedeEstudo(VTEstudo *estudo, TList *lisREDE) = 0;
      //virtual bool __fastcall  CarregaReducaoEstudo(VTEstudo *estudo, VTMRede *mrede, VTCorte *corte) = 0;
      virtual bool __fastcall  CriaEstudoNovo(VTEstudo *estudo, TList *lisREDE) = 0;
      virtual bool __fastcall  LeListaCargaNL(VTEstudo *estudo, TList *lisREDE, TList* lisCargaNL, bool bolObra) = 0;
      virtual bool __fastcall  LeListaCorte(VTEstudo *estudo, TList *lisMREDE, TList *lisCORTE=NULL) = 0;
      virtual bool __fastcall  LeListaDominio(void) = 0;
      virtual bool __fastcall  LeListaEstudo(void) = 0;
      virtual bool __fastcall  LeListaMRede(VTEstudo *estudo, TList *lisCORTE) = 0;
      virtual bool __fastcall  LeListaRede(VTEstudo *estudo, VTDominio *dominio) = 0;
      virtual bool __fastcall  LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede) = 0;
      virtual bool __fastcall  LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ) = 0;
      virtual int  __fastcall  LeQtdeCargaRede(int rede_id) = 0;
      virtual bool __fastcall  RemoveDominio(VTEstudo *estudo, VTDominio *dominio) = 0;
      virtual bool __fastcall  RemoveEstudo(VTEstudo *estudo) = 0;
      virtual bool __fastcall  RemoveEstudoRede(VTEstudo *estudo, VTRede *rede) = 0;
      virtual bool __fastcall  RemoveManobra(VTManobra* manobra) = 0;
      virtual bool __fastcall  RemoveSincronismo(VTLock* lock) = 0;
      virtual bool __fastcall  SalvaAlternativaEstudo(VTEstudo *estudo) = 0;
      virtual bool __fastcall  SalvaListaCargaNL(VTEstudo *estudo, TList* lisCargaNL, bool bolObra) = 0;
      virtual bool __fastcall  SalvaListaDominio(TList *lisDOM) = 0;
      virtual bool __fastcall  SalvaRedeEstudo(VTEstudo *estudo) = 0;
      virtual bool __fastcall  SalvaRedeEstudoComo(VTEstudo *estudo) = 0;
      virtual bool __fastcall  ValidaVersaoBaseParaGravacao(void) = 0;
	  virtual bool __fastcall  ValidaVersaoBaseParaLeitura(void) = 0;
	  virtual AnsiString  __fastcall  VersaoBase(void) = 0;

	protected:
		#define              BASE_REV_ATUAL "425"
		#define              BASE_REV_425   "425"
		#define              BASE_REV_424   "424"
		#define              BASE_REV_423   "423"
		#define              BASE_REV_422   "422"
		#define              BASE_REV_421   "421"
		#define              BASE_REV_42x   "420"
		#define              BASE_REV_41x   "410"
		#define              BASE_REV_4x    "40"
		#define              BASE_REV310    "310"
    };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422298
//https://sinapsisenergia.teamwork.com/#tasks/14422299
//https://sinapsisenergia.teamwork.com/#tasks/14422301
VTBDeqpto* NewObjBDeqptoAccess310(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess40(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess410(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess420(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess421(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess422(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess423(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoAccess424(VTApl *apl_owner, VTSNPDBConn *bd_conn); //igual ao 423, só houve a mudança das figuras
VTBDeqpto* NewObjBDeqptoAccess425(VTApl *apl_owner, VTSNPDBConn *bd_conn);
VTBDeqpto* NewObjBDeqptoOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession);

#endif
//---------------------------------------------------------------------------
//eof