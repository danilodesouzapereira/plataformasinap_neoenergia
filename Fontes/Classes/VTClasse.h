//---------------------------------------------------------------------------
#ifndef VTClasseH
#define VTClasseH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum classeCONSUMIDOR{consRESIDENCIAL=0, consCOMERCIAL, consINDUSTRIAL,
                      consRURAL, consOUTROS, consIP, consA4, consEQUIVALENTE, consA2,
                      consNULO, consINDEFINIDO=-1};

//---------------------------------------------------------------------------
class VTClasse : public TObject
   {
   protected: //dados acessados via property
      struct   {
               int        id;
               int        tag;  //ver enum classeCONSUMIDOR
               AnsiString codigo;
               bool       tipico;
               TColor     color;
               } PD;

   public:  //property
      __property AnsiString Codigo = {read=PD.codigo, write=PD.codigo };
      __property int        Id     = {read=PD.id,     write=PD.id     };
      __property int        Tag    = {read=PD.tag,    write=PD.tag    };
      __property bool       Tipico = {read=PD.tipico, write=PD.tipico };
      __property TColor     Color  = {read=PD.color,  write=PD.color  };

   public:
      __fastcall  VTClasse(void) {};
      __fastcall ~VTClasse(void) {};
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTClasse
//---------------------------------------------------------------------------
VTClasse* __fastcall NewObjClasse(void);

//---------------------------------------------------------------------------
#endif
//eof



 