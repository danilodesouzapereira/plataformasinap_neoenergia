
//---------------------------------------------------------------------------
#ifndef TTriplaH
#define TTriplaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\..\Constante\Fases.h"

class VTLigacaoG3;
class TBarra;
class TGrupoTripla;

//---------------------------------------------------------------------------
class TTripla : public TObject
   {
   public:
                         __fastcall  TTripla(VTLigacaoG3 *trafo,
                                             int         fase_comum=faseN,
                                             TBarra      *bar1=NULL,
                                             TBarra      *bar2=NULL,
                                             TBarra      *bar3=NULL);
                         __fastcall ~TTripla(void);
                    bool __fastcall  NoComum(TTripla *tr1);

   public: // Dados
      VTLigacaoG3  *trafo;     // Ponteiro para o trafo que gerou a tripla (TTrafo12,
                               // TTrafo32 ou TTrafo33)
      int          fase_comum; // Fase comum da tripla
      TGrupoTripla *grupo;     // Grupo da tripla
      TList        *lisNO;     // Lista de nós da tripla
   };

#endif
//---------------------------------------------------------------------------
//eof
