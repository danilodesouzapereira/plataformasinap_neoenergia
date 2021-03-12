//---------------------------------------------------------------------------
#ifndef VTLocusH
#define VTLocusH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
enum tipoPar  {//tipos
                 tipoCont=0,
                 tipoBin,
                 tipoInt
                };

//---------------------------------------------------------------------------
class VTLocus: public TObject
   {
   public:
      //__property int itipo = {read=PM_GetTipo, write=PM_SetTipo};

   public:
                   __fastcall VTLocus(void) {};
                   __fastcall ~VTLocus(void) {};

   protected:  //m�todos acessados via property
      //virtual int  __fastcall PM_GetTipo(void) = 0;
      //virtual void __fastcall PM_SetTipo(int i) = 0;

   public:
      int itipo;
   public:
     AnsiString  nome;
	  //Par�metros gerais
	  VTLocus*    locus_ori;//posi��o origem
	  int         imostra;//assume valores 0: X ou 1: Y para cada vari�vel
	  //Par�metros: vari�veis inteiras
	  double      p; //valor e desvio
	  double      s; //desvio da busca
	  double      minP, maxP; //Limitantes
	  double      minS, maxS; //Limitantes
	  //TIRAR Par�metros: vari�veis bin�rias e inteiras
      //int         ival;


   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTDespacho
//---------------------------------------------------------------------------
VTLocus*  NewObjLocus(void);
VTLocus*  NewObjLocusDesp(VTLocus* ploc_0);
VTLocus*  NewObjLocusReconf(VTLocus* ploc_0);
VTLocus*  NewObjLocusRecRede(VTLocus* ploc_0);
VTLocus*  NewObjLocusGD(VTLocus* ploc_0);
VTLocus*  NewObjLocusTurmas(VTLocus* ploc_0);
VTLocus*  NewObjLocusPosIF(VTLocus* ploc_0);
VTLocus*  NewObjLocusDistribIF(VTLocus* ploc_0);

//---------------------------------------------------------------------------
#endif
