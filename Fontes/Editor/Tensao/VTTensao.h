//---------------------------------------------------------------------------
#ifndef VTTensaoH
#define VTTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;
class VTZona;

//---------------------------------------------------------------------------
class VTTensao : public TObject
   {
   public:
                   __fastcall  VTTensao(void) {};
      virtual      __fastcall ~VTTensao(void) {};
		virtual bool __fastcall  Executa(VTRede *rede, VTBarra *barra_ref, double vff_kv) = 0;
		virtual bool __fastcall  Executa(VTZona *zona, double vff_kv) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTensao
//---------------------------------------------------------------------------
VTTensao* __fastcall NewObjTensao(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 