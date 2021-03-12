#ifndef TRedeFHH
#define TRedeFHH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTRedeFH.h"

//---------------------------------------------------------------------------
class VTLigacao;

//---------------------------------------------------------------------------
class TRedeFH : public VTRedeFH
   {
   public:
                __fastcall  TRedeFH(VTApl *apl);
                __fastcall ~TRedeFH(void);
      VTBarFH*  __fastcall  ExisteBarFH(VTEqpto *eqpto);
      VTLigFH*  __fastcall  ExisteLigFH(VTEqpto *eqpto);
      bool      __fastcall  Inicia(VTRedes *redes,
                                   VTNet   *net,
                                   int     num_pat,
                                   int     num_harmonicas,
                                   int     *vet_ordem_harmonicas);
      TList*    __fastcall  LisBarFH(void);
      TList*    __fastcall  LisBarFH(VTNo *no);
      TList*    __fastcall  LisLigFH(void);
      TList*    __fastcall  LisLigFH_Ficticia(void);

   private: //métodos
      VTLigFH* __fastcall ExisteLigFH_Ficticia(VTLigacao *ligacao);
      void     __fastcall IniciaLisBarFH(TList *lisBAR);
      void     __fastcall IniciaLisLigFH(TList *lisLIG);
      void     __fastcall IniciaLisLigFH_Ficticia(VTRedes *redes);
      void     __fastcall ReiniciaResultados(void);

   private: //métodos acessados via property
      int      __fastcall  PM_GetOrdemHarmonica(int ind_freq);
      int      __fastcall  PM_GetNumHarmonicas(void);
      //
      void     __fastcall  PM_SetOrdemHarmonica(int ind_freq, int ordem);
      void     __fastcall  PM_SetNumHarmonicas(int num_harmonicas);

   private: //objetos externos
      VTApl *apl;
      VTNet *net;

   private: //dados acessados via property
      struct
         {
         int num_pat;                // Número de patamares armazenados
         int num_harmonicas;         // Número de ordens harmônicas
         int *vet_ordem_harmonicas;  // Vetor de ordens harmônicas
         }PD;

   private: //dados locais
      TList *lisBAR_FH_NO;
      TList *lisBAR_FH;  //lista de BarFH   da RedeFH
      TList *lisLIG_FH;  //lista de LigFH   da RedeFH
      struct   {
               TList *lisLIG_FH; //lista de LigFH das Ligacoes fictícas das Reducoes
               }ficticia;
   };

#endif
//-----------------------------------------------------------------------------
// eof
