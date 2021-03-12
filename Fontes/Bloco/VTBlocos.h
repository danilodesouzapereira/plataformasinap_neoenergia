//---------------------------------------------------------------------------
#ifndef VTBlocosH
#define VTBlocosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTChave;
class VTEqpto;
class VTLigacao;
class VTRede;
class VTRedes;
class VTElo;
class VTBloco;

//---------------------------------------------------------------------------
class VTBlocos : public TObject
	{
   public:
							  __fastcall  VTBlocos(void) {};
      virtual          __fastcall ~VTBlocos(void) {};
		virtual bool     __fastcall  EliminaBloco(VTBloco *bloco) = 0;
		virtual bool     __fastcall  EliminaBlocos(TList *lisEXT) = 0;
		virtual bool     __fastcall  EliminaBlocosNaoEnergizados(void) = 0;
		virtual bool     __fastcall  EliminaElosComBlocosIguais(void) = 0;
		virtual bool     __fastcall  EliminaElosComChaveAberta(void) = 0;
		virtual bool     __fastcall  Executa(VTEqpto *eqpto) = 0;
		virtual bool     __fastcall  Executa(VTRede *rede) = 0;
		virtual bool     __fastcall  Executa(VTRedes *redes) = 0;
		virtual VTBloco* __fastcall  ExisteBloco(VTBarra *barra) = 0;
		virtual VTBloco* __fastcall  ExisteBloco(VTLigacao *ligacao) = 0;
		virtual VTElo*   __fastcall  ExisteElo(VTBloco* bloco1, VTBloco* bloco2) = 0;
		virtual VTElo*   __fastcall  ExisteElo(VTChave *chave) = 0;
		virtual bool     __fastcall  FundeBlocos(VTBloco* bloco, TList* lisEXT) = 0;//hk
		virtual bool     __fastcall  FundeBlocos(VTElo *elo) = 0;
		virtual TList*   __fastcall  LisBloco(void) = 0;
		virtual void     __fastcall  LisBlocoEnergizado(TList *lisEXT) = 0;
		virtual void     __fastcall  LisBlocoGeracao(TList *lisEXT) = 0;
		virtual void     __fastcall  LisBlocoGeracao(VTRede*rede, TList *lisEXT) = 0;
		virtual void     __fastcall  LisBlocoNaoEnergizado(TList *lisEXT) = 0;
		virtual void     __fastcall  LisBlocoRede(VTRede *rede, TList *lisEXT) = 0;
		virtual void     __fastcall  LisBlocoVizinho(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisBlocoVizinho1(VTBloco *bloco, TList *lisEXT, TList* lisCHV) = 0;//hk
		virtual void     __fastcall  LisBlocoVizinhoConectado(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveBloco(VTBarra *barra, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveBloco(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveBloco(VTLigacao *ligacao, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveFronteira(VTBarra *barra, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveFronteira(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveFronteira(VTLigacao *ligacao, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveFronteira(VTRede *rede, TList *lisEXT) = 0;
		virtual void     __fastcall  LisChaveInterna(VTRede *rede, TList *lisEXT) = 0;
		virtual TList*   __fastcall  LisElo(void) = 0;
		virtual void     __fastcall  LisElo(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisEloFronteira(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisEloFronteira(VTRede *rede, TList *lisEXT) = 0;
		virtual void     __fastcall  LisEloFronteiraSuper(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisEloInterno(VTBloco *bloco, TList *lisEXT) = 0;
		virtual void     __fastcall  LisEloInterno(VTRede *rede, TList *lisEXT) = 0;
		virtual void     __fastcall  LisRedeVizinha(VTBloco *bloco, TList *lisEXT) = 0;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBlocos* __fastcall NewObjBlocos(void);

#endif
//---------------------------------------------------------------------------
//eof
