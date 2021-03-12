//-----------------------------------------------------------------------------
#ifndef TIntegraEtdH
#define TIntegraEtdH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTIntegraEtd.h"

//---------------------------------------------------------------------------
class VTArea;
class VTBarra;
class VTCarga;
class VTEqpto;
class VTLigacao;
class VTRede;
class VTSubestacao;

//---------------------------------------------------------------------------
class TIntegraEtd : public VTIntegraEtd
   {
   public:
           __fastcall  TIntegraEtd(VTApl *apl);
		   __fastcall ~TIntegraEtd(void);
      bool __fastcall  InsereETDcompleta(void);
      bool __fastcall  InsereETDsimplificada(void);
      bool __fastcall  InsereSoSuprimentos(void);

   private: //métodos
      bool       __fastcall CalculaAreaEsq(TList *lisBAR, VTArea *area);
      bool       __fastcall CalculaAreaUtm(TList *lisBAR, VTArea *area);
      VTRede*    __fastcall CarregaRedeDaBase(AnsiString codigo);
	  void       __fastcall ClonaEqptos(TList *lisORIG, TList *lisCLONE);
	  bool       __fastcall Determina1Suprimento(VTSubestacao *subestacao, VTBarra *barra);
	  bool       __fastcall DeterminaBarraATMT(VTSubestacao *subestacao, VTBarra **barra_at, VTBarra **barra_mt);
      VTCarga*   __fastcall ExisteCarga(TList *lisCAR, AnsiString codigo);
      VTLigacao* __fastcall ExisteChave(TList *lisEXT);
      void       __fastcall InsereToDel(VTEqpto *eqpto);
      void       __fastcall InsereToDel(TList *lisEQP);
      bool       __fastcall RedefineBarraInicial(VTRede *rede, VTBarra *bar_ini);
      //void       __fastcall RedefineCoordenadas(VTRede *rede, VTBarra *bar_ref);
      void       __fastcall RedefineCoordenadasATMT(VTBarra *barra_at,VTBarra *barra_mt);
      void       __fastcall RedefineCoordenadasEsqETD();
      void       __fastcall RedefineCoordenadasUtmETD(VTRede *rede_etd, VTRede *rede_etd_ref);
      void       __fastcall RedefineCoordenadasUtmPRI_SEC(void);
      void       __fastcall RedefineDominioRede(VTRede *rede, VTRede *rede_ref);
	  void       __fastcall RedefineEqptoRedeStatusNovo(VTRede *rede);
	  bool       __fastcall RedefineTrafos(VTSubestacao *subestacao, VTBarra* barra_at, VTBarra* barra_mt);
      void       __fastcall RedefineTrafoZat(VTRede *rede);
      AnsiString __fastcall StringTrim(AnsiString txt);

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList     *lisBAR;
      TList     *lisBAR_INI;
      TList     *lisCAR;
      TList     *lisETD;
      TList     *lisLIG;
      TList     *lisPRI;
      TList     *lisSEC;
      TList     *lisEQP;
	  TList     *lisDEL;
	  TList     *lisALT;
   };

#endif
//---------------------------------------------------------------------------
//eof
