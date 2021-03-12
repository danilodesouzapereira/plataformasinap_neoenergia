//---------------------------------------------------------------------------
#ifndef VTImportaRegiaoH
#define VTImportaRegiaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
enum tipoCOORDENADA {coord_LATLON, coord_UTM};

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaRegiao : public TObject
   {
   public:  //property
      __property int TipoCoordenada  = {read=PD.tipo_coordenada, write=PD.tipo_coordenada};

   public:
                  __fastcall  VTImportaRegiao(void) {};
      virtual      __fastcall ~VTImportaRegiao(void) {};
      virtual bool __fastcall  Executa(AnsiString arq_txt) = 0;

   protected:  //dados acessados via property
      struct   {
               int tipo_coordenada;
               }PD;
   };

//---------------------------------------------------------------------------
VTImportaRegiao* __fastcall NewObjImportaRegiao(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
