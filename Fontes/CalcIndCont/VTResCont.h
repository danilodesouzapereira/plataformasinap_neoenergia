//---------------------------------------------------------------------------
#ifndef VTResContH
#define VTResContH
//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TCargaBloco;
class VTApl;
class VTCarga;
class VTBloco;
class VTRede;
class VTBlocoRad;
//---------------------------------------------------------------------------
struct strResCarga : TObject
					 {
					 VTCarga *carga;
					 VTBloco *bloco;
					 VTRede  *rede;
					 double  dic, fic, dicBT, ficBT;
					 double  endBT;
					 double  dic_pu[12], fic_pu[12];
					 double  compensacao;
					 double  ncons;
					 double dmic;
					 };
//---------------------------------------------------------------------------
struct strResBloco : TObject
					 {
					 VTBlocoRad *blocoRad;
					 VTBloco *bloco;
					 VTRede  *rede; //hk201701: problemas em redes AT
					 //TCargaBloco *cBloco;
					 double  tdesl, ndesl;       //tempos(min) e desligamentos(número/periodo) (sem x ncons) para desligamentos > 3min
					 double  end;                //end e energia não distribuida total
					 double  npiscadas;          //número total de piscadas no bloco no periodo
					 double  tdeslEND, ndeslEND; //tempos(min) e desligamentos(número/período) (sem x ncons) totais sem limitantes
					 int     ncons;              //número de consumidores do bloco
					 int     nconsVIP;           //número de consumidores vip do bloco

					 //Variáveis de desligamentos por falha na rede de BT
					 double  tdeslBTN, ndeslBTN; //tempodeslig x consumidores, deslig x consumdidores para desligamentos > 3min
					 double  endBT;              //end devido a falhas na rede BT

					 //Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
					 double    dcons;   //desliga x cons  (deslig x consumidores)
					 double    tcons;   //tdesligax cons  (min x consumidores)
					 double    dconsPisca; //piscadas x nons (piscadas x consumidores)
					 double    dconsPiscaVIP; //piscadas x consVIP (piscadas x consumidores)
					 //Tratamento do defeito
					 double    nmanAuto;//#manobras automáticas  hkedp int->double
					 double    nmanMan; //#manobras manuais      hkedp int->double
					 int       indexResCarga;
					 };
//---------------------------------------------------------------------------
struct strResRede : TObject
					 {
					 VTRede     *rede;
					 //Variáveis de desligamentos por falha na rede de MT
					 double  tdeslN, ndeslN; //tempodeslig x consumidores (min x consum), deslig.consumdidores para desligamentos > 3min
					 double  end;            //end total MT+BT
					 double  tdeslENDN, ndeslENDN; //tempos.consum (min x consum) e deslig.consum (deslig x consum) totais sem limitantes
					 int     ncons;

					 //Variáveis de desligamentos por falha na rede de BT
					 double  tdeslBTN, ndeslBTN; //tempodeslig x consumidores (min x consum), deslig.consumdidores (deslig x consum) para desligamentos > 3min
					 double  endBT; //end devido a falhas BT
					 //Variáveis de piscadas
					 double  ndeslPisca;   //(piscadas x consum)
					 double  ndeslPiscaVIP;//(piscadas x consum VIP)
					 double  nconsVIP;    //Número de consumidores VIP

					 //Indicadores individuais
					 double  compensacao; //compensação regulatória por transgressao de indic indiv
					 double  ncargaVIP;   //número de consumidores A4 e redes BT com consumidores VIP
					 double  dicVIP;      //dic médio por rede BT com VIP
					 double  ficVIP;      //fic médio por rede BT com VIP

					 //indice para busca de strResBloco em lisResBloco hkrel
					 int     indexResBloco;
					 int     indexResCarga;
					 };
//---------------------------------------------------------------------------
class VTResCont : public TObject
	{
	public:  //property
		__property strResBloco* ResBloco[VTBloco *bloco][bool flagCria] = {read=PM_GetResBloco};
		__property strResCarga* ResCarga[VTCarga *carga][TList *lisRes] = {read=PM_GetResCarga};
		__property strResRede*  ResRede[VTRede *rede]   = {read=PM_GetResRede};
		__property strResRede*  ResGeral   = {read=PM_GetResGeral};
		__property TList*       LisResBloco              = {read=PM_GetLisResBloco};
		__property TList*       LisResRede               = {read=PM_GetLisResRede};
		__property TList*       LisResCarga              = {read=PM_GetLisResCarga};
		__property TList*       LisResBlocoRede[VTRede *rede]     = {read=PM_GetLisResBlocoRede};
		__property TList*       LisResCargaRede[VTRede *rede]     = {read=PM_GetLisResCargaRede};
		__property TList*       LisResCargaBloco[VTBloco *bloco]  = {read=PM_GetLisResCargaBloco};

	public:
								 __fastcall VTResCont(void) {};
		virtual            __fastcall ~VTResCont(void) {};
		virtual		  void __fastcall IniciaDicFicPu(bool flagZera = true) = 0;
		virtual		  void __fastcall IniciaListas(VTRede *rede = NULL) = 0;
		virtual		  void __fastcall IniciaResultados(VTRede *rede = NULL) = 0;
		virtual VTResCont* __fastcall ClonaResCont(void) = 0;
		virtual	   double __fastcall Indic(strResBloco *resBloco, int ind) = 0; //ind - 0:tdesl 1:ndesl 2:end 3:tcons 4:dcons
		virtual	    void  __fastcall ImprimeResBloco(AnsiString arq) = 0;
		virtual	    void  __fastcall ImprimeResCarga(AnsiString arq) = 0;
		virtual      void  __fastcall ImprimeResGeralRede(AnsiString arq) = 0;
		//virtual	   void   __fastcall LisResCargaRede(TList *lisExt, VTRede *rede)  0;

	protected:  //métodos acessados via property
		virtual strResBloco* __fastcall PM_GetResBloco(VTBloco *bloco, bool flagCria = false) = 0;
		virtual strResCarga* __fastcall PM_GetResCarga(VTCarga *carga, TList *lisRes) = 0;
		virtual strResRede*  __fastcall PM_GetResRede(VTRede *rede) = 0;
		virtual strResRede*  __fastcall PM_GetResGeral(void) = 0;
		virtual TList*       __fastcall PM_GetLisResBloco(void) = 0;
		virtual TList*       __fastcall PM_GetLisResRede(void) = 0;
		virtual TList*       __fastcall PM_GetLisResCarga(void) = 0;
		virtual TList*       __fastcall PM_GetLisResBlocoRede(VTRede *rede) = 0;
		virtual TList*       __fastcall PM_GetLisResCargaRede(VTRede *rede) = 0;
		virtual TList*       __fastcall PM_GetLisResCargaBloco(VTBloco *bloco) = 0;

	public: //dados
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedeViz
//---------------------------------------------------------------------------
VTResCont* __fastcall NewObjResCont(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
