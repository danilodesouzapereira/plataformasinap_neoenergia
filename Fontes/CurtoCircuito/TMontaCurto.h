//---------------------------------------------------------------------------
#ifndef TMontaCurtoH
#define TMontaCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaCurto.h"

//---------------------------------------------------------------------------
class VTNet;
class VTTree;

//---------------------------------------------------------------------------
class TMontaCurto : public VTMontaCurto
   {
   public:
             __fastcall TMontaCurto(VTApl *apl);
             __fastcall ~TMontaCurto(void);
      VTNet* __fastcall Executa(VTRedes *redes, VTBarra *barra);

   private: //métodos
      void    __fastcall IniciaLisBarraGeracao(VTRedes *redes, TList *lisBAR);
      void    __fastcall IniciaLisLigacao(VTRedes *redes, TList *lisLIG);
      VTTree* __fastcall IniciaTree(VTRedes *redes);
      VTNet*  __fastcall MontaNet(TList *lisBAR, TList *lisLIG);

   private: //dados externos
      VTApl   *apl;
      VTBarra *barra;
      VTRedes *redes;

   private: //dados locais
      TList  *lisEQP;
      TList  *lisBAR;
      TList  *lisLIG;
   };

//---------------------------------------------------------------------------
#endif
//eof

