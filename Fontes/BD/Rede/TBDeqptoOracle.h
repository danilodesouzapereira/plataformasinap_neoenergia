//---------------------------------------------------------------------------
#ifndef TBDeqptoOracleH
#define TBDeqptoOracleH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>
#include "VTBDeqpto.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------
class TBDfunc;
class VTArranjos;
class VTBDgeral;
class VTComentario;
class VTClasses;
class VTCronometro;
class VTCurva;
class VTCurvas;
class VTDominio;
class VTEqpto;
class VTLock;
class VTLog;
class VTManobra;
class VTMonta;
class VTProgresso;
class VTRedes;

//---------------------------------------------------------------------------
class TBDeqptoOracle : public VTBDeqpto
   {
   public:
           __fastcall  TBDeqptoOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession);
           __fastcall ~TBDeqptoOracle(void);
      bool __fastcall  CarregaEstudo(VTEstudo *estudo);
      bool __fastcall  CarregaMRedeEstudo(VTEstudo *estudo, TList *lisMREDE, TList *lisMREDE_VIZ);
      bool __fastcall  CarregaRedeEstudo(VTEstudo *estudo);
      bool __fastcall  CriaEstudoNovo(VTEstudo *estudo, TList *lisREDE);
      bool __fastcall  LeListaCargaNL(VTEstudo *estudo, TList *lisREDE, TList* lisCargaNL, bool bolObra);
      bool __fastcall  LeListaCorte(VTEstudo *estudo, TList *lisMREDE, TList *lisCORTE=NULL);
      bool __fastcall  LeListaDominio(void);
      bool __fastcall  LeListaEstudo(void);
      bool __fastcall  LeListaMRede(VTEstudo *estudo, TList *lisCORTE);
      bool __fastcall  LeListaRede(VTEstudo *estudo, VTDominio *dominio);
      bool __fastcall  LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede);
      bool __fastcall  LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ);
      int  __fastcall  LeQtdeCargaRede(int rede_id);
      bool __fastcall  RemoveDominio(VTEstudo *estudo, VTDominio *dominio);
      bool __fastcall  RemoveEstudo(VTEstudo *estudo);
      bool __fastcall  RemoveEstudoRede(VTEstudo *estudo, VTRede *rede);
      bool __fastcall  RemoveManobra(VTManobra* manobra);
      bool __fastcall  RemoveSincronismo(VTLock* lock);
      bool __fastcall  SalvaAlternativaEstudo(VTEstudo *estudo);
      bool __fastcall  SalvaListaCargaNL(VTEstudo *estudo, TList* lisCargaNL, bool bolObra);
      bool __fastcall  SalvaListaDominio(TList *lisDOM);
      bool __fastcall  SalvaRedeEstudo(VTEstudo *estudo);
      bool __fastcall  SalvaRedeEstudoComo(VTEstudo *estudo);
	  bool __fastcall  ValidaVersaoBaseParaGravacao(void);
	  bool __fastcall  ValidaVersaoBaseParaLeitura(void);
	  AnsiString __fastcall VersaoBase(void);

   private:  //objetos externos
      VTApl          *apl;
      TOracleSession *bd_session;
      VTLog          *plog;
      VTRedes        *redes;
      VTMonta        *monta;
      VTBDgeral      *bd_geral;
      TBDfunc        *bd_func;

   private:  //objetos locais
      VTCronometro *cronometro;
      VTProgresso  *progresso;

   private:  //métodos locais
      bool __fastcall CarregaEqptosRede(VTEstudo *estudo, VTRede *rede, bool bolObra);
      bool __fastcall CarregaRedeEstudo(VTEstudo *estudo, TList *lisREDE);
      bool __fastcall CarregaReducaoEstudo(VTEstudo *estudo);
      bool __fastcall CarregaAlternativaEstudo(VTEstudo *estudo);
      bool __fastcall CarregaReducaoEstudo(VTEstudo *estudo, TList *lisMREDE);
      bool __fastcall SalvaReducoes(VTEstudo *estudo);
      bool __fastcall RedefineIDAlternativa(VTEstudo* estudo);
      bool __fastcall TrataLixeira(VTEstudo* estudo);

   private:  //métodos locais
      bool          __fastcall SalvaReducoesEstudoZero(VTEstudo *estudo);
      bool          __fastcall SalvaReducoesEstudoNaoZero(VTEstudo *estudo);
      bool          __fastcall SPSNP_rdRede(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdRedeDominio(VTEstudo* estudo, VTDominio *dominio);
      bool          __fastcall SPSNP_rdRedeCursor(VTEstudo* estudo, TOracleQuery* ptrCursor, TList *lisRedeAux = NULL);
      bool          __fastcall SPSNP_rdRedeVizinha(VTEstudo* estudo, int intRedeId, TList *lisVIZ);
      bool          __fastcall SPSNP_rdBarra(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdBarraCursor(VTEstudo* estudo, TOracleQuery* ptrCursor);
      bool          __fastcall SPSNP_rdTrecho(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);

      bool          __fastcall SPSNP_rdCarga(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdCargaIP(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);;
      bool          __fastcall SPSNP_rdCargaNL(VTEstudo* estudo, int intRedeId, bool bolObra);
      bool          __fastcall SPSNP_rdCapacitor(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdDominio(void);
      bool          __fastcall SPSNP_rdGerador(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdMedidor(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdFiltro(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdReator(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdTrafoZZ(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdCanal(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdSuprimento(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdCapacitorSerie(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdMutua(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdChave(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdRegulador(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdTrafo(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdTrafo3E(VTEstudo* estudo, int intRedeId = IDINDEFINIDO, bool bolObra = false);
      bool          __fastcall SPSNP_rdEstudo(void);
      bool          __fastcall SPSNP_rdEstudoAlternativa(VTEstudo* estudo);
      bool          __fastcall SPSNP_rdCorteMRede(VTEstudo* estudo, TList *lisMRede, TList *lisCORTE);
      bool          __fastcall SPSNP_rdMRedeCorte(VTEstudo* estudo, TList *lisCORTE);
      bool          __fastcall SPSNP_rdRedeMRede(VTEstudo* estudo, int intMRedeId, TList *lisRedeAux);

      bool          __fastcall SPSNP_wrAgrupamento(VTEstudo *estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrDominio(TList* lisDominio, bool bolObra = false);
      bool          __fastcall SPSNP_wrRede(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrRedeBarraIni(VTEstudo* estudo, int intRedeId, int intBarraIniId);
      bool          __fastcall SPSNP_wrBarra(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrBarraRede(VTEstudo* estudo, int intRedeId, TList* lisObjRede);
      bool          __fastcall SPSNP_wrTrecho(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrCarga(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrCargaIP(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrCargaNL(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrCapacitor(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrGerador(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrMedidor(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrCanal(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrFiltro(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrReator(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrTrafoZZ(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrSuprimento(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrCapacitorSerie(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrMutua(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrChave(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrRegulador(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrTrafo(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrTrafo3E(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra = false);

      bool          __fastcall SPSNP_dlEstudo(VTEstudo* estudo, bool bolRedeInclusive);
      bool          __fastcall SPSNP_dlEstudoCorte(VTEstudo* estudo, TList* lisMRede);
      bool          __fastcall SPSNP_wrEstudoCorteNaoZero(VTEstudo* estudo, TList* lisMRede);
      bool          __fastcall SPSNP_wrEstudoCorteZero(VTEstudo* estudo, TList* lisMRede);
      bool          __fastcall SPSNP_wrRRCarga(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrRRSuprimento(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrRRYRef(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra = false);
      bool          __fastcall SPSNP_wrRRTrafo(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra = false);

      bool          __fastcall SPSNP_rdRRBarra(VTEstudo* estudo, int intMRedeId, int intCorteId);
      bool          __fastcall SPSNP_rdRRCarga(VTEstudo* estudo, int intMRedeId, int intCorteId);
      bool          __fastcall SPSNP_rdRRSuprimento(VTEstudo* estudo, int intMRedeId, int intCorteId);
      bool          __fastcall SPSNP_rdRRTrafo(VTEstudo* estudo, int intMRedeId, int intCorteId);
      bool          __fastcall SPSNP_rdRRYRef(VTEstudo* estudo, int intMRedeId, int intCorteId);

      bool          __fastcall SPSNP_wrConjCurvaEquipamento(VTEstudo* estudo, VTEqpto* eqpto, VTCurva* curva);

      bool          __fastcall SPSNP_wrEstudo(VTEstudo* estudo);
      bool          __fastcall SPSNP_wrEstudoEqpto(VTEstudo* estudo);
      bool          __fastcall SPSNP_wrEstudoAlternativa(VTEstudo *estudo);
      bool          __fastcall SPSNP_wrAlternativaObra(VTEstudo *estudo);
      bool          __fastcall SPSNP_wrObraAcao(VTEstudo *estudo);
      bool          __fastcall SPSNP_wrObraAtribExt(VTEstudo *estudo);

      bool          __fastcall SPSNP_dlDominio(VTEstudo *estudo, VTDominio* dominio);
      bool          __fastcall SPSNP_dlRedeDominio(VTEstudo *estudo, VTRede* rede);
      bool          __fastcall SPSNP_wrRedeDominio(VTEstudo *estudo, VTRede* rede, VTDominio* dominio);

      bool          __fastcall SPSNP_dlEstudoRede(VTEstudo *estudo, AnsiString strRedeNome);
      bool          __fastcall SPSNP_dlEstudoRede(VTEstudo *estudo, int rede_id);

      bool          __fastcall SPSNP_dlManobra(VTManobra* manobra);
      bool          __fastcall SPSNP_dlManobraChave(VTManobra* manobra);
      bool          __fastcall SPSNP_dlSincronismo(VTLock* lock);
      bool          __fastcall SPSNP_dlSincronismoChave(VTLock* lock);
      bool          __fastcall SPSNP_rdManobra(void);
      bool          __fastcall SPSNP_rdSincronismo(void);
      bool          __fastcall SPSNP_wrManobra(VTEstudo* estudo);
      bool          __fastcall SPSNP_wrManobraChave(VTEstudo* estudo, VTManobra* manobra);
      bool          __fastcall SPSNP_wrSincronismo(VTEstudo* estudo);
      bool          __fastcall SPSNP_wrSincronismoChave(VTEstudo* estudo, VTLock* lock);

   };

#endif
//---------------------------------------------------------------------------
//eof
