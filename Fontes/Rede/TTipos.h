//---------------------------------------------------------------------------
#ifndef TTiposH
#define TTiposH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTTipos.h"

//---------------------------------------------------------------------------
class TTipos : public VTTipos
   {
   public:
                   __fastcall  TTipos(void);
                   __fastcall ~TTipos(void);
      void         __fastcall  Clear(void);
      VTTipoChave* __fastcall  ExisteTipoChave(int tipochave_id);
      VTTipoChave* __fastcall  ExisteTipoChave(AnsiString codigo, int tipo);
      VTTipoRede*  __fastcall  ExisteTipoRede(int tipo_rede_id);
      VTTipoRede*  __fastcall  ExisteTipoRede(AnsiString codigo, int segmento);
      void         __fastcall  InsereTipoChave(VTTipoChave *tipochave);
      void         __fastcall  InsereTipoRede(VTTipoRede *tiporede);
      TList*       __fastcall  LisTipoChave(void);
      TList*       __fastcall  LisTipoRede(void);

   private: //métodos

   private: //dados externos

   private: //dados locais
      TList *lisTIPOCHAVE; //lista de objetos VTTipoChave
      TList *lisTIPOREDE;  //lista de objetos VTTipoRede
      //lista de objetos duplicados (código igual mas ID diferente)
      struct{
            TList  *lisTIPOCHAVE;
            TList  *lisTIPOREDE;
            }dup;
   };

//---------------------------------------------------------------------------
#endif
//eof
