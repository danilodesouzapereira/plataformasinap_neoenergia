//---------------------------------------------------------------------------
#ifndef TAgregadaH
#define TAgregadaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAgregada.h"

//---------------------------------------------------------------------------
class TSegmento;
class VTNode;

//---------------------------------------------------------------------------
class TAgregada : public VTAgregada
   {
   public:  //métodos
                __fastcall  TAgregada(VTApl *apl_owner);
                __fastcall ~TAgregada(void);
      void      __fastcall  Inicia(void);
      TList*    __fastcall  LisSegmentoRaiz(void);
      VTResumo* __fastcall  ResumoTotal(void);

   protected:
      bool __fastcall  PM_GetValida(void);

   private:  //métodos
      void       __fastcall  AgregaResumoTotal(void);
      void       __fastcall  CalculaTotalEnergia(void);
      //TSegmento* __fastcall  ExisteSegmento(int tiporede_id);
      //TSegmento* __fastcall  ExisteSegmento(TList *lisSEG, int tiporede_id);
      TSegmento* __fastcall  ExisteSegmentoRaiz(int tiporede_id);
      void       __fastcall  InsereSegmentoRaiz(VTNode *node_pai);
      void       __fastcall  InsereSegmentoFilho(VTNode *node_pai, TSegmento *segmento_pai);
      int        __fastcall  LisAllSegmento(TList *lisEXT);
      void       __fastcall  Reinicia(void);

   private:  //objetos externos
      VTApl *apl;

   private:  //dados locais
      VTResumo *resumo_total;
      TList    *lisSEGMENTO;     //lista de Segmentos
      TList    *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof
