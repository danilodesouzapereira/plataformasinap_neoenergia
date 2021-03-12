// ---------------------------------------------------------------------------
#ifndef TBuscaProtH
#define TBuscaProtH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBuscaProt.h"
// ---------------------------------------------------------------------------
class VTLisBlocoChave;
class VTTipoChave;
class VTCarga;//hk201902
// ---------------------------------------------------------------------------
class TBuscaProt : public VTBuscaProt
{
public:
	__fastcall TBuscaProt(VTApl *apl, bool flagCalc);
	__fastcall ~TBuscaProt(void);
	void __fastcall BuscaBlocoAgreg(VTBloco *bloco, TList *lisEXT);
	bool __fastcall ChaveReligadora(VTChave *pchv);
	VTBloco* __fastcall IdentificaBloco(VTEqpto *eqpto);
	VTVisRede* __fastcall IdentificaVisRede(VTChave *pchv, TList *lisBlocosDesl = NULL);//hk201805: rede pai-filha: não transferencia para blocos desligados
	bool __fastcall CalculaCargasBlocoRad(int iPat);
	double __fastcall CargaLisBlocosMan(VTDefeito *pdef, int iPat);
	double __fastcall FolgaIA(VTChave *pchvVis, int indPat);
	double __fastcall FolgaIA(VTVisRede *visRede, int indPat);
	VTBlocoRad* __fastcall IdentificaBlocoRad(VTBloco *bloco);
	VTBlocoRad* __fastcall IdentificaBlocoRad(VTEqpto *eqpto);
	void __fastcall IdentificaChvMont(VTEqpto *eqpto, TList *lisExt, bool flagProt);
	// Identifica chaves a montante do defeito
	void __fastcall IdentificaChvMont(VTBloco *bloco, TList *lisExt, bool flagProt);
	// Identifica chaves a montante do defeito
	int __fastcall IndiceChave(VTChave *pchv);  //hk201808: reformula: sai do publico
	bool __fastcall Inicia(bool flagExeBlocos = true);
	void __fastcall ListaBlocoRadTotal(TList *lisEXT);
	double __fastcall IA_Crit(VTChave *pchvVis, int indPat);
	double __fastcall IA_Crit(VTVisRede *visRede, int indPat);
	VTEqpto* __fastcall Liga_Crit(VTChave *pchvVis, int indPat);
	// hkv201609: Restr.cap.Chaves: Inclusão de.. : Liga_Crit(VTChave *pchvVis, int indPat)
	bool __fastcall LisBlManLis(VTDefeito *pdef);
	bool __fastcall LisBlJus(VTChave *pchv, TList *lisEXTBloco);
	TList* __fastcall LisBlJusChave(VTChave *pchv); // Retorna blocos a jusante da chave
	VTBarraRad* __fastcall LisBarProt(VTBloco *bloco);
	bool __fastcall LisBarChave(VTChave *pchave, TList *lisBarra);
	bool __fastcall LisBlProt(VTChave *pchv, TList *lisEXTBloco);
	bool __fastcall LisBlVisMan(VTBloco *blocoDef, TList *lisEXTBloco);
	void __fastcall LisBlRadTerm(VTBloco *bloco, TList *lisEXTBlocoRad);
	void __fastcall LisChaveOrd(TList *lisExt);
	bool __fastcall LisChvBloco(VTBloco *bloco, TList *lisExt);
	void __fastcall LisBlocosMed(VTChave *pchave, TList *lisExt, TList *lisExtFixo = NULL);
	// Retorna blocos a jusante e a chave de medicao seguinte
	bool __fastcall LisLigaVis(VTChave *chv, VTBarra *pbarra, TList *lisExt, TList *lisBlocosExt);
	bool __fastcall LisLigaVis(VTBarra *pbarra1, TList *lisExt);
	bool __fastcall LisLigaBloco(VTChave *chv1, VTChave *chv2, TList *lisExt);
	void __fastcall ListaOrdenadaBlocos(TList *lisBlocoExt, VTRede *rede);
	void __fastcall ListaLigaVisTotal(TList *lisChvVis, TList *lisLigaVis, TList *lisLigaVisDef);
	void __fastcall LisChvJus(VTBloco *bloco, VTChave *chvMont, TList *lisChvJus);

	bool __fastcall LisBarChaveZonaProt(VTChave *pchave, TList *lisBarra);

