//---------------------------------------------------------------------------
#ifndef TResumosH
#define TResumosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResumos.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTBalancos;
class VTPerdas;
class VTRede;
class VTEnergiaPassantes;

//---------------------------------------------------------------------------
class TResumos : public VTResumos
   {
   public:  //métodos
                 __fastcall  TResumos(VTApl *apl);
                 __fastcall ~TResumos(void);
      void       __fastcall  AtualizaTodosResumos(void);
      void       __fastcall  Clear(void);
      VTResumo*  __fastcall  ExisteResumo(VTEqpto *eqpto);
      bool       __fastcall  IniciaArvoreComResumo(void);
      bool       __fastcall  InsereResumo(int nodepai_id, int node_id, VTResumo *resumo);
      TList*     __fastcall  LisResumo(void);
      void       __fastcall  LisResumo(TList *lisEXT);
      void       __fastcall  LisResumo(int tiporede_id, TList *lisEXT);
      void       __fastcall  LisResumoJusante(VTNode *node, TList *lisEXT);
      void       __fastcall  LisResumoJusante(VTNode *node, int tiporede_id, TList *lisEXT);
      void       __fastcall  LisResumoMontante(VTNode *node, TList *lisEXT);
      void       __fastcall  LisResumoNode(VTNode *node, TList *lisEXT);
      void       __fastcall  LisResumoNode(VTNode *node, int tiporede_id, TList *lisEXT);
      int        __fastcall  SelecionaResumo(TList *lisORIG, TList *lisDEST, int tiporede_id);
	  void       __fastcall  TrataResumoPaiFilho(VTResumo *resumo, TList *lisRESUMO);

   private:  //métodos
      bool   __fastcall IniciaNodeComResumo(VTNode *node);
      bool   __fastcall IniciaResumoRede(VTResumo *resumo, VTRede *rede);
      double __fastcall SomatoriaEnergiaCargas(VTRede *rede);

   private:  //objetos externos
      VTApl      *apl;
      VTArvore   *arvore;
	  VTBalancos *balancos;
	  VTEnergiaPassantes *energiaPassantes;
      VTPerdas   *perdas;

   private:  //dados locais
      TList    *lisRESUMO;
      TList    *lisTMP;
      VTResumo *resumo_tmp;
   };

//---------------------------------------------------------------------------
#endif
//eof
