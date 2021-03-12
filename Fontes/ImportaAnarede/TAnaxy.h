//---------------------------------------------------------------------------
#ifndef TAnaxyH
#define TAnaxyH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTAnaxy.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTEqbar;
class VTLigacao;
class VTRedes;

//---------------------------------------------------------------------------
class TAnaxy : public VTAnaxy
   {
   public:
           __fastcall  TAnaxy(VTApl *apl);
           __fastcall ~TAnaxy(void);
      bool __fastcall  Executa(TStrings *Lines);


   private: //objetos externos
      VTApl   *apl;
      VTRedes *redes;

   private: //dados
      #define     DIM_BAR  48    //tamanho "empírico" da barra
      #define     DX   400000    //valor de deslocamento do x, pois pode ser negativo
      #define     DY   750000    //valor de deslocamento do y, pois pode ser negativo
      enum enumESTADO {estINICIO=1, estCOORD, estLINHA, estLIGA, estFIM};
      TStringList *campos;
      TList       *lisBAR, *lisEQP, *lisALT;
      VTLigacao   *ligacao;
      struct strCOMPONENTE  {
                            AnsiString numero;
                            int tipo;
                            int rot;
                            int hac;
                            double px;
                            double py;
                            int cor;
                            int sl;
                            AnsiString subtp;
                            AnsiString ch1;
                            AnsiString ch2;
                            AnsiString ch3;
                            AnsiString ch4;
                            AnsiString descrip;
                            };
      struct strLINHAS      {
                            int numero;
                            int tipo;
                            int hac;
                            int sl;
                            AnsiString de;
                            AnsiString para;
                            int ch1;
                            int nsg;
                            int segmentos;
                            };
      struct strLIGACOES    {
                            int numero;
                            int flags;
                            int cor;
                            AnsiString inic;
                            int coni;
                            AnsiString fin;
                            int conf;
                            int linhas;
                            int nqb;
                            };

   private: //métodos
      void       __fastcall AlteraBarra(strCOMPONENTE *componente);
      void       __fastcall AlteraBarrasChaves(VTBarra  *barra);
      void       __fastcall AlteraBarrasTrechos(void);
      void       __fastcall AlteraCapacitor(strCOMPONENTE *componente);
      void       __fastcall AlteraCarga(strCOMPONENTE *componente);
//      void       __fastcall AlteraGerador(strCOMPONENTE *componente);
      void       __fastcall AlteraLigacao(strCOMPONENTE *componente);
      void       __fastcall AlteraLigacao(strLIGACOES *liga);
      void       __fastcall AlteraLigacao(strLINHAS *linha);
      void       __fastcall AlteraSuprimento(strCOMPONENTE *componente);
      void       __fastcall ClonaBarra(VTBarra *barra);
      bool       __fastcall Comentario(AnsiString txt);
      void       __fastcall CriaChave(VTBarra *barra, VTLigacao *ligacao);
      VTBarra*   __fastcall ExisteBarra(AnsiString cod_bar);
      VTBarra*   __fastcall ExisteBarraExtId(AnsiString ext_id);
      VTEqbar*   __fastcall ExisteEqbar(AnsiString cod, int tipo);
      VTEqbar*   __fastcall ExisteEqbar(AnsiString ext_id, VTBarra *barra);
      VTLigacao* __fastcall ExisteChave(AnsiString ext_id, VTBarra *barra);
      VTLigacao* __fastcall ExisteLigacao(AnsiString ext_id);
      VTLigacao* __fastcall ExisteLigacao(VTBarra *barra1, VTBarra *barra2, int num=-1);
      void       __fastcall InsereChaves(void);
      void       __fastcall LeComponente(strCOMPONENTE *componente);
      void       __fastcall LeLigacoes(strLIGACOES *liga);
      void       __fastcall LeLinha(strLINHAS *linha);
      void       __fastcall LisEqbar(AnsiString cod, int tipo, TList *lisEQB);
      void       __fastcall RedefineBarrasClone(VTLigacao *ligacao);
      void       __fastcall RedefineEqbar(VTEqbar *eqbar, int valor);
      void       __fastcall RedefineLigacao(VTLigacao *ligacao, VTBarra *barra, int valor);
	  void       __fastcall Visivel(bool mostra);
	  void       __fastcall Visivel(bool mostra, TList *lisEQPT);
   };

#endif

//-----------------------------------------------------------------------------
// eof
