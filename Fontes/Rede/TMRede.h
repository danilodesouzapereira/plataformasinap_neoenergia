//---------------------------------------------------------------------------
#ifndef TMRedeH
#define TMRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMRede.h"

//---------------------------------------------------------------------------
class TMRede : public VTMRede
   {
   public:
                  __fastcall  TMRede(void);
                  __fastcall ~TMRede(void);
      VTMRede*    __fastcall  Clone(void);
      void        __fastcall  CopiaAtributosDe(VTEqpto &ref);
      bool        __fastcall  AreaEsquematico(int &x1, int &y1, int &x2, int &y2);
      bool        __fastcall  AreaUtm_cm(int &x1, int &y1, int &x2, int &y2);
      bool        __fastcall  ExisteCorte(VTCorte *corte);
      VTCorte*    __fastcall  ExisteCorte(VTMRede *mrede);
      bool        __fastcall  ExisteRede(VTRede *rede);
      VTReducao*  __fastcall  ExisteReducao(VTCorte *corte);
      VTReducao*  __fastcall  ExisteReducaoMRedeNaoCarregada(void);
      void        __fastcall  IncorporaMRede(TList *lisMR);
      void        __fastcall  IncorporaMRede(VTMRede *mrede);
      void        __fastcall  InsereCorte(VTCorte *corte);
      void        __fastcall  InsereRede(VTRede *rede);
      void        __fastcall  InsereRede(TList *lisEQP);
      void        __fastcall  InsereReducao(VTReducao *reducao, VTCorte *corte);
      void        __fastcall  LisBarra(TList *lisEQP, bool barra_isolada=true);
      TList*      __fastcall  LisCorte(void);
      void        __fastcall  LisCorte(TList *lisEQP);
      void        __fastcall  LisEqbar(TList *lisEQP, int tipo, bool barra_isolada=true);
      void        __fastcall  LisEqpto(TList *lisEQP, int tipo, bool barra_isolada=true);
      void        __fastcall  LisLigacao(TList *lisEQP, int tipo=-1);
      void        __fastcall  LisMRedeVizinha(TList *lisMRV);
      void        __fastcall  LisMRedeVizinha(VTCorte *corte, TList *lisMRV);
      void        __fastcall  LisMRedeVizinhaCarregada(VTCorte *corte, TList *lisMRV);
      void        __fastcall  LisMutua(TList *lisEQP);
      TList*      __fastcall  LisRede(void);
      void        __fastcall  LisRede(TList *lisEQP);
      TList*      __fastcall  LisReducao(void);
      void        __fastcall  LisReducao(TList *lisEQP);
      void        __fastcall  LisReducaoVizinha(TList *lisEQP);
      int         __fastcall  NumeroDeCortes(void);
      int         __fastcall  NumeroDeGerSup(void);
      int         __fastcall  NumeroDeRedes(void);
      void        __fastcall  ReiniciaReducao(void);
      void        __fastcall  RemoveCorte(VTCorte *corte);
      int         __fastcall  Tipo(void);
      AnsiString  __fastcall  TipoAsString(void);

   protected:  //métodos acessados via property
      bool       __fastcall PM_GetCarregada(void);
      TColor     __fastcall PM_GetColor(void);
      AnsiString __fastcall PM_GetCodigo(void);
      bool       __fastcall PM_GetGeracao(void);
      int        __fastcall PM_GetId(void);
      bool       __fastcall PM_GetMalha(void);
      VTMNet*    __fastcall PM_GetMNet(void);
      VTMRede*   __fastcall PM_GetPai(void);
      bool       __fastcall PM_GetRedutivel(void);
      TDate      __fastcall PM_GetRevisao(void);
      bool       __fastcall PM_GetValida(void);
      void       __fastcall PM_SetCarregada(bool carregada);
      void       __fastcall PM_SetMalha(bool malha);
      void       __fastcall PM_SetPai(VTMRede *pai);
      void       __fastcall PM_SetRedutivel(bool redutivel);
      void       __fastcall PM_SetRevisao(TDate revisao);
      void       __fastcall PM_SetMNet(VTMNet *mnet);

   private:  //métodos
      void     __fastcall IniciaLisCorte(void);

   private:  //dados externos
      //VTBarra *pbar_ini;   //Barra inicial (válido apenas p/ MRede simples)

   private:  //dados locais
      struct   {
               bool     carregada;
               bool     malha;
               TColor   color;
               bool     redutivel;
               TDate    revisao;
               VTMNet   *mnet;
               VTMRede  *pai;
               }PD;
      TList *lisREDE;   //lista de objetos VTRede
      TList *lisREDU;   //lista de objetos VTReducao
      TList *lisCORTE;  //lista de objetos VTCorte
      TList *lisTMP;    //lista p/ uso temporário
   };

//---------------------------------------------------------------------------
#endif
//eof
