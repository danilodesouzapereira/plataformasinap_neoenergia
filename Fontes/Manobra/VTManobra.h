//---------------------------------------------------------------------------
#ifndef VTManobraH
#define VTManobraH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTChave;
class VTLocked;

//---------------------------------------------------------------------------
class VTManobra : public TObject
   {
   public:	//property
		__property int        Id                   = {read=PM_GetId,         write=PM_SetId};
		__property AnsiString Codigo               = {read=PM_GetCodigo,     write=PM_SetCodigo};
		__property bool       Reversivel           = {read=PM_GetReversivel, write=PM_SetReversivel};
		__property bool       Status[unsigned bit] = {read=PM_GetStatus,     write=PM_SetStatus};

   public:
                        __fastcall VTManobra(void) {};
      virtual           __fastcall ~VTManobra(void) {};
      virtual bool      __fastcall ExisteChave(VTChave *chave) = 0;
      virtual VTLocked* __fastcall ExisteLocked(VTChave *chave, int estado) = 0;
		virtual VTLocked* __fastcall InsereLocked(VTChave *chave, int estado) = 0;
		virtual void      __fastcall LisChave(TList *lisEXT) = 0;
		virtual TList*    __fastcall LisLocked(void) = 0;
		virtual void      __fastcall RetiraLocked(VTLocked *locked) = 0;
		virtual void      __fastcall RetiraLocked(VTChave *chave) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
		virtual int        __fastcall PM_GetId(void) = 0;
		virtual bool       __fastcall PM_GetReversivel(void) = 0;
		virtual bool       __fastcall PM_GetStatus(unsigned bit) = 0;
      virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
      virtual void       __fastcall PM_SetId(int id) = 0;
		virtual void       __fastcall PM_SetReversivel(bool reversivel) = 0;
		virtual void       __fastcall PM_SetStatus(unsigned bit, bool enabled) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTManobra* __fastcall NewObjManobra(void);

#endif
//---------------------------------------------------------------------------
//eof
