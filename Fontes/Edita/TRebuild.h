//---------------------------------------------------------------------------
#ifndef TRebuildH
#define TRebuildH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCluster;
class VTLigacao;
class VTMutua;
class VTRede;

//---------------------------------------------------------------------------
class TRebuild : public TObject
   {
   public:
                  __fastcall  TRebuild(VTApl *apl);
                  __fastcall ~TRebuild(void);
	  void        __fastcall  IniciaListas(TList *lisEXT_REDE);
	  void        __fastcall  InsereBarra(VTBarra *barra);
      void        __fastcall  InsereCluster(VTCluster *cluster);
      void        __fastcall  InsereLigacao(VTLigacao *ligacao);
      void        __fastcall  InsereMutua(VTMutua *mutua);
      void        __fastcall  InsereRede(VTRede *rede);
      TList*      __fastcall  LisBarra(void);
      TList*      __fastcall  LisCluster(void);
      TList*      __fastcall  LisLigacao(void);
      TList*      __fastcall  LisMutua(void);
      TList*      __fastcall  LisRede(void);
      void        __fastcall  RemoveBarra(VTBarra *barra);
      void        __fastcall  RemoveCluster(VTCluster *cluster);
      void        __fastcall  RemoveLigacao(VTLigacao *ligacao);
      void        __fastcall  RemoveMutua(VTMutua *mutua);
      void        __fastcall  RemoveRede(VTRede *rede);

   private:  //objetos externos  métodos

      
   private:  //objetos externos
      VTApl *apl;

   private:  //dados locais
      TList   *lisBARRA;
      TList   *lisLIGACAO;
      TList   *lisMUTUA;
      TList   *lisREDE;
      TList   *lisCLUSTER;
   };

//---------------------------------------------------------------------------
#endif
//eof
