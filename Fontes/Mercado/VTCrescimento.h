//---------------------------------------------------------------------------
#ifndef VTCrescimentoH
#define VTCrescimentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <sysdyn.h>
//---------------------------------------------------------------------------
//class VTArea;
class VTClasse;
class VTSegmento;

//---------------------------------------------------------------------------
class VTCrescimento : public TObject
   {
   public:  //property
      __property AnsiString  Codigo             = {read=PM_GetCodigo};
      __property int         Id                 = {read=PD.id,            write=PD.id};
		__property VTClasse    *Classe            = {read=PM_GetClasse,     write=PM_SetClasse};
		__property VTSegmento  *Segmento          = {read=PD.segmento,      write=PD.segmento};
      __property int         AnoFinal           = {read=PM_GetAnoFinal};
      __property int         AnoInicial         = {read=PM_GetAnoInicial};
      __property int         NumPeriodo         = {read=PM_GetNumPeriodo};
      __property double      Taxa_perc[int ano] = {read=PM_GetTaxa_perc,  write=PM_SetTaxa_perc};
      __property double      Taxa_pu[int ano]   = {read=PM_GetTaxa_pu,    write=PM_SetTaxa_pu};
      __property double      TaxaAcumulada_pu[int index] = {read=PM_GetTaxaAcumulada_pu};

   public:  //funções públicas
                             __fastcall VTCrescimento(void) {};
      virtual                __fastcall ~VTCrescimento(void) {};
      virtual VTCrescimento* __fastcall Clone(void) = 0;
      virtual bool           __fastcall InserePeriodo(int ano_ini, int ano_fim, double taxa_pu) = 0;
      virtual TList*         __fastcall LisPeriodo(void) = 0;
      virtual bool           __fastcall Nulo(void) = 0;
      virtual void           __fastcall RedefinePeriodo(int ano_ini, int ano_fim) = 0;
      virtual void           __fastcall Reinicia(void) = 0;

   protected: //dados acessados via property
      struct   {//IMPORTANTE: um Crescimento está associado a uma Area ou a uma VTClase
               //AnsiString  codigo;
               int         id;
               //VTArea      *area;
               VTClasse    *classe;   //Classe de consumdidor
					VTSegmento  *segmento;
               } PD;

   protected: //métodos acessados via property
      virtual int        __fastcall PM_GetAnoFinal(void) = 0;
      virtual int        __fastcall PM_GetAnoInicial(void) = 0;
      virtual VTClasse*  __fastcall PM_GetClasse(void) = 0;
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
      virtual int        __fastcall PM_GetNumPeriodo(void) = 0;
      virtual double     __fastcall PM_GetTaxa_perc(int ano) = 0;
      virtual double     __fastcall PM_GetTaxa_pu(int ano) = 0;
      virtual double     __fastcall PM_GetTaxaAcumulada_pu(int index) = 0;
      virtual void       __fastcall PM_SetClasse(VTClasse *classe) = 0;
      virtual void       __fastcall PM_SetTaxa_pu(int ano, double taxa) = 0;
      virtual void       __fastcall PM_SetTaxa_perc(int ano, double taxa) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCrescimento
//---------------------------------------------------------------------------
VTCrescimento* __fastcall NewObjCrescimento(void);

//---------------------------------------------------------------------------
#endif
//eof