	void __fastcall LisBlocosVis(VTRede *rede, TList *lisBlocoExt);
	void __fastcall LisChavesVis(VTRede *rede, TList *lisChaveExt);
	// Retorna lista de chaves NA --> suprimento
	void __fastcall LisChavesProt(VTRede *rede, TList *lisChaveExt);
	// Retorna chaves dos equipamentos de protecao ate o suprimento
	bool __fastcall ListaZonaProt(VTChave *pchv, TList *lisBlocoExt);
   bool __fastcall ListaZonaProtJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt);//, TList *lisBarrasExt);
   bool __fastcall ListaBlocoRadZonaProtJus(VTChave *pchv, TList *lisBlocoRadExt);//hk201802a
   bool __fastcall ListaZonaProtIFJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt); //hk201710IF
   bool __fastcall ListaZonaReligaJus(VTChave *pchv, TList *lisBlocoExt, TList *lisChavesExt);//, TList *lisBarrasExt);
	TList* __fastcall ListaZonaReliga(VTChave *pchv);
	bool __fastcall PreparaDefeito(VTDefeito *pdef); // hkedp, TList *lisChavesAtivas);
	void __fastcall LisLigaJusChave(VTChave *chave, TList *lisLigaJus);
	bool __fastcall ChavesSerieMax(VTRede *rede, TList *lisTipoChaves, int ncLim,
		AnsiString &chaveSerie);
	bool __fastcall LisChavesDmax(double dMax, TList *lisChavesDmax);
	void __fastcall ReiniciaZonaReliga(void);
   void __fastcall PreparaIndicacaoFalta(void);   //hk201808: reformulação: sai para buscaProt

	// bool __fastcall LisBlMan(VTBloco *blocoDef, TList *lisEXTBloco, TList *lisEXTBlocoVis, TList *lisEXTChv);
	// bool __fastcall LisBlManDefeito(VTDefeito *pdef);
private: // métodos
   VTBarra* __fastcall BuscaBarraChave(VTChave *chave, VTBloco *bloco);//hk201802a: referencia do paramBloco por chave e barra
	VTChave* __fastcall BuscaChaveMon(VTBloco *bloco);
	bool __fastcall ChaveAutomatica(VTChave *pchv);
	bool __fastcall ChaveProtecao(VTChave *pchv);
	VTBloco* __fastcall IdentificaBlocoEqNaoChave(VTEqpto *eqpto);
	void __fastcall IdentificaChvMontante(TObject *pobj, TList *lisExt, bool flagProt);
	void __fastcall IdentificaLisBlocosRadJusante(TList *lisBlocoDef, TList *lisEXT);
	VTLisBlocoChave* __fastcall IdentificaBlocoChave(VTChave *pchv);
	VTEqpto* __fastcall Liga_Crit(VTVisRede *visRede, int indPat); // 201609: Restr.cap.Chaves
	bool __fastcall LisBarChave(VTBlocoRad *blocoRad, TList *lisBarra);
	VTBarraRad* __fastcall LisBarraBloco(VTBloco *bloco, VTBarra *pbarra);
	bool __fastcall LisBarraRadChave(VTBlocoRad *blocoRad, TList *lisBarraRad);
	bool __fastcall LisBlJus(VTBlocoRad *blocoRad, TList *lisEXTBloco, bool flagProt = false);
	bool __fastcall LisBlJus(VTBloco *bloco, TList *lisEXTBloco);
	void __fastcall LisBlocoChave(VTBloco *bloco, TList *lisExt);
	bool __fastcall LisLigaVis(VTVisRede *visRede);
   bool __fastcall ListaZonaProtJus(VTBlocoRad  *blocoRad, TList *lisBlocoExt, TList *lisChavesExt, bool flagIF, bool flagBloco = true);//hk201802a
   bool __fastcall ListaZonaReligaJus(VTBlocoRad  *blocoRad, TList *lisBlocoExt, TList *lisChavesExt = NULL);//, TList *lisBarrasExt = NULL);
	bool __fastcall OrdenaChaves(bool flagExeBlocos = true);
	bool __fastcall MontaLisBlocos(VTLisBlocoChave *plisBlocoChave); // , bool flagVis =false);
	// bool __fastcall MontaLisBlocos(VTBloco *bloco);//, bool flagVis =false);
	// bool __fastcall MontaLisBlocos(VTChave *chave);//, bool flagVis =false);
	void __fastcall MontaLisBlocosJus(VTBloco *blocoAux);
	void __fastcall MontaLisBlocosJusVis(VTBloco *blocoAux);
	void __fastcall LisLigaDMax(VTBarra *pbarra, int tipoLigacao, TList *lisLiga, double distMax);
	void __fastcall OrdenaLisBlocosGeracao(TList *lisBlocoGeracao);
 double __fastcall ComprimentoBloco(VTBloco *bloco);
    int __fastcall NCons(VTCarga *carga);//hk201902


