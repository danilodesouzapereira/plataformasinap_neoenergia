//---------------------------------------------------------------------------
#ifndef TSincronizaH
#define TSincronizaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSincroniza.h"

//---------------------------------------------------------------------------
class VTLock;
class VTLocks;

//---------------------------------------------------------------------------
class TSincroniza : public VTSincroniza
   {
   public:
             __fastcall TSincroniza(VTApl *apl);
             __fastcall ~TSincroniza(void);
      TList* __fastcall Abre(VTChave *chave);
      void   __fastcall Disable(void);
      TList* __fastcall Enable(void);
      TList* __fastcall Fecha(VTChave *chave);
      TList* __fastcall Manobra(VTChave *chave);
      TList* __fastcall Manobra(TList *lisEXT);
      
   private: // métodos
      bool    __fastcall AlteraChave(VTChave *chave, unsigned novo_estado);
      VTLock* __fastcall ExisteLockComChaveMaster(VTChave *chave, int estado);
      void    __fastcall RestauraChavesAlteradas(void);

   private: //objeto externos
      VTApl   *apl;
      VTLocks *locks;

   private: // dados
      TList  *lisCHV;
      TList  *lisALT;
      TList  *lisLOCK;
   };

#endif
//---------------------------------------------------------------------------
//eof
