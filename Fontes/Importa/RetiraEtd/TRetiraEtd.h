//-----------------------------------------------------------------------------
#ifndef TRetiraEtdH
#define TRetiraEtdH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRetiraEtd.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTEqpto;

//---------------------------------------------------------------------------
class TRetiraEtd : public VTRetiraEtd
	{
   public:
           __fastcall  TRetiraEtd(VTApl *apl);
			  __fastcall ~TRetiraEtd(void);
      bool __fastcall  Executa(void);

   private: //métodos
      void       __fastcall AlteraTensao(VTBarra *barra_ini);
      bool       __fastcall InsereSuprimento(VTBarra *barra);
      void       __fastcall InsereToDel(VTEqpto *eqpto);
      void       __fastcall InsereToDel(TList *lisEQP);

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList     *lisBAR_INI;
      TList     *lisETD;
      TList     *lisPRI;
      TList     *lisEQP;
      TList     *lisDEL;
   };

#endif
//---------------------------------------------------------------------------
//eof