protected: // métodos acessados via property
	double __fastcall PM_GetCargaBlocoJus(VTChave* chave, int np);
	bool __fastcall PM_GetFlagInicia(void) {return (flagInicia);};
	VTBlocos* __fastcall PM_GetBlocos(void) {return (blocos);};
	TList* __fastcall PM_GetLisBlocoRad(void)	{return (lisBlocoRad);};
	TList* __fastcall PM_GetLisChavesVis(void) {return (lisChavesVis);};
	TList* __fastcall PM_GetLisVisRede(void) {return (lisVisRede);};
	TList* __fastcall PM_GetLisTipoEqProt(void) {return (lisTipoEqProt);};
	TList* __fastcall PM_GetLisChavesMed(void) {return (lisChavesMed);};
	TList* __fastcall PM_GetLisDJPri(void)	{return (lisDjPri);};
	TList* __fastcall PM_GetLisChvAuto(void) {return (lisChvAuto);};
	TList* __fastcall PM_GetLisChvAtivas(void) {return (lisChavesAtivas);};
	int __fastcall PM_GetIProfReliga(void)	{return (iProfReliga);};
	void __fastcall PM_SetIProfReliga(int iProfReliga);
   TList* __fastcall PM_GetLisEQIF(void) {return(lisEQIF);}; //hk201710IF
   TList* __fastcall PM_GetLisChavesIF(void) {return(lisChavesIF);}; //hk201710IF
   int    __fastcall PM_GetIProfDescoord(void) {return(iProfDescoord);};  //hk201711: taxa de descoordenação de proteção
   void   __fastcall PM_SetIProfDescoord(int iProfDescoord) {this->iProfDescoord = iProfDescoord;};              //hk201711: taxa de descoordenação de proteção
	// void   __fastcall PM_SetLisChvAtivas(TList *lisChavesAtivas) {this->lisChavesAtivas = lisChavesAtivas;};
	// TList* __fastcall PM_GetLisBlocoJus(void) {return(lisBlocoJus);};
	// TList* __fastcall PM_GetLisBlocoMon(void) {return(lisBlocoMon);};

private: // objetos externos

	VTApl *apl;

	// dados locais
	TList *lisBlocoChv; // lista de chaves ordenadas montante->jusante
	bool flagInicia; // flag se foi inicializado o objeto
	VTBarraRad *barraRad;
	VTBlocos *blocos;

	TList *lisBlocoRad; // lista de blocosRad de suprimento -> gera os outros blocos

	TList *lisVisRede; // lista de objetos TVisRede com informaçoes de chaves VIS
	TList *lisChavesVis; // lista de objetos VTChave  com as chaves VIS de lisVisRede
	TList *lisChavesMed; // Lista de chaves com medicao
	TList *lisDjPri; // Lista de disjuntores início de alimentadores primários

	TList *lisChvAuto; // Lista de chaves automáticas
	TList *lisChavesAtivas;
	// Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
	TList *lisTipoEqProt; // Lista de tipos de equipamento de protecao

      TList *lisEQIF;//lista de objetos strIF, para cálculo de ganhos em duração de localização:hk201710IF
      TList *lisChavesIF;//lista de chaves com IF instalado:hk201710IF

	bool flagCalc; // se sim for criado por TCalcIndCont, que é dono do VTBlocos
	VTTipoChave *tipoReligador;
	VTTipoChave *tipoDisjuntor;

	int iProfReliga;
	// sensibilidade de religamento: =0, somente o bloco imediatamente a jusante: =1, até o próximo religador

        int   iProfDescoord; //hk201711: taxa de descoordenação de proteção
public:
};
// ---------------------------------------------------------------------------
#endif
// eof
