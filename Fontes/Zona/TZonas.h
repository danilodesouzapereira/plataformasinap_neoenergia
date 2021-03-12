//---------------------------------------------------------------------------
#ifndef TZonasH
#define TZonasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTZonas.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTGerador;
class VTLigacao;
class VTSuprimento;
class VTTrafo;
class VTTrafo3E;
class VTMNet;
class VTRede;
class VTRedes;
class VTZona;
class VTConsulta;

//---------------------------------------------------------------------------
class TZonas : public VTZonas
   {
   public:
              __fastcall  TZonas(VTApl *apl);
              __fastcall ~TZonas(void);
      void    __fastcall  Clear(VTRedes *redes);
      bool    __fastcall  Executa(VTRedes *redes);
      VTZona* __fastcall  ExisteZona(VTBarra *barra);
      TList*  __fastcall  LisZona(void);

   private: //dados
      void     __fastcall CancelaAssociacaoBarraZona(TList *lisBARRA);
      bool     __fastcall DefineVnomAnguloFases(void);
      bool     __fastcall DefineVnomAnguloZonaTrafo(VTZona *zona, VTTrafo* trafo);
      bool     __fastcall DefineVnomAnguloZonaTrafoMonofasico(VTZona *zona, VTTrafo* trafo);
      bool     __fastcall DefineVnomAnguloZonaTrafoReducao(VTZona *zona, VTTrafo *trafo);
      bool     __fastcall DefineVnomAnguloZonaTrafoTrifasico(VTZona *zona, VTTrafo* trafo);
      bool     __fastcall DefineVnomAnguloZonaTrafo3E(VTZona *zona, VTTrafo3E* trafo);
      void     __fastcall EliminaDefasagemNegativa(void);
      VTZona*  __fastcall ExisteZonaReferencia(void);
      void     __fastcall FundeZonas(VTZona *zona1, VTZona *zona2);
      void     __fastcall IniciaLisBarraLigacao(VTRedes *redes, TList *lisB, TList *lisL);
      void     __fastcall IniciaLisBarraLigacao(VTMNet *mnet, TList *lisB, TList *lisL, int &num_zona, int &num_trf);
      void     __fastcall IniciaLisTrafo(TList *lisDES);
      void     __fastcall IniciaLisTrafoZZ(TList *lisDES);
      void     __fastcall IniciaLisSuprimentoGerador(void);
      void     __fastcall IniciaZonaReferencia(VTZona *zona, VTGerador *gerador);
      void     __fastcall IniciaZonaReferencia(VTZona *zona, VTSuprimento *suprimento);
      bool     __fastcall InsereZona(VTBarra *pbarra);
      bool     __fastcall InsereZona(TList *lisBARRA);
      bool     __fastcall TrafoConectaZona(VTLigacao *pliga, VTZona *zona);
      void     __fastcall VerificaZonaSemTerra(void);

   private: //objetos externos
      VTApl   *apl;
      VTRedes *redes;

   private: //dados locais
      VTConsulta *consulta;
      TList      *lisBAR;
      TList      *lisGER;
      TList      *lisLIG;
      TList      *lisSUP;
      TList      *lisTRF;
      TList      *lisTMP;
      TList      *lisZON;
      TList      *lisINV;     //lista de Geradores/Suprimentos com fases incompatíveis com suas Zonas
      TList      *lisDELTA;   //lista de Zonas do tipo zonaDELTA
   };

#endif
//---------------------------------------------------------------------------
//eof
