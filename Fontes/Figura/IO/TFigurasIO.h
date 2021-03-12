//---------------------------------------------------------------------------
#ifndef TFigurasIOH
#define TFigurasIOH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFigurasIO.h"

//---------------------------------------------------------------------------
class VTFiguras;
class VTInfoset;
class VTRedes;
class VTObra;


//---------------------------------------------------------------------------
class TFigurasIO : public VTFigurasIO
   {
   public:
           __fastcall  TFigurasIO(VTApl *apl_owner);
           __fastcall ~TFigurasIO(void);
	  bool __fastcall  ImportaTXT(VTEstudo *estudo, AnsiString full_filepath, TList* lisREDE_SELE = NULL);
	  bool __fastcall  ExportaTXT(VTEstudo *estudo, AnsiString full_filepath);

   private:  //métodos
	  VTObra* __fastcall ExisteObra(int obra_id);
      bool __fastcall FiguraFilename(AnsiString &filename);
      int  __fastcall NextComentarioId(void);
      bool __fastcall ReadComentario(void);
      bool __fastcall ReadComentarioTexto(void);
      bool __fastcall ReadComentarioLink(void);
      bool __fastcall ReadRetangulo(void);
      bool __fastcall WriteComentario(VTEstudo *estudo);
      bool __fastcall WriteRetangulo(VTEstudo *estudo);


   private:  //objetos externos
      VTApl       *apl;
      VTFiguras   *figuras;
      VTInfoset   *infoset;
      VTRedes     *redes;
	  VTEstudo *estudo;

   private:  //dados locais
      struct{
            int comentario;
            int link;
			}next_id;
	  TList *lisOBRA;
	  TList *lisREDES;
   };

#endif
//---------------------------------------------------------------------------
//eof