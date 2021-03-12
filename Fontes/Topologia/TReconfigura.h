//---------------------------------------------------------------------------
#ifndef TReconfiguraH
#define TReconfiguraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReconfigura.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTRede;

//---------------------------------------------------------------------------
class TReconfigura : public VTReconfigura
   {
   public:
			  __fastcall  TReconfigura(VTApl *apl);
           __fastcall ~TReconfigura(void);
		bool __fastcall  Executa(VTRedes *redes, TList *lisREDE);

   private: //métodos
      void       __fastcall LimpaListas(void);
      bool       __fastcall Ordena(VTRede *rede);
      bool       __fastcall OrdenaLigacaoIsolada(VTRede *rede);
      void       __fastcall ReiniciaLisLigBarras(TList *lisBARRA);

   private: //dados externos
		VTApl   *apl;
		TList   *lisREDE;
		VTRedes *redes;

   private: //dados locais
      TList *lisL;      //lista de Ligacoes NÃO ordenadas de uma Rede
      TList *lisBAR;    //lista de Barras       ordenadas de uma Rede
      TList *lisLIG;    //lista de Ligacoes     ordenadas de uma Rede
      TList *lisCHV;    //lista de Chaves       abertas   de todas Redes
      TList *lisISO;    //lista de Ligacoes     isoladas  de todas Redes
   };

#endif
//---------------------------------------------------------------------------
//eof

