//---------------------------------------------------------------------------
#ifndef VTBuscaProtH
#define VTBuscaProtH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTBlocos;
class VTBloco;
class VTChave;
class VTEqpto;
class VTRede;
class VTBlocoRad;
class VTVisRede;
class VTBarraRad;
class VTDefeito;
class VTLigacao;
//---------------------------------------------------------------------------
struct strIF : TObject //hk201710IF
					 { //Objeto para guardar informações em eq. de protecao e chaves secc com IF
					 VTChave *chave, *chaveProt;
                bool   flagIF;  //se tem IF instalado
					 double compProt;
					 double compIF;
                TList *lisBlJus;
                TList *lisBlProt;
					 };
//---------------------------------------------------------------------------
class VTBuscaProt: public TObject
	{
	public:  //property
		__property double      CargaBlocoJus[VTChave* chave][int np] = {read=PM_GetCargaBlocoJus};
		__property VTBlocos*   Blocos              = {read=PM_GetBlocos};
		__property bool        FlagInicia          = {read=PM_GetFlagInicia};
		__property TList*      LisBlocoRad         = {read=PM_GetLisBlocoRad};
		__property TList*      LisChvVis           = {read=PM_GetLisChavesVis};
		__property TList*      LisVisRede          = {read=PM_GetLisVisRede};
		__property TList*      LisTipoEqProt       = {read=PM_GetLisTipoEqProt};
		__property TList*      LisChavesMed        = {read=PM_GetLisChavesMed};
		__property TList*      LisDjPri            = {read=PM_GetLisDJPri};
		__property TList*      LisChvAuto          = {read=PM_GetLisChvAuto};
		__property TList*      LisChavesAtivas     = {read=PM_GetLisChvAtivas};
		__property int         IProfReliga         = {read=PM_GetIProfReliga, write=PM_SetIProfReliga};
		__property int         IProfDescoord       = {read=PM_GetIProfDescoord, write=PM_SetIProfDescoord};
		__property TList*      LisEQIF             = {read=PM_GetLisEQIF};
		__property TList*      LisChavesIF         = {read=PM_GetLisChavesIF};
		//__property TList*      LisChavesAtivas     = {write=PM_SetLisChvAtivas};
		//__property TList*      LisBlocoJus         = {read=PM_GetLisBlocoJus};
		//__property TList*      LisBlocoMon         = {read=PM_GetLisBlocoMon};

	public:
									__fastcall VTBuscaProt(void) {};
									__fastcall ~VTBuscaProt(void) {};
		  virtual    	 void __fastcall BuscaBlocoAgreg(VTBloco *bloco, TList *lisEXT) = 0;
		  virtual		 bool __fastcall ChaveReligadora(VTChave *pchv) = 0;
		  virtual VTBlocoRad* __fastcall IdentificaBlocoRad(VTBloco *bloco) = 0;
		  virtual VTBlocoRad* __fastcall IdentificaBlocoRad(VTEqpto *eqpto) = 0;
		  virtual	VTBloco* __fastcall IdentificaBloco(VTEqpto *eqpto) = 0;
		  virtual VTVisRede* __fastcall IdentificaVisRede(VTChave *pchv, TList *lisBlocosDesl = NULL) = 0;  //hk201805: rede pai-filha: não transferencia para blocos desligados
		  virtual	    bool __fastcall CalculaCargasBlocoRad(int iPat = -1) = 0;
		  virtual	  double __fastcall CargaLisBlocosMan(VTDefeito *pdef, int iPat) = 0;
		  virtual	  double __fastcall FolgaIA(VTChave *pchvVis, int indPat) = 0;
		  virtual	  double __fastcall FolgaIA(VTVisRede *visRede, int indPat) = 0;
		  virtual  		 void __fastcall IdentificaChvMont(VTEqpto *eqpto, TList *lisExt, bool flagProt) = 0;  //Identifica chaves a montante do defeito
		  virtual  		 void __fastcall IdentificaChvMont(VTBloco *bloco, TList *lisExt, bool flagProt) = 0;  //Identifica chaves a montante do defeito
		  virtual  		  int __fastcall IndiceChave(VTChave *pchv) = 0;
		  virtual  		 bool __fastcall Inicia(bool flagExeBlocos = true) = 0;
		  virtual		 void __fastcall ListaBlocoRadTotal(TList *lisEXT) = 0;
		  virtual	  double __fastcall IA_Crit(VTChave *pchvVis, int indPat) = 0;
		  virtual	  double __fastcall IA_Crit(VTVisRede *visRede, int indPat) = 0;
		  virtual   VTEqpto* __fastcall Liga_Crit(VTChave *pchvVis, int indPat) = 0;  //201609: Restr.cap.Chaves
		  virtual		 bool __fastcall LisBlManLis(VTDefeito *pdef) = 0;
		  virtual		 bool __fastcall LisBlJus(VTChave *pchv, TList *lisEXTBloco) = 0;
		  virtual     TList* __fastcall LisBlJusChave(VTChave *pchv) = 0; //Retorna blocos a jusante da chave
		  virtual VTBarraRad* __fastcall LisBarProt(VTBloco *bloco) = 0;
		  virtual       bool __fastcall LisBarChave(VTChave *pchave, TList *lisBarra) = 0;
		  virtual		 bool __fastcall LisBlProt(VTChave *pchv, TList *lisEXTBloco) = 0;
		  virtual  		 bool __fastcall LisBlVisMan(VTBloco *blocoDef, TList *lisEXTBloco) = 0;
		  virtual		 void __fastcall LisBlRadTerm(VTBloco *bloco, TList *lisEXTBlocoRad) = 0;
		  virtual  		 void __fastcall LisChaveOrd(TList *lisExt) = 0;
		  virtual  		 bool __fastcall LisChvBloco(VTBloco *bloco, TList *lisExt) = 0;
		  virtual  		 void __fastcall LisBlocosMed(VTChave *pchave, TList *lisExt, TList *lisExtFixo = NULL) = 0;//Retorna blocos a jusante e a chave de medicao seguinte
		  virtual       bool __fastcall LisLigaVis(VTChave *chv, VTBarra *pbarra, TList *lisExt, TList *lisBlocosExt = NULL) = 0;
		  virtual       bool __fastcall LisLigaVis(VTBarra *pbarra1, TList *lisExt) = 0;
		  virtual  		 bool __fastcall LisLigaBloco(VTChave *chv1, VTChave *chv2, TList *lisExt) = 0;
		  virtual		 void __fastcall ListaOrdenadaBlocos(TList *lisBlocoExt, VTRede *rede = NULL) = 0;
		  virtual		 void __fastcall LisBlocosVis(VTRede *rede, TList *lisBlocoExt) = 0;
		  virtual		 void __fastcall LisChavesVis(VTRede *rede, TList *lisChaveExt) = 0;
		  virtual		 void __fastcall LisChavesProt(VTRede *rede, TList *lisChaveExt) = 0;//Retorna chaves dos equipamentos de protecao ate o suprimento
		  virtual		 bool __fastcall ListaZonaProt(VTChave *pchv, TList *lisBlocoExt) = 0;
		  virtual		 bool __fastcall ListaZonaProtJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt = NULL) = 0;
		  virtual		 bool __fastcall ListaBlocoRadZonaProtJus(VTChave *pchv, TList *lisBlocoRadExt) = 0;//hk201802a
		  virtual		 bool __fastcall ListaZonaProtIFJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt = NULL) = 0; //hk201710IF
		  virtual	    bool __fastcall ListaZonaReligaJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt = NULL) = 0;//, TList *lisBarrasExt);
		  virtual	  TList* __fastcall ListaZonaReliga(VTChave *pchv) = 0;
		  virtual		 void __fastcall ListaLigaVisTotal(TList *lisChvVis, TList *lisLigaVis, TList *lisLigaVisDef) = 0;
		  virtual		 void __fastcall LisChvJus(VTBloco *bloco, VTChave *chvMont, TList *lisChvJus) = 0;
		  virtual       bool __fastcall LisBarChaveZonaProt(VTChave *pchave, TList *lisBarra) = 0; //n14
		  virtual       bool __fastcall PreparaDefeito(VTDefeito *pdef) = 0;//hkedp, TList *lisChavesAtivas = NULL) = 0;
		  virtual		 void __fastcall LisLigaJusChave(VTChave *chave, TList *lisLigaJus) = 0;
		  virtual		 bool __fastcall ChavesSerieMax(VTRede *rede, TList *lisTipoChaves, int ncLim, AnsiString &chaveSerie) = 0;
		  virtual		 bool __fastcall LisChavesDmax(double dMax, TList *lisChavesDmax) = 0;
		  virtual		 void __fastcall ReiniciaZonaReliga(void) = 0;
	     virtual       void __fastcall PreparaIndicacaoFalta(void) = 0; //hk2018: reformula: hk201710IF
		  //virtual  		 void __fastcall LisBlocoChave(VTBloco *bloco, TList *lisExt) = 0;
		  //virtual  	  TList* __fastcall LisBlChave(VTChave *pchv) = 0; //Retorna blocos a jusante da chave
		  //virtual  		 bool __fastcall OrdenaChaves(void) = 0;
		  //virtual  		 bool __fastcall LisBlMan(VTBloco *blocoDef, TList *lisEXTBloco, TList *lisEXTBlocoVis, TList *lisEXTChv) = 0;
        //virtual		 bool __fastcall LisBlManDefeito(VTDefeito *pdef) = 0;

	protected: //métodos acessados via property
		 virtual double    __fastcall PM_GetCargaBlocoJus(VTChave* chave, int np) = 0;
		 virtual VTBlocos* __fastcall PM_GetBlocos(void) = 0;
		 virtual bool      __fastcall PM_GetFlagInicia(void)    = 0;
		 virtual TList*    __fastcall PM_GetLisBlocoRad(void)   = 0;
		 virtual TList*    __fastcall PM_GetLisChavesVis(void)  = 0;
		 virtual TList*    __fastcall PM_GetLisVisRede(void)    = 0;
		 virtual TList*    __fastcall PM_GetLisTipoEqProt(void) = 0;
		 virtual TList*    __fastcall PM_GetLisChavesMed(void)  = 0;
		 virtual TList*    __fastcall PM_GetLisDJPri(void)      = 0;
		 virtual TList*    __fastcall PM_GetLisChvAuto(void)    = 0;
		 virtual TList*    __fastcall PM_GetLisChvAtivas(void)  = 0;
		 virtual int       __fastcall PM_GetIProfReliga(void)   = 0;
		 virtual void      __fastcall PM_SetIProfReliga(int iProfReliga)   = 0;
		 virtual TList*    __fastcall PM_GetLisEQIF(void)  = 0;  //hk201710IF
		 virtual TList*    __fastcall PM_GetLisChavesIF(void)  = 0;  //hk201710IF
		 //virtual void      __fastcall PM_SetLisChvAtivas(TList *lisChavesAtivas) = 0;
		 //virtual TList*    __fastcall PM_GetLisBlocoJus(void)   = 0;
		 //virtual TList*    __fastcall PM_GetLisBlocoMon(void)   = 0;
		 virtual int    __fastcall PM_GetIProfDescoord(void) = 0;  //hk201711: taxa de descoordenação de proteção
		 virtual void   __fastcall PM_SetIProfDescoord(int iProfDescoord) = 0;              //hk201711: taxa de descoordenação de proteção
	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto
//---------------------------------------------------------------------------
VTBuscaProt* __fastcall NewObjBuscaProt(VTApl* apl, bool flagCalc);

//---------------------------------------------------------------------------
#endif
//eof
