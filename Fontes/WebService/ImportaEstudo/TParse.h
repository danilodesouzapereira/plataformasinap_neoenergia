//---------------------------------------------------------------------------
#ifndef TParseH
#define TParseH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTParse.h"

//---------------------------------------------------------------------------
class VTExtrator;

//---------------------------------------------------------------------------
class TParse : public VTParse
   {
   public:
           __fastcall  TParse(VTApl *apl);
           __fastcall ~TParse(void);
      bool __fastcall  Executa(AnsiString arq_txt);

   private: //métodos
      int __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
      int  __fastcall PalavraChave(void);
      void __fastcall TrataCampos(void);

   private: //objetos externos
      VTApl       *apl;
      VTExtrator  *extrator;

   private: //dados locais
      enum     tipoDADO {tipoINVALIDO=-1,
                         tipoDUMMY=0,
                         tipoESTUDO=1,
                         tipoALTERNATIVA,
                         tipoOBRA,
                         tipoREDE_ALTERNATIVA,
                         tipoDEMANDA_POTENCIA,
						 tipoPOTENCIA_INSTALADA,
						 tipoELEMENTO_POTENCIA, //DVK 2015.04.28
                         tipoINFORMACAO_TECNICA,
                         tipoVALOR,
						 tipoEQUIPAMENTO,
						 tipoREDE,
						 tipoSOLICITACAO,
                         tipoMUNICIPIO
                         };
      int      estado;
      TStrings *campos;
      TStrings *lines;
   };

#endif
//---------------------------------------------------------------------------
//eof

