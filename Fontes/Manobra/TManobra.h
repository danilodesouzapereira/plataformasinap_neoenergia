//---------------------------------------------------------------------------
#ifndef TLockH
#define TManobraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTManobra.h"

//---------------------------------------------------------------------------
class TManobra : public VTManobra
   {
   public:
                __fastcall TManobra(void);
                __fastcall ~TManobra(void);
      bool      __fastcall ExisteChave(VTChave *chave);
      VTLocked* __fastcall ExisteLocked(VTChave *chave, int estado);
		VTLocked* __fastcall InsereLocked(VTChave *chave, int estado);
		void      __fastcall LisChave(TList *lisEXT);
      TList*    __fastcall LisLocked(void);
      void      __fastcall RetiraLocked(VTLocked *locked);
      void      __fastcall RetiraLocked(VTChave *chave);

   protected:  //métodos acessados via property
      AnsiString __fastcall PM_GetCodigo(void);
		int        __fastcall PM_GetId(void);
		bool       __fastcall PM_GetReversivel(void);
      bool       __fastcall PM_GetStatus(unsigned bit);
      void       __fastcall PM_SetCodigo(AnsiString codigo);
		void       __fastcall PM_SetId(int id);
		void       __fastcall PM_SetReversivel(bool reversivel);
      void       __fastcall PM_SetStatus(unsigned bit, bool enabled);

   private: //métodos
		VTLocked* __fastcall ExisteLocked(VTChave *chave);

   private: // dados
      TList *lisLOCKED;  //lista de objetos Locked
      struct   {
               int        id;
					int        status;
					bool       reversivel;
               AnsiString codigo;
               } PD;
   };

#endif
//---------------------------------------------------------------------------
//eof
