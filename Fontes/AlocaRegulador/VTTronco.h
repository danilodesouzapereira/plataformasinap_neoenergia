//---------------------------------------------------------------------------
#ifndef VTTroncoH
#define VTTroncoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTPrimario;

//---------------------------------------------------------------------------
class VTTronco : public TObject
   {
   public:
                         __fastcall VTTronco(void) {};
      virtual            __fastcall ~VTTronco(void) {};
      virtual void       __fastcall DefinePrimario(VTPrimario *primario, int ind_pat) = 0;
      virtual AnsiString __fastcall CaboRamalBifasico(void) = 0;
      virtual AnsiString __fastcall CaboRamalMonofasico(void) = 0;
      virtual AnsiString __fastcall CaboRamalTrifasico(void) = 0;
      virtual AnsiString __fastcall CaboTroncoBifasico(void) = 0;
      virtual AnsiString __fastcall CaboTroncoMonofasico(void) = 0;
      virtual AnsiString __fastcall CaboTroncoTrifasico(void) = 0;
      virtual int        __fastcall CompRamalBifasico(void) = 0;
      virtual int        __fastcall CompRamalMonofasico(void) = 0;
      virtual int        __fastcall CompRamalTrifasico(void) = 0;
      virtual int        __fastcall CompTroncoBifasico(void) = 0;
      virtual int        __fastcall CompTroncoMonofasico(void) = 0;
      virtual int        __fastcall CompTroncoTrifasico(void) = 0;
      virtual TList*     __fastcall LisBarraTronco(void) = 0;
      virtual TList*     __fastcall LisLigacaoRamal(void) = 0;
      virtual TList*     __fastcall LisLigacaoTronco(void) = 0;
		virtual double     __fastcall MaiorTensaoTronco(int ind_pat) = 0;
      virtual double     __fastcall MenorTensaoTronco(int ind_pat) = 0;
   };

//---------------------------------------------------------------------------
//método global p/ criar objetos VTTronco
//---------------------------------------------------------------------------
VTTronco* __fastcall NewObjTronco(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof