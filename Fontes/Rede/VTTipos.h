//---------------------------------------------------------------------------
#ifndef VTTiposH
#define VTTiposH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTTipoChave;
class VTTipoRede;

//---------------------------------------------------------------------------
class VTTipos : public TObject
   {
   public:
                           __fastcall  VTTipos(void) {};
      virtual              __fastcall ~VTTipos(void) {};
      virtual void         __fastcall  Clear(void) = 0;
      virtual VTTipoChave* __fastcall  ExisteTipoChave(int tipochave_id) = 0;
      virtual VTTipoChave* __fastcall  ExisteTipoChave(AnsiString codigo, int tipo=-1) = 0;
      virtual VTTipoRede*  __fastcall  ExisteTipoRede(int tipo_rede_id) = 0;
      virtual VTTipoRede*  __fastcall  ExisteTipoRede(AnsiString codigo, int segmento=-1) = 0;
      virtual void         __fastcall  InsereTipoChave(VTTipoChave *tipochave) = 0;
      virtual void         __fastcall  InsereTipoRede(VTTipoRede *tiporede) = 0;
      virtual TList*       __fastcall  LisTipoChave(void) = 0;
      virtual TList*       __fastcall  LisTipoRede(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTipos
//---------------------------------------------------------------------------
VTTipos* __fastcall NewObjTipos(void);

//---------------------------------------------------------------------------
#endif
//eof

 