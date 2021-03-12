//---------------------------------------------------------------------------
#ifndef TDelDirH
#define TDelDirH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Thread\VTProcesso.h>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TDelDir : public TThread
   {
   public:
		   __fastcall  TDelDir(VTApl *apl);
		   __fastcall ~TDelDir(void);

   protected:  //metodos acessados via property

   private: //metodos
	  bool __fastcall DeleteDiretorioParaLixeira(AnsiString dir);
	  bool __fastcall Executa(void);
	  void __fastcall Execute(void);
	  void __fastcall Terminate(void);

   private: //objetos externos
      VTApl     *apl;

   private: //dados locais
	  TEvent *event;
   };

#endif
//---------------------------------------------------------------------------
//eof

