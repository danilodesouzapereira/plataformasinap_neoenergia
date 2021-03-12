
//---------------------------------------------------------------------------
#ifndef TGrupoTriplaH
#define TGrupoTriplaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TTripla;

//---------------------------------------------------------------------------
class TGrupoTripla: public TObject
   {
   public:
                         __fastcall  TGrupoTripla(TTripla *tripla);
                         __fastcall ~TGrupoTripla(void);
                    void __fastcall  AddTripla(TTripla *tripla);
                    void __fastcall  ImpoeAterramentoMaisSignificativo(void);

   public: // Dados
      bool  tipo_carga;     // Indica se o grupo é composto por triplas de nós de carga
      bool  neutro;         // Indica se o grupo é de triplas de neutro ou de fase (A, B, C)
      bool  aterr_atSolido; // Indica se o grupo de triplas de neutro é atSolido
      TList *lisTRIPLA;     // Lista de triplas do grupo
   };

#endif
//---------------------------------------------------------------------------
//eof
