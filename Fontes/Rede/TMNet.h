//---------------------------------------------------------------------------
#ifndef TMNetH
#define TMNetH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMNet.h"

//---------------------------------------------------------------------------
class TMNet : public VTMNet
   {
   public:
				  __fastcall TMNet(void);
				  __fastcall ~TMNet(void);
	  bool       __fastcall ExisteCorte(VTCorte *corte);
	  VTCorte*   __fastcall ExisteCorte(VTMNet *mnet);
	  bool       __fastcall ExisteMRede(VTMRede *mrede);
	  VTMRede*   __fastcall ExisteMRede(VTRede *rede);
	  VTMRede*   __fastcall ExisteMRedeCarregada(void);
	  VTReducao* __fastcall ExisteReducao(VTCorte *corte);
	  void       __fastcall IncorporaMNet(TList *lisEXT);
	  void       __fastcall IncorporaMNet(VTMNet *mnet);
	  void       __fastcall InsereMRede(VTMRede *mrede);
	  void       __fastcall InsereMRede(TList *lisEXT);
	  void       __fastcall InsereReducao(VTReducao *reducao, VTCorte *corte);
	  void       __fastcall LisBarra(TList *lisEXT, bool barra_isolada=true);
	  TList*     __fastcall LisCorte(void);
	  void       __fastcall LisCorte(TList *lisEXT);
	  void       __fastcall LisEqbar(TList *lisEXT, int tipo=-1, bool barra_isolada=true);
	  void       __fastcall LisEqpto(TList *lisEXT, int tipo=-1, bool barra_isolada=true);
	  TList*     __fastcall LisGerador(void);
	  void       __fastcall LisLigacao(TList *lisEXT, int tipo=-1);
	  void       __fastcall LisMNetVizinha(TList *lisEXT);
	  void       __fastcall LisMNetVizinha(VTCorte *corte, TList *lisEXT);
	  TList*     __fastcall LisMRede(void);
	  void       __fastcall LisMRede(VTCorte *corte, TList *lisEXT);
	  void       __fastcall LisMRedeCarregada(VTCorte *corte, TList *lisEXT);
	  void       __fastcall LisMutua(TList *lisEXT);
	  void       __fastcall LisRede(TList *lisEXT);
	  TList*     __fastcall LisReducao(void);
	  void       __fastcall LisReducaoVizinha(TList *lisEXT);
	  void       __fastcall LisReducaoVizinha(VTCorte *corte, TList *lisEXT);
	  TList*     __fastcall LisSuprimento(void);
	  void       __fastcall OrdenaMRedes(VTCorte *corte);

   protected:  //métodos acessados via property
	  AnsiString __fastcall PM_GetCodigo(void);
	  bool       __fastcall PM_GetEnabled(void);
	  bool       __fastcall PM_GetGeracao(void);
	  VTNet*     __fastcall PM_GetNet(void);
	  int        __fastcall PM_GetNumeroGerSup(void);
	  int        __fastcall PM_GetNumeroMRede(void);
	  TObject*   __fastcall PM_GetObj(void);
	  VTMNet*    __fastcall PM_GetPai(void);
	  //
	  void       __fastcall PM_SetEnabled(bool enabled);
	  void       __fastcall PM_SetNet(VTNet* net);
	  void       __fastcall PM_SetObj(TObject* obj);
	  void       __fastcall PM_SetPai(VTMNet *pai);

   private:  //métodos
	  void     __fastcall IniciaLisCorte(void);
	  void     __fastcall InsereCorte(VTCorte *corte);

   private:  //dados locais
	  TList *lisMR;     //lista de objetos VTMRede
	  TList *lisREDU;   //lista de objetos VTReducao
	  TList *lisCORTE;  //lista de objetos VTCorte
	  TList *lisTMP;    //lista p/ uso temporário
	  TList *lisGER;
	  TList *lisSUP;

   private:  //dados dados acessados via propery
      struct   {
               bool    enabled;
               VTMNet  *pai;
               VTNet   *net;
               TObject *obj;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
