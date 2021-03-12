//---------------------------------------------------------------------------
#ifndef TDelDirH
#define TDelDirH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TDelDir : public TObject
   {
   public:
		   __fastcall  TDelDir(VTApl *apl);
		   __fastcall ~TDelDir(void);

   protected:  //metodos acessados via property

   private: //metodos
	  bool __fastcall DeleteDiretorioParaLixeira(AnsiString dir);
	  bool __fastcall Executa(void);

   private: //objetos externos
      VTApl     *apl;

   private: //dados locais

   };

#endif
//---------------------------------------------------------------------------
//eof

