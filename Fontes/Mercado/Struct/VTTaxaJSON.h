//---------------------------------------------------------------------------
#ifndef VTTaxaJSONH
#define VTTaxaJSONH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <sysdyn.h>
//---------------------------------------------------------------------------
//class VTArea;
class VTClasse;
class VTSegmento;

//---------------------------------------------------------------------------
class VTTaxaJSON : public TObject
   {
   public:  //property
	  __property int         Ano         = {read=PM_GetAno, write=PM_SetAno};
      __property double      Valor_Percent      = {read=PM_GetTaxa_perc, write = PM_SetTaxa_perc};
      

   public:  //funções públicas
                             __fastcall VTTaxaJSON(void) {};
      virtual                __fastcall ~VTTaxaJSON(void) {};
//      virtual bool           __fastcall InserePeriodo(int ano_ini, int ano_fim, double taxa_pu) = 0;
//      virtual TList*         __fastcall LisPeriodo(void) = 0;
//      virtual bool           __fastcall Nulo(void) = 0;
//      virtual void           __fastcall RedefinePeriodo(int ano_ini, int ano_fim) = 0;
//      virtual void           __fastcall Reinicia(void) = 0;

   protected: //métodos acessados via property
      // GET
	  virtual int        __fastcall PM_GetAno(void) = 0;
	  virtual double     __fastcall PM_GetTaxa_perc(void) = 0;
	  // SET
	  virtual void        __fastcall PM_SetAno(int ano) = 0;
	  virtual void        __fastcall PM_SetTaxa_perc(double valor) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTaxaJSON
//---------------------------------------------------------------------------
VTTaxaJSON* __fastcall NewObjTaxaJSON(void);

//---------------------------------------------------------------------------
#endif
//eof
