//---------------------------------------------------------------------------
#ifndef VTMontaEqvH
#define VTMontaEqvH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;

//---------------------------------------------------------------------------
class VTMontaEqv : public TObject
   {
   public:
                     __fastcall VTMontaEqv(void) {};
                     __fastcall ~VTMontaEqv(void) {};
      virtual void   __fastcall DefineModeloCarga(double Icte,
                                                  double Scte,
                                                  double Zcte) = 0;
      virtual void   __fastcall DefineModeloEqv(bool equilibrado) = 0;
      virtual bool   __fastcall Executa(VTNet *net, TList *lisLIG, TList *lisBAR,
                                        TList *lisBAR_COR, TList *lisBAR_GER,
                                        int ano_carga, double Sbase,
                                        bool aviso_enabled) = 0;
      virtual TList* __fastcall LisCargaEquivalente(void) = 0;
      virtual TList* __fastcall LisTrafoEquivalente(void) = 0;
      virtual TList* __fastcall LisTrechoEquivalente(void) = 0;
      virtual TList* __fastcall LisYrefEquivalente(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaEqv
//---------------------------------------------------------------------------
VTMontaEqv* __fastcall NewObjMontaEqv(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



