//---------------------------------------------------------------------------
#ifndef TRadialH
#define TRadialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRadial.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTBarra;
class VTCorte;
class VTTipoRede;
class VTTipos;

//---------------------------------------------------------------------------
class TRadial : public VTRadial
   {
   private: //objetos externos
      VTApl   *apl;
      VTRedes *redes;
      VTTipos *tipos;

   private: //dados locais
      TList  *lisEQP;
      TList  *lisBAR;
      TList  *lisVIZ;
      TList  *lisSUBESTACAO;
      TList  *lisPRIMARIO;
      TList  *lisRETICULADO;
	  TList  *lisSECUNDARIO;

   public:  //métodos
                    __fastcall  TRadial(VTApl *apl);
                    __fastcall ~TRadial(void);
      VTPrimario*   __fastcall  ExistePrimario(VTRede *rede);
      VTReticulado* __fastcall  ExisteReticulado(VTRede *rede);
      VTSecundario* __fastcall  ExisteSecundario(VTRede *rede);
      VTSubestacao* __fastcall  ExisteSubestacao(VTRede *rede);
	  bool          __fastcall  Inicia(VTRedes *redes);
      TList*        __fastcall  LisPrimario(void);
      void          __fastcall  LisRede(TList *lisEXT, int segmento);
      TList*        __fastcall  LisSecundario(void);
      TList*        __fastcall  LisSubestacao(void);

   private: //métodos
	  void          __fastcall AssociaPrimario(void);
	  void          __fastcall AssociaReticulado(void);
	  void          __fastcall AssociaSecundario(void);
	  void          __fastcall AssociaSubestacaoPrimarioSecundario(VTArvore *arvore);
	  VTCorte*      __fastcall ExisteCorte(VTRede *rede, VTRede *rede_viz);
	  //VTRede*       __fastcall ExisteRedeVizinha(VTRede *rede, VTTipoRede *tiporede);
	  VTRede*       __fastcall ExisteRedeVizinha(VTRede *rede, VTTipoRede *tiporede);
	  VTSecundario* __fastcall ExisteSecundario(VTRede *rede, TList *lisSEC);
	  void          __fastcall MontaPrimario(void);
	  void          __fastcall MontaReticulado(void);
	  void          __fastcall MontaSecundario(void);
	  void          __fastcall MontaSubestacao(void);
	  void          __fastcall Reinicia(void);
   };

//---------------------------------------------------------------------------
#endif
//eof
