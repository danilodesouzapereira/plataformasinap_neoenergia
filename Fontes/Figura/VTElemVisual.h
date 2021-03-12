//---------------------------------------------------------------------------
#ifndef VTElemVisualH
#define VTElemVisualH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum TipoElemV {elemvFIGURA=1,elemvESTILO};
//---------------------------------------------------------------------------
enum elemVisualSTATUS //status do elemento visual
{
	sttEV_NOVO          =0x00000001,
	sttEV_ALTERADO      =0x00000002,
	sttEV_EXCLUIDO      =0x00000004,
};
//---------------------------------------------------------------------------
class VTEstilo;
//---------------------------------------------------------------------------
class VTElemVisual : public TObject
   {
   public:  //property
	  __property int        Id      				  = {read=id,        write=id};
	  __property TObject*   Object  				  = {read=object,    write=object};
	  __property int        Tag    					  = {read=tag ,      write=tag};
	  __property int        TipoElemVisual			  = {read=PM_Tipo};
	  __property bool       Status[unsigned mask_bit] = {read=PM_GetStatus, write=PM_SetStatus};
	  __property unsigned   StatusAsUnsigned          = {read=PM_GetStatusAsUnsigned,write=PM_SetStatusAsUnsigned};

   public:
				   __fastcall  VTElemVisual(void);
      virtual	   __fastcall ~VTElemVisual(void);
	  virtual void __fastcall  DefineStatus(unsigned mask_bit, bool enabled);
	  virtual bool __fastcall StatusAlterado(void);
	  virtual bool __fastcall StatusExcluido(void);
	  virtual bool __fastcall StatusNovo(void);

   protected:  //métodos acessados via property
	  virtual bool       __fastcall PM_GetStatus(unsigned bit);
	  virtual unsigned   __fastcall PM_GetStatusAsUnsigned(void);
	  virtual void   __fastcall PM_SetStatus(unsigned mask_bit, bool enabled);
	  virtual void   __fastcall PM_SetStatusAsUnsigned(unsigned status);
	  virtual int    __fastcall PM_Tipo(void) = 0;


   protected:  //dados acessados via property
		int       id;
		int       tag;
		TObject   *object;
		unsigned  status;  //ver
   };

#endif
//---------------------------------------------------------------------------
//eof

