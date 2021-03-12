//---------------------------------------------------------------------------
#ifndef TEnvoltoriaH
#define TEnvoltoriaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEnvoltoria.h"

//---------------------------------------------------------------------------
class TPtoCarga;

//---------------------------------------------------------------------------
class TEnvoltoria : public VTEnvoltoria
   {
   public:
             __fastcall  TEnvoltoria(void);
             __fastcall ~TEnvoltoria(void);
      TList* __fastcall  Executa(TList *lisPTC);

   private:
      TPtoCarga* __fastcall BuscaPtoCargaInicial(void);
      TPtoCarga* __fastcall BuscaSeg(TPtoCarga *ptc1, TPtoCarga *ptc2);
      double     __fastcall CalcAng(double xA, double yA, double xm, double ym, double modA);
      TPtoCarga* __fastcall ExistePtoCarga(TList *lisPTC, int x_cm, int y_cm);


   public:
      struct   {
               TList *lisPTC;  //Lista local com todos PtoCarga
               TList *lisENV;  //Lista local com todos PtoCarga da envoltória
               }local;
   };

#endif
//---------------------------------------------------------------------------
//eof

