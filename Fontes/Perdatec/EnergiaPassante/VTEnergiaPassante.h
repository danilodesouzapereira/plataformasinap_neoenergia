//---------------------------------------------------------------------------
#ifndef VTEnergiaPassanteH
#define VTEnergiaPassanteH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
enum  enumTCATPT
{
	catptIND = -1,    //não utilizado
	catptA1 = 0,      //não utilizado
	catptA1_A2,
	catptA1_A3,
	catptA1_A3A,
	catptA1_A4,
	catptA2,         //não utilizado
	catptA2_A3,
	catptA2_A3A,
	catptA2_A4,
	catptA3,         //não utilizado
	catptA3_A2,
	catptA3_A3A,
	catptA3_A4,
	catptA3A_A2,
	catptA3A_A4,
	catptA3A_B,
	catptA4,         //não utilizado
	catptA4_A2,
	catptA4_A3,
	catptA4_A3A,
	catptA4_A4,
	catptA4_B,
	catptB,          //não utilizado
	catptB_A3A,
	catptB_A4,
	catptMED,       //não utilizado
	catptRAM,       //não utilizado
	catptREG,       //não utilizado
	catptCOUNT
};
//---------------------------------------------------------------------------
class VTEnergiaPassante : public TObject
   {
   public:  //property
	  __property TObject*   Object                 			  = {read=PM_GetObject,                 write=PM_SetObject                 };
	  __property AnsiString Descricao              			  = {read=PM_GetDescricao,              write=PM_SetDescricao              };
	  __property double     EnergiaRecebidaExt_mwh[int catPt] = {read=PM_GetEnergiaRecebidaExt_mwh, write=PM_SetEnergiaRecebidaExt_mwh };
	  __property double     EnergiaEntregueExt_mwh[int catPt] = {read=PM_GetEnergiaEntregueExt_mwh, write=PM_SetEnergiaEntregueExt_mwh };

   public:
                     __fastcall  VTEnergiaPassante(void) {};
      virtual        __fastcall ~VTEnergiaPassante(void) {};
      virtual bool   __fastcall  Agrega(VTEnergiaPassante *energiaPassante) = 0;

   protected:  //métodos acessados via property
	  virtual AnsiString __fastcall PM_GetDescricao(void) = 0;
	  virtual double     __fastcall PM_GetEnergiaEntregueExt_mwh(int catPt) = 0;
	  virtual double     __fastcall PM_GetEnergiaRecebidaExt_mwh(int catPt) = 0;
      virtual TObject*   __fastcall PM_GetObject(void) = 0;
      //
	  virtual void       __fastcall PM_SetDescricao(AnsiString descricao) = 0;
	  virtual void       __fastcall PM_SetEnergiaEntregueExt_mwh(int catPt, double energia_mwh) = 0;
      virtual void       __fastcall PM_SetEnergiaRecebidaExt_mwh(int catPt, double energia_mwh) = 0;
      virtual void       __fastcall PM_SetObject(TObject *object) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEnergiaPassante
//---------------------------------------------------------------------------
VTEnergiaPassante* __fastcall NewObjEnergiaPassante(void);

//---------------------------------------------------------------------------
#endif
//eof



