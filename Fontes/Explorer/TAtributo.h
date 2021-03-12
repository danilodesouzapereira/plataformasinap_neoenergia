//---------------------------------------------------------------------------
#ifndef TAtributoH
#define TAtributoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAtributo.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCapacitor;
class VTCarga;
class VTChave;
class VTEqbar;
class VTGerador;
class VTLigacao;
class VTSuprimento;
class VTTrafo;
class VTTrecho;
class VTRede;
class VTYref;
class VTConsulta;
class VTGeral;

//---------------------------------------------------------------------------
class TAtributo : public VTAtributo
   {
   public:
                 __fastcall TAtributo(VTApl *apl);
                 __fastcall ~TAtributo(void);
      int        __fastcall Count(void);
      void       __fastcall Executa(VTEqpto *eqpto);
      void       __fastcall NomeValor(int index, AnsiString &nome, AnsiString &valor);
      AnsiString __fastcall Valor(AnsiString nome);

   private:
      double __fastcall ExtensaoTrecho(TList *lisTRE);
      void   __fastcall InsereLinhaSeparacao(void);
      void   __fastcall MontaBarra(VTBarra *pbarra);
      void   __fastcall MontaCapacitor(VTCapacitor *pcap);
      void   __fastcall MontaCarga(VTCarga *pcarga);
      void   __fastcall MontaChave(VTChave *pchave);
      void   __fastcall MontaEqbar(VTEqbar *eqbar);
      void   __fastcall MontaEqpto(VTEqpto *eqpto);
      void   __fastcall MontaGerador(VTGerador *pgera);
      void   __fastcall MontaLigacao(VTLigacao *pliga);
      void   __fastcall MontaRede(VTRede *rede);
      void   __fastcall MontaSuprimento(VTSuprimento *psup);
      void   __fastcall MontaTrafo(VTTrafo *ptrafo);
      void   __fastcall MontaTrecho(VTTrecho *ptrecho);
		void   __fastcall MontaYref(VTYref *yref);

   private: //dados
      VTApl       *apl;
      VTConsulta  *consulta;
      VTGeral     *geral;
      TStringList *slNome, *slValor;
   };

#endif
//---------------------------------------------------------------------------
//eof


